#pragma once
#include "include.h"

template<typename T>
class MemoryReader {
private:
    // Verifica si el tipo es v�lido para leer
    static constexpr bool IsValidType() {
        return std::is_trivially_copyable<T>::value;
    }

    static bool IsAddressValid(uintptr_t address, size_t size) {
        // Verificar rango de direcciones v�lido
        if (address == 0 || address == UINTPTR_MAX ||
            address + size < address || // Overflow check
            address + size > UINTPTR_MAX) {
            return false;
        }

        // Verificar que el rango completo est� en la misma p�gina de memoria
        MEMORY_BASIC_INFORMATION mbi;
        if (!VirtualQuery(reinterpret_cast<LPCVOID>(address), &mbi, sizeof(mbi))) {
            return false;
        }

        // Verificar que todo el rango est� dentro de la regi�n
        if (address + size > (uintptr_t)mbi.BaseAddress + mbi.RegionSize) {
            return false;
        }

        // Verificar permisos de memoria
        if (!(mbi.State == MEM_COMMIT &&
            mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
            return false;
        }

        return true;
    }

public:
    static bool ReadMemory(uintptr_t address, T& output) {
        // Verificaciones iniciales
        if (!IsValidType()) {
            return false;
        }

        if (!IsAddressValid(address, sizeof(T))) {
            return false;
        }

        __try {
            const T* ptr = reinterpret_cast<const T*>(address);

            // Verificar alineaci�n de memoria
            if (reinterpret_cast<uintptr_t>(ptr) % alignof(T) != 0) {
                return false;
            }

            output = {}; // Inicializar output antes de la lectura
            memcpy(&output, ptr, sizeof(T));
            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            output = {}; // Limpiar output en caso de excepci�n
            return false;
        }
    }

    // Sobrecarga que retorna un valor por defecto si falla
    static T ReadMemorySafe(uintptr_t address, const T& defaultValue = T()) {
        T result;
        return ReadMemory(address, result) ? result : defaultValue;
    }
};

template<typename T>
inline bool read(uintptr_t address, T& output) {
    return MemoryReader<T>::ReadMemory(address, output);
}

/*template<typename T>
inline T readIn(uintptr_t address, const T& defaultValue = T()) {
    return MemoryReader<T>::ReadMemorySafe(address, defaultValue);
}*/

template<typename T>
class MemoryWriter {
private:
    static constexpr bool IsValidType() {
        return std::is_trivially_copyable<T>::value;
    }

    // Funci�n auxiliar para cambiar la protecci�n de memoria
    static bool ModifyMemoryProtection(uintptr_t address, size_t size, DWORD newProtection, DWORD& oldProtection) {
        return VirtualProtect(reinterpret_cast<LPVOID>(address), size, newProtection, &oldProtection);
    }

public:
    static bool WriteMemory(uintptr_t address, const T& value) {
        // Verificaciones iniciales
        if (!IsValidType()) {
            return false;
        }

        if (address == 0 || address == UINTPTR_MAX) {
            return false;
        }

        // Verificar alineaci�n
        if (address % alignof(T) != 0) {
            return false;
        }

        DWORD oldProtection;
        bool protectionChanged = false;

        __try {
            // Verificar permisos de memoria actuales
            MEMORY_BASIC_INFORMATION mbi;
            if (!VirtualQuery(reinterpret_cast<LPCVOID>(address), &mbi, sizeof(mbi))) {
                return false;
            }

            // Si la memoria no es escribible, intentar cambiar la protecci�n
            if (!(mbi.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE))) {
                if (!ModifyMemoryProtection(address, sizeof(T), PAGE_EXECUTE_READWRITE, oldProtection)) {
                    return false;
                }
                protectionChanged = true;
            }

            // Escribir el valor
            *reinterpret_cast<T*>(address) = value;

            // Restaurar la protecci�n original si fue cambiada
            if (protectionChanged) {
                ModifyMemoryProtection(address, sizeof(T), oldProtection, oldProtection);
            }

            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            if (protectionChanged) {
                ModifyMemoryProtection(address, sizeof(T), oldProtection, oldProtection);
            }
            return false;
        }
    }

    // Versi�n para escribir arrays
    static bool WriteMemoryArray(uintptr_t address, const T* values, size_t count) {
        if (!values || count == 0) {
            return false;
        }

        DWORD oldProtection;
        bool protectionChanged = false;

        __try {
            // Cambiar protecci�n para todo el array
            if (!ModifyMemoryProtection(address, sizeof(T) * count, PAGE_EXECUTE_READWRITE, oldProtection)) {
                return false;
            }
            protectionChanged = true;

            // Copiar datos
            memcpy(reinterpret_cast<void*>(address), values, sizeof(T) * count);

            // Restaurar protecci�n
            ModifyMemoryProtection(address, sizeof(T) * count, oldProtection, oldProtection);
            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            if (protectionChanged) {
                ModifyMemoryProtection(address, sizeof(T) * count, oldProtection, oldProtection);
            }
            return false;
        }
    }
};

// Funciones helper para uso m�s simple
template<typename T>
inline bool write(uintptr_t address, const T& value) {
    return MemoryWriter<T>::WriteMemory(address, value);
}

template<typename T>
inline bool WriteMemArraySafe(uintptr_t address, const T* values, size_t count) {
    return MemoryWriter<T>::WriteMemoryArray(address, values, count);
}

bool setBooleanBit(DWORD64 addr, int offset, int bitPosition, bool value) {
    if (addr == 0 || addr == UINTPTR_MAX) {
        return false;
    }
    
    if (bitPosition < 0 || bitPosition > 7) {
        return false;
    }
    
    uintptr_t targetAddress = addr + offset;
    
    if (targetAddress < addr) {
        return false;
    }
    
    DWORD oldProtection;
    bool protectionChanged = false;
    
    __try {
        MEMORY_BASIC_INFORMATION mbi;
        if (!VirtualQuery(reinterpret_cast<LPCVOID>(targetAddress), &mbi, sizeof(mbi))) {
            return false;
        }
        
        if (targetAddress < (uintptr_t)mbi.BaseAddress || 
            targetAddress >= (uintptr_t)mbi.BaseAddress + mbi.RegionSize) {
            return false;
        }
        
        if (mbi.State != MEM_COMMIT) {
            return false;
        }
        
        if (!(mbi.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE))) {
            if (!VirtualProtect(reinterpret_cast<LPVOID>(targetAddress), 1, PAGE_EXECUTE_READWRITE, &oldProtection)) {
                return false;
            }
            protectionChanged = true;
        }
        
        char* bytePtr = reinterpret_cast<char*>(targetAddress);
        char currentByte = *bytePtr;
        
        if (value) {
            currentByte |= (1 << bitPosition);
        } else {
            currentByte &= ~(1 << bitPosition);
        }
        
        *bytePtr = currentByte;
        
        if (protectionChanged) {
            VirtualProtect(reinterpret_cast<LPVOID>(targetAddress), 1, oldProtection, &oldProtection);
        }
        
        return true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        if (protectionChanged) {
            VirtualProtect(reinterpret_cast<LPVOID>(targetAddress), 1, oldProtection, &oldProtection);
        }
        return false;
    }
}

struct world {
    uintptr_t uworld;
	DWORD64 gameInstance;
	DWORD64 localPlayer;
	DWORD64 playerController;
	DWORD64 acknowledgedPawn;
    DWORD64 inventorycomp;
    DWORD64 lastweapon;
    DWORD64 magazines;
	DWORD64 cameraComponent;
	DWORD64 rootComponent;
	DWORD64 characterHealth;
	DWORD64 cameraManager;
};
world adresses;

bool ReadUworld();
bool ReadGameInstance();
bool ReadLocalPlayer();
bool ReadPlayerController();
bool ReadAcknowledgedPawn();
bool ReadCameraComponent();
bool ReadRootComponent();
bool ReadCharacterHealth();
bool ReadInventoryComp();
bool ReadLastWeapon();
bool ReadMagazines();
bool ReadCameraManager();
bool ReadCameraCache();

bool ReadValues() {
	if (!ReadUworld()) return false;
	if (!ReadGameInstance()) return false;
	if (!ReadLocalPlayer()) return false;
	if (!ReadPlayerController()) return false;
	if (!ReadAcknowledgedPawn()) return false;
	if (!ReadCameraComponent()) return false;
	if (!ReadRootComponent()) return false;
	if (!ReadCharacterHealth()) return false;
    if (!ReadInventoryComp()) return false;
    if (!ReadLastWeapon()) return false;
    if (!ReadMagazines()) return false;
	if (!ReadCameraManager()) return false;
	if (!ReadCameraCache()) return false;
	return true;
}

bool ReadUworld() {
	return read<uintptr_t>(GetAddr(offset::UworldOffset), adresses.uworld);
}
bool ReadGameInstance() {
    if (adresses.uworld == NULL) return false;
    return read<DWORD64>(adresses.uworld + offset::game_instance, adresses.gameInstance);
}
bool ReadLocalPlayer() {
	if (adresses.gameInstance == NULL) return false;
    DWORD64 LocalPlayerAddr = 0;
    if (!read<DWORD64>(adresses.gameInstance + offset::localplayer, LocalPlayerAddr)) return false;
	return read<DWORD64>(LocalPlayerAddr, adresses.localPlayer);
}
bool ReadPlayerController() {
	if (adresses.localPlayer == NULL) return false;
	return read<DWORD64>(adresses.localPlayer + offset::player_controller, adresses.playerController);
}
bool ReadAcknowledgedPawn() {
	if (adresses.playerController == NULL) return false;
	return read<DWORD64>(adresses.playerController + offset::acknowledged_pawn, adresses.acknowledgedPawn);
}
bool ReadCameraComponent() {
    if (adresses.acknowledgedPawn == NULL) return false;
    return read<DWORD64>(adresses.acknowledgedPawn + offset::camera_component, adresses.cameraComponent);
}
bool ReadRootComponent() {
	if (adresses.acknowledgedPawn == NULL) return false;
	return read<DWORD64>(adresses.acknowledgedPawn + offset::root_component, adresses.rootComponent);
}
bool ReadCharacterHealth() {
	if (adresses.acknowledgedPawn == NULL) return false;
	return read<DWORD64>(adresses.acknowledgedPawn + offset::character_health, adresses.characterHealth);
}
bool ReadInventoryComp() {
    if (adresses.acknowledgedPawn == NULL) return false;
    return read<DWORD64>(adresses.acknowledgedPawn + offset::inventory_comp, adresses.inventorycomp);
}
bool ReadLastWeapon() {
    if (adresses.inventorycomp == NULL) return false;
    return read<DWORD64>(adresses.inventorycomp + offset::last_weapon, adresses.lastweapon);
}
bool ReadMagazines() {
    if (adresses.lastweapon == NULL) return false;
    return read<DWORD64>(adresses.lastweapon + offset::magazines, adresses.magazines);
}
bool ReadCameraManager() {
	if (adresses.playerController == NULL) return false;
	return read<DWORD64>(adresses.playerController + offset::camera_manager, adresses.cameraManager);
}
bool ReadCameraCache() {
    if (adresses.cameraManager == NULL) return false;

    uintptr_t cameraAddress = adresses.cameraManager + offset::camera_cache;

    MEMORY_BASIC_INFORMATION mbi;
    if (!VirtualQuery(reinterpret_cast<LPCVOID>(cameraAddress), &mbi, sizeof(mbi))) {
        return false;
    }

    if (cameraAddress + sizeof(FMinimalViewInfo) > (uintptr_t)mbi.BaseAddress + mbi.RegionSize) {
        return false;
    }

    if (!(mbi.State == MEM_COMMIT &&
        mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
        return false;
    }

    __try {
        POV = *(FMinimalViewInfo*)(cameraAddress);
        return true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}