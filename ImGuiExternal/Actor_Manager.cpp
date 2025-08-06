#include "include.h"


constexpr size_t MAX_ACTORS = 254;
constexpr uintptr_t INVALID_POINTER = 0;

struct ActorCacheEntry {
	Actors* actorPtr;
	bool isValid;
	DWORD lastValidationTime;
	DWORD lastPositionUpdateTime;
	ValidatedActorData cachedData;

	ActorCacheEntry() : actorPtr(nullptr), isValid(false), lastValidationTime(0), lastPositionUpdateTime(0) {}
};

constexpr DWORD CACHE_TTL_MS = 100;        // 100ms para datos est�ticos (salud, arresto)
constexpr DWORD POSITION_TTL_MS = 16;      // 16ms para posici�n (~60 FPS)
ActorCacheEntry actorCache[MAX_ACTORS];

void InvalidateCacheEntry(size_t index) {
	if (index < MAX_ACTORS) {
		actorCache[index] = ActorCacheEntry();
	}
}

void InvalidateAllCache() {
	for (size_t i = 0; i < MAX_ACTORS; i++) {
		actorCache[i] = ActorCacheEntry();
	}
}

bool IsValidActorPointer(Actors* actorPtr) {
	if (!actorPtr || reinterpret_cast<uintptr_t>(actorPtr) == INVALID_POINTER) {
		return false;
	}

	MEMORY_BASIC_INFORMATION mbi;
	if (!VirtualQuery(actorPtr, &mbi, sizeof(mbi))) {
		return false;
	}

	if (mbi.State != MEM_COMMIT ||
		!(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
		return false;
	}

	__try {
		if (!actorPtr->PtrToActor) {
			return false;
		}

		if (!VirtualQuery(actorPtr->PtrToActor, &mbi, sizeof(mbi))) {
			return false;
		}

		if (mbi.State != MEM_COMMIT ||
			!(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
			return false;
		}

		return true;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return false;
	}
}

void CleanupInvalidActors() {
	static size_t currentBatchStart = 0;
	constexpr size_t BATCH_SIZE = 10; // Procesar 10 actores por frame

	size_t endIndex = (currentBatchStart + BATCH_SIZE < MAX_ACTORS) ?
		(currentBatchStart + BATCH_SIZE) : MAX_ACTORS;

	for (size_t i = currentBatchStart; i < endIndex; i++) {
		if (Actor[i] != nullptr && !IsValidActorPointer(Actor[i])) {
			Actor[i] = nullptr; // Limpiar puntero inv�lido
			InvalidateCacheEntry(i); // Tambi�n invalidar entrada de cach� correspondiente
		}
	}

	currentBatchStart = (endIndex >= MAX_ACTORS) ? 0 : endIndex;
}

int FindActorIndex(Actors* actorPtr) {
	for (size_t i = 0; i < MAX_ACTORS; i++) {
		if (Actor[i] == actorPtr) {
			return static_cast<int>(i);
		}
	}
	return -1;
}

int FindEmptySlot() {
	for (size_t i = 0; i < MAX_ACTORS; i++) {
		if (Actor[i] == nullptr) {
			return static_cast<int>(i);
		}
	}
	return -1;
}

bool SafeValidateActorData(Actors* actor, Actor_Data*& outActorData) {
	if (!actor) return false;

	__try {
		outActorData = actor->PtrToActor;
		if (!outActorData) return false;

		MEMORY_BASIC_INFORMATION mbi;
		if (!VirtualQuery(outActorData, &mbi, sizeof(mbi))) {
			return false;
		}

		if (mbi.State != MEM_COMMIT ||
			!(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
			return false;
		}

		return true;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		outActorData = nullptr;
		return false;
	}
}

bool SafeGetActorHealthData(Actor_Data* actorData, float& outHealth, float& outMaxHealth) {
	if (!actorData) return false;

	__try {
		Health_Data* healthData = actorData->PtrToHealth;
		if (!healthData) return false;

		MEMORY_BASIC_INFORMATION mbi;
		if (!VirtualQuery(healthData, &mbi, sizeof(mbi))) {
			return false;
		}

		if (mbi.State != MEM_COMMIT ||
			!(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&healthData->Health) < reinterpret_cast<uintptr_t>(healthData) ||
			reinterpret_cast<uintptr_t>(&healthData->Health) >= reinterpret_cast<uintptr_t>(healthData) + sizeof(Health_Data)) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&healthData->Max_Health) < reinterpret_cast<uintptr_t>(healthData) ||
			reinterpret_cast<uintptr_t>(&healthData->Max_Health) >= reinterpret_cast<uintptr_t>(healthData) + sizeof(Health_Data)) {
			return false;
		}

		outHealth = healthData->Health;
		outMaxHealth = healthData->Max_Health;
		return true;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		outHealth = 0.0f;
		outMaxHealth = 0.0f;
		return false;
	}
}

bool SafeGetActorHealth(Actor_Data* actorData, float& outHealth) {
	float maxHealth;
	return SafeGetActorHealthData(actorData, outHealth, maxHealth);
}

bool SafeGetActorPosition(Actor_Data* actorData, fvector& outPosition) {
	if (!actorData) return false;

	__try {
		Pos_data* posData = actorData->PtrToPos;
		if (!posData) return false;

		MEMORY_BASIC_INFORMATION mbi;
		if (!VirtualQuery(posData, &mbi, sizeof(mbi))) {
			return false;
		}

		if (mbi.State != MEM_COMMIT ||
			!(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&posData->Pos) < reinterpret_cast<uintptr_t>(posData) ||
			reinterpret_cast<uintptr_t>(&posData->Pos) >= reinterpret_cast<uintptr_t>(posData) + sizeof(Pos_data)) {
			return false;
		}

		outPosition = posData->Pos;
		return true;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		outPosition = { 0, 0, 0 };
		return false;
	}
}

bool SafeGetActorArrestStatus(Actor_Data* actorData, bool& outArrestComplete) {
	if (!actorData) return false;

	__try {
		MEMORY_BASIC_INFORMATION mbi;
		if (!VirtualQuery(actorData, &mbi, sizeof(mbi))) {
			return false;
		}

		if (mbi.State != MEM_COMMIT ||
			!(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&actorData->arrest_complete) < reinterpret_cast<uintptr_t>(actorData) ||
			reinterpret_cast<uintptr_t>(&actorData->arrest_complete) >= reinterpret_cast<uintptr_t>(actorData) + sizeof(Actor_Data)) {
			return false;
		}

		outArrestComplete = actorData->arrest_complete;
		return true;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		outArrestComplete = false;
		return false;
	}
}

bool SafeGetActorStateData(Actor_Data* actorData, bool& outBeingCarried, float& outMeleeDamage, float& outMeleeRange, bool& outReportedComplete, bool& outSurrenderComplete) {
	if (!actorData) return false;

	__try {
		MEMORY_BASIC_INFORMATION mbi;
		if (!VirtualQuery(actorData, &mbi, sizeof(mbi))) {
			return false;
		}

		if (mbi.State != MEM_COMMIT ||
			!(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
			return false;
		}

		// Validate each field address
		if (reinterpret_cast<uintptr_t>(&actorData->Being_Carried) < reinterpret_cast<uintptr_t>(actorData) ||
			reinterpret_cast<uintptr_t>(&actorData->Being_Carried) >= reinterpret_cast<uintptr_t>(actorData) + sizeof(Actor_Data)) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&actorData->MeleeDamage) < reinterpret_cast<uintptr_t>(actorData) ||
			reinterpret_cast<uintptr_t>(&actorData->MeleeDamage) >= reinterpret_cast<uintptr_t>(actorData) + sizeof(Actor_Data)) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&actorData->MeleeRange) < reinterpret_cast<uintptr_t>(actorData) ||
			reinterpret_cast<uintptr_t>(&actorData->MeleeRange) >= reinterpret_cast<uintptr_t>(actorData) + sizeof(Actor_Data)) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&actorData->reported_complete) < reinterpret_cast<uintptr_t>(actorData) ||
			reinterpret_cast<uintptr_t>(&actorData->reported_complete) >= reinterpret_cast<uintptr_t>(actorData) + sizeof(Actor_Data)) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&actorData->surrender_complete) < reinterpret_cast<uintptr_t>(actorData) ||
			reinterpret_cast<uintptr_t>(&actorData->surrender_complete) >= reinterpret_cast<uintptr_t>(actorData) + sizeof(Actor_Data)) {
			return false;
		}

		outBeingCarried = actorData->Being_Carried;
		outMeleeDamage = actorData->MeleeDamage;
		outMeleeRange = actorData->MeleeRange;
		outReportedComplete = actorData->reported_complete;
		outSurrenderComplete = actorData->surrender_complete;
		return true;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		outBeingCarried = false;
		outMeleeDamage = 0.0f;
		outMeleeRange = 0.0f;
		outReportedComplete = false;
		outSurrenderComplete = false;
		return false;
	}
}

bool SafeGetActorDetailedHealthData(Actor_Data* actorData, float& outHeadHealth, float& outHeadMaxHealth, float& outLeftArmHealth, float& outLeftArmMaxHealth, 
	float& outLeftLegHealth, float& outLeftLegMaxHealth, float& outRightArmHealth, float& outRightArmMaxHealth, float& outRightLegHealth, float& outRightLegMaxHealth) {
	if (!actorData) return false;

	__try {
		Health_Data* healthData = actorData->PtrToHealth;
		if (!healthData) return false;

		MEMORY_BASIC_INFORMATION mbi;
		if (!VirtualQuery(healthData, &mbi, sizeof(mbi))) {
			return false;
		}

		if (mbi.State != MEM_COMMIT ||
			!(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
			return false;
		}

		// Validate each health field address
		if (reinterpret_cast<uintptr_t>(&healthData->HeadHealth) < reinterpret_cast<uintptr_t>(healthData) ||
			reinterpret_cast<uintptr_t>(&healthData->HeadHealth) >= reinterpret_cast<uintptr_t>(healthData) + sizeof(Health_Data)) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&healthData->HeadMaxHealth) < reinterpret_cast<uintptr_t>(healthData) ||
			reinterpret_cast<uintptr_t>(&healthData->HeadMaxHealth) >= reinterpret_cast<uintptr_t>(healthData) + sizeof(Health_Data)) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&healthData->LeftArmHealth) < reinterpret_cast<uintptr_t>(healthData) ||
			reinterpret_cast<uintptr_t>(&healthData->LeftArmHealth) >= reinterpret_cast<uintptr_t>(healthData) + sizeof(Health_Data)) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&healthData->LeftArmMaxHealth) < reinterpret_cast<uintptr_t>(healthData) ||
			reinterpret_cast<uintptr_t>(&healthData->LeftArmMaxHealth) >= reinterpret_cast<uintptr_t>(healthData) + sizeof(Health_Data)) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&healthData->LeftLegHealth) < reinterpret_cast<uintptr_t>(healthData) ||
			reinterpret_cast<uintptr_t>(&healthData->LeftLegHealth) >= reinterpret_cast<uintptr_t>(healthData) + sizeof(Health_Data)) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&healthData->LeftLegMaxHealth) < reinterpret_cast<uintptr_t>(healthData) ||
			reinterpret_cast<uintptr_t>(&healthData->LeftLegMaxHealth) >= reinterpret_cast<uintptr_t>(healthData) + sizeof(Health_Data)) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&healthData->RightArmHealth) < reinterpret_cast<uintptr_t>(healthData) ||
			reinterpret_cast<uintptr_t>(&healthData->RightArmHealth) >= reinterpret_cast<uintptr_t>(healthData) + sizeof(Health_Data)) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&healthData->RightArmMaxHealth) < reinterpret_cast<uintptr_t>(healthData) ||
			reinterpret_cast<uintptr_t>(&healthData->RightArmMaxHealth) >= reinterpret_cast<uintptr_t>(healthData) + sizeof(Health_Data)) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&healthData->RightLegHealth) < reinterpret_cast<uintptr_t>(healthData) ||
			reinterpret_cast<uintptr_t>(&healthData->RightLegHealth) >= reinterpret_cast<uintptr_t>(healthData) + sizeof(Health_Data)) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&healthData->RightLegMaxHealth) < reinterpret_cast<uintptr_t>(healthData) ||
			reinterpret_cast<uintptr_t>(&healthData->RightLegMaxHealth) >= reinterpret_cast<uintptr_t>(healthData) + sizeof(Health_Data)) {
			return false;
		}

		outHeadHealth = healthData->HeadHealth;
		outHeadMaxHealth = healthData->HeadMaxHealth;
		outLeftArmHealth = healthData->LeftArmHealth;
		outLeftArmMaxHealth = healthData->LeftArmMaxHealth;
		outLeftLegHealth = healthData->LeftLegHealth;
		outLeftLegMaxHealth = healthData->LeftLegMaxHealth;
		outRightArmHealth = healthData->RightArmHealth;
		outRightArmMaxHealth = healthData->RightArmMaxHealth;
		outRightLegHealth = healthData->RightLegHealth;
		outRightLegMaxHealth = healthData->RightLegMaxHealth;
		return true;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		outHeadHealth = 0.0f;
		outHeadMaxHealth = 0.0f;
		outLeftArmHealth = 0.0f;
		outLeftArmMaxHealth = 0.0f;
		outLeftLegHealth = 0.0f;
		outLeftLegMaxHealth = 0.0f;
		outRightArmHealth = 0.0f;
		outRightArmMaxHealth = 0.0f;
		outRightLegHealth = 0.0f;
		outRightLegMaxHealth = 0.0f;
		return false;
	}
}

bool SafeGetActorPlayerName(Actor_Data* actorData, wchar_t outPlayerName[22]) {
	if (!actorData) return false;
	
	memset(outPlayerName, 0, 22 * sizeof(wchar_t));

	__try {
		Player_Data* playerData = actorData->PtrToPlayerState;
		if (!playerData) return false;

		MEMORY_BASIC_INFORMATION mbi;
		if (!VirtualQuery(playerData, &mbi, sizeof(mbi))) {
			return false;
		}

		if (mbi.State != MEM_COMMIT ||
			!(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
			return false;
		}

		Name_Data* nameData = playerData->PtrToName;
		if (!nameData) return false;

		if (!VirtualQuery(nameData, &mbi, sizeof(mbi))) {
			return false;
		}

		if (mbi.State != MEM_COMMIT ||
			!(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
			return false;
		}

		if (reinterpret_cast<uintptr_t>(&nameData->Name) < reinterpret_cast<uintptr_t>(nameData) ||
			reinterpret_cast<uintptr_t>(&nameData->Name) >= reinterpret_cast<uintptr_t>(nameData) + sizeof(Name_Data)) {
			return false;
		}

		memcpy(outPlayerName, nameData->Name, 22 * sizeof(wchar_t));
		return true;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		memset(outPlayerName, 0, 22 * sizeof(wchar_t));
		return false;
	}
}

bool SafeGetActorWeaponData(Actor_Data* actorData, uint16_t& outAmmo, uint16_t& outAmmoType, wchar_t outWeaponName[8]) {
	if (!actorData) return false;
	
	outAmmo = 0;
	outAmmoType = 0;
	memset(outWeaponName, 0, 8 * sizeof(wchar_t));

	__try {
		Weapon_Data* weaponData = actorData->PtrToWeapon;
		if (!weaponData) return false;

		MEMORY_BASIC_INFORMATION mbi;
		if (!VirtualQuery(weaponData, &mbi, sizeof(mbi))) {
			return false;
		}

		if (mbi.State != MEM_COMMIT ||
			!(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
			return false;
		}

		LastWeapon_Data* lastWeaponData = weaponData->PtrToLastWeapon;
		if (!lastWeaponData) return false;

		if (!VirtualQuery(lastWeaponData, &mbi, sizeof(mbi))) {
			return false;
		}

		if (mbi.State != MEM_COMMIT ||
			!(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
			return false;
		}

		// Get ammo data
		Magazine_Data* magazineData = lastWeaponData->PtrToMagazine;
		if (magazineData) {
			if (VirtualQuery(magazineData, &mbi, sizeof(mbi))) {
				if (mbi.State == MEM_COMMIT &&
					(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
					
					if (reinterpret_cast<uintptr_t>(&magazineData->Ammo) >= reinterpret_cast<uintptr_t>(magazineData) &&
						reinterpret_cast<uintptr_t>(&magazineData->Ammo) < reinterpret_cast<uintptr_t>(magazineData) + sizeof(Magazine_Data)) {
						outAmmo = magazineData->Ammo;
					}

					if (reinterpret_cast<uintptr_t>(&magazineData->Ammo_Type) >= reinterpret_cast<uintptr_t>(magazineData) &&
						reinterpret_cast<uintptr_t>(&magazineData->Ammo_Type) < reinterpret_cast<uintptr_t>(magazineData) + sizeof(Magazine_Data)) {
						outAmmoType = magazineData->Ammo_Type;
					}
				}
			}
		}

		// Get weapon name
		WeaponName_Data* weaponNameData = lastWeaponData->PtrToWeaponName;
		if (weaponNameData) {
			if (VirtualQuery(weaponNameData, &mbi, sizeof(mbi))) {
				if (mbi.State == MEM_COMMIT &&
					(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
					
					WeaponName_Data1* weaponNameData1 = weaponNameData->PtrToWeaponName1;
					if (weaponNameData1) {
						if (VirtualQuery(weaponNameData1, &mbi, sizeof(mbi))) {
							if (mbi.State == MEM_COMMIT &&
								(mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
								
								if (reinterpret_cast<uintptr_t>(&weaponNameData1->WeaponName) >= reinterpret_cast<uintptr_t>(weaponNameData1) &&
									reinterpret_cast<uintptr_t>(&weaponNameData1->WeaponName) < reinterpret_cast<uintptr_t>(weaponNameData1) + sizeof(WeaponName_Data1)) {
									memcpy(outWeaponName, weaponNameData1->WeaponName, 8 * sizeof(wchar_t));
								}
							}
						}
					}
				}
			}
		}

		return true;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		outAmmo = 0;
		outAmmoType = 0;
		memset(outWeaponName, 0, 8 * sizeof(wchar_t));
		return false;
	}
}

ValidatedActorData SafeGetActorCompleteData(Actors* actor) {
	ValidatedActorData result;

	if (!actor) return result;

	// Buscar en cach� primero
	DWORD currentTime = GetTickCount();
	ActorCacheEntry* cacheEntry = nullptr;
	size_t cacheIndex = MAX_ACTORS;

	for (size_t i = 0; i < MAX_ACTORS; i++) {
		ActorCacheEntry& entry = actorCache[i];

		if (entry.actorPtr == actor) {
			cacheEntry = &entry;
			cacheIndex = i;
			break;
		}
	}

	if (cacheEntry != nullptr) {
		bool staticDataValid = (currentTime - cacheEntry->lastValidationTime) < CACHE_TTL_MS;
		bool positionValid = (currentTime - cacheEntry->lastPositionUpdateTime) < POSITION_TTL_MS;

		if (cacheEntry->isValid && staticDataValid && positionValid) {
			return cacheEntry->cachedData;
		}

		if (cacheEntry->isValid && staticDataValid && !positionValid) {
			Actor_Data* actorData = nullptr;
			if (SafeValidateActorData(actor, actorData)) {
				fvector newPosition;
				if (SafeGetActorPosition(actorData, newPosition)) {
					cacheEntry->cachedData.position = newPosition;
					cacheEntry->cachedData.ptrToActor = actorData;
					cacheEntry->lastPositionUpdateTime = currentTime;
					return cacheEntry->cachedData;
				}
			}
		}

		if (!cacheEntry->isValid && staticDataValid) {
			return result;
		}
	}

	Actor_Data* actorData = nullptr;
	if (!SafeValidateActorData(actor, actorData)) {
		UpdateActorCache(actor, false, result, currentTime);
		return result;
	}

	if (!SafeGetActorHealthData(actorData, result.health, result.maxHealth)) {
		UpdateActorCache(actor, false, result, currentTime);
		return result;
	}

	if (!SafeGetActorPosition(actorData, result.position)) {
		UpdateActorCache(actor, false, result, currentTime);
		return result;
	}

	if (!SafeGetActorArrestStatus(actorData, result.arrestComplete)) {
		UpdateActorCache(actor, false, result, currentTime);
		return result;
	}

	// Get additional actor state data
	if (!SafeGetActorStateData(actorData, result.beingCarried, result.meleeDamage, result.meleeRange, result.reportedComplete, result.surrenderComplete)) {
		UpdateActorCache(actor, false, result, currentTime);
		return result;
	}

	// Get detailed health data for body parts
	if (!SafeGetActorDetailedHealthData(actorData, result.headHealth, result.headMaxHealth, result.leftArmHealth, result.leftArmMaxHealth,
		result.leftLegHealth, result.leftLegMaxHealth, result.rightArmHealth, result.rightArmMaxHealth, result.rightLegHealth, result.rightLegMaxHealth)) {
		UpdateActorCache(actor, false, result, currentTime);
		return result;
	}

	// Get player name (non-critical, continue even if it fails)
	SafeGetActorPlayerName(actorData, result.playerName);

	// Get weapon data (non-critical, continue even if it fails)
	SafeGetActorWeaponData(actorData, result.weaponAmmo, result.weaponAmmoType, result.weaponName);

	result.ptrToActor = actorData;

	// Get pointer addresses for external references
	__try {
		result.ptrToIncapacitatedBy = actorData->PtrToIncapacitatedBy;
		result.ptrToKilledBy = actorData->PtrToKilledBy;
		result.ptrToArrestedBy = actorData->PtrToArrestedBy;
		result.ptrToTakenHostageBy = actorData->PtrToTakenHostageBy;
		
		// Get mesh skeleton pointers
		if (actorData->PtrToMesh) {
			result.ptrToSkeleton = actorData->PtrToMesh->PtrToSkeleton;
			result.ptrToSkeleton1 = actorData->PtrToMesh->PtrToSkeleton1;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		// If pointer retrieval fails, set to nullptr but don't fail the entire function
		result.ptrToIncapacitatedBy = nullptr;
		result.ptrToKilledBy = nullptr;
		result.ptrToArrestedBy = nullptr;
		result.ptrToTakenHostageBy = nullptr;
		result.ptrToSkeleton = nullptr;
		result.ptrToSkeleton1 = nullptr;
	}

	result.isSquad = result.maxHealth == 160 || result.maxHealth == 250;
	result.isCivilian = result.maxHealth == 200 || result.maxHealth == 100;
	result.isEnemy = result.maxHealth == 240 || result.maxHealth > 250;

	result.ActorIncapacitated = result.ptrToIncapacitatedBy != nullptr;
	result.ActorTakenHostage = result.ptrToTakenHostageBy != nullptr;

	result.isValid = true;

	UpdateActorCache(actor, true, result, currentTime);

	return result;
}

void UpdateActorCache(Actors* actor, bool isValid, const ValidatedActorData& data, DWORD currentTime) {
	if (!actor) return;

	size_t targetIndex = MAX_ACTORS;
	size_t oldestIndex = 0;
	DWORD oldestTime = currentTime;

	for (size_t i = 0; i < MAX_ACTORS; i++) {
		ActorCacheEntry& entry = actorCache[i];

		if (entry.actorPtr == actor) {
			targetIndex = i;
			break;
		}

		if (entry.actorPtr == nullptr && targetIndex == MAX_ACTORS) {
			targetIndex = i;
		}

		if (entry.lastValidationTime < oldestTime) {
			oldestTime = entry.lastValidationTime;
			oldestIndex = i;
		}
	}

	if (targetIndex == MAX_ACTORS) {
		targetIndex = oldestIndex;
	}

	ActorCacheEntry& entry = actorCache[targetIndex];
	entry.actorPtr = actor;
	entry.isValid = isValid;
	entry.lastValidationTime = currentTime;
	entry.lastPositionUpdateTime = currentTime;
	entry.cachedData = data;
}

void UpdateActorCache(Actors* actor, bool isValid, const ValidatedActorData& data) {
	UpdateActorCache(actor, isValid, data, GetTickCount());
}

bool FastGetActorPosition(Actors* actor, fvector& outPosition) {
	if (!actor) return false;

	Actor_Data* actorData = nullptr;
	if (!SafeValidateActorData(actor, actorData)) {
		return false;
	}

	return SafeGetActorPosition(actorData, outPosition);
}

void ManageActors() {
	if (USettings.esp) {
		CleanupInvalidActors();

		if (IsValidActorPointer(ActorsAddr)) {
			if (FindActorIndex(ActorsAddr) == -1) {
				int emptySlot = FindEmptySlot();
				if (emptySlot != -1) {
					Actor[emptySlot] = ActorsAddr;
				}
			}
		}
	}
	else {
		for (size_t i = 0; i < MAX_ACTORS; i++) {
			Actor[i] = nullptr;
		}
		InvalidateAllCache();
	}
}

bool shouldloop(ValidatedActorData actorData) {
	if (!success) return false;

	if (!actorData.isValid) return false;

	if (actorData.health < 1) return false;

	fvector actorLocation = actorData.position;
	actorLocation.z -= 80;

	if (POV.Location.distance(actorLocation) / 100 - 1 > USettings.ESP_Distance) return false;

	fvector2d screenlocation = w2s(actorLocation);
	if (screenlocation.x < 0 || screenlocation.x > widthscreen ||
		screenlocation.y < 0 || screenlocation.y > heightscreen) return false;

	if (adresses.acknowledgedPawn == reinterpret_cast<DWORD64>(actorData.ptrToActor)) return false;

	if (actorData.isSquad && !USettings.Show_Squad) return false;
	if (actorData.isCivilian && !USettings.Show_Civilian) return false;
	if (actorData.isEnemy && !USettings.Show_Enemy) return false;

	return true;
}