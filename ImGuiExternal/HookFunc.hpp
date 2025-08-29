#pragma once
#include "include.h"

extern PVOID veh_handle;
extern BYTE* hook_target;


enum offset {
    UworldOffset = 0x8BAE758,
    game_instance = 0x1B8,
    localplayer = 0x38,
    player_controller = 0x30,
    acknowledged_pawn = 0x340,
    camera_component = 0x1B20,
    root_component = 0x1A0,
    character_health = 0x908,
    inventory_comp = 0x910,
    last_weapon = 0x1D8,
    magazines = 0x1608,
    camera_manager = 0x350,
    camera_cache = 0x1330,


	// specific offsets
    health = 0xD8,
    jump = 0x46C,
    speed = 0x2814,
    acceleration = 0x2818,
    god_mode = 0x1768,
	unlimited_health = 0xF4,
    melee_reange = 0x1140,
	unlimited_ammo = 0x162C,
	fire_rate = 0xFD0,
    fov = 0x2A0,
    base_fov = 0x22EC,


	//hook/patch offsets
    fov_hook = 0x2996E10,
    actors_hook = 0x2EE31BF
};
//"ReadyOrNotSteam-Win64-Shipping.exe" + 2996840
//Address of signature = ReadyOrNotSteam - Win64 - Shipping.exe + 0x02EE2CEF
//"\x0F\x10\x82\x00\x00\x00\x00\x0F\x11\x83\x00\x00\x00\x00\xF2\x0F\x10\x8A\x00\x00\x00\x00\xF2\x0F\x11\x8B\x00\x00\x00\x00\x0F\x10\x82\x00\x00\x00\x00\x0F\x11\x83\x00\x00\x00\x00\xF2\x0F\x10\x8A\x00\x00\x00\x00\xF2\x0F\x11\x8B\x00\x00\x00\x00\x0F\x10\x82", "xxx????xxx????xxxx????xxxx????xxx????xxx????xxxx????xxxx????xxx"
//"0F 10 82 ? ? ? ? 0F 11 83 ? ? ? ? F2 0F 10 8A ? ? ? ? F2 0F 11 8B ? ? ? ? 0F 10 82 ? ? ? ? 0F 11 83 ? ? ? ? F2 0F 10 8A ? ? ? ? F2 0F 11 8B ? ? ? ? 0F 10 82"

void GenerateRandomNops(BYTE* buffer, UINT size) {
    BYTE nop_patterns[][4] = {
        {0x90, 0x90, 0x90, 0x90},           // NOP NOP NOP NOP
        {0x66, 0x90, 0x66, 0x90},           // 16-bit NOP, 16-bit NOP  
        {0x0F, 0x1F, 0x40, 0x00},           // Multi-byte NOP
        {0x0F, 0x1F, 0x44, 0x00},           // Multi-byte NOP variant
        {0x48, 0x89, 0xC0, 0x90},           // MOV RAX, RAX; NOP
        {0x48, 0x31, 0xC0, 0x48},           // XOR RAX, RAX; (partial)
    };
    
    for (UINT i = 0; i < size; i += 4) {
        int pattern_idx = rand() % (sizeof(nop_patterns) / sizeof(nop_patterns[0]));
        UINT copy_size = min(4, size - i);
        memcpy(buffer + i, nop_patterns[pattern_idx], copy_size);
    }
}

//bool Hook(BYTE* pTarget, BYTE* pHook, UINT Length)
//{
//	if (!pTarget || !pHook || Length < 14)
//		return false;
//
//	DWORD dwOld = 0;
//	if (!VirtualProtect(pTarget, Length, PAGE_EXECUTE_READWRITE, &dwOld))
//		return false;
//	
//	GenerateRandomNops(pTarget, Length);
//
//#ifdef _WIN64
//	BYTE hookCode[] = {
//		0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // MOV RAX, hook_address
//		0xFF, 0xE0,                                                   // JMP RAX
//		0x90, 0x90                                                    // Padding NOPs
//	};
//	memcpy(hookCode + 2, &pHook, 8);
//	memcpy(pTarget, hookCode, 14);
//#else
//	*pTarget = 0xE9;
//	*reinterpret_cast<DWORD*>(pTarget + 1) = pHook - pTarget - 5;
//#endif
//	
//	VirtualProtect(pTarget, Length, dwOld, &dwOld);
//	FlushInstructionCache(GetCurrentProcess(), pTarget, Length);
//	return true;
//}

MODULEINFO GetModuleInfo(char* szModule)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandleA(szModule);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

DWORD64 FindPattern(char* module, char* pattern, char* mask)
{
	MODULEINFO mInfo = GetModuleInfo(module);

	DWORD64 base = (DWORD64)mInfo.lpBaseOfDll;
	DWORD64 size = (DWORD64)mInfo.SizeOfImage;

	DWORD64 patternLength = (DWORD64)strlen(mask);

	for (DWORD64 i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (DWORD64 j = 0; j < patternLength; j++)
		{
			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
		}
		if (found)
		{
			return base + i;
		}
	}

	return NULL;
}


LONG WINAPI GetActors_VEHHandler(PEXCEPTION_POINTERS ExceptionInfo);

//void VEH_GetActors_Cleanup() {
//    if (hook_target) {
//        HANDLE hThread = GetCurrentThread();
//        if (hThread && hThread != INVALID_HANDLE_VALUE) {
//            CONTEXT ctx = {};
//            ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
//
//            if (GetThreadContext(hThread, &ctx)) {
//                ctx.Dr0 = 0;
//                ctx.Dr7 &= ~(1 << 0);  // Disable Dr0
//                ctx.Dr7 &= ~(3 << 16); // Clear condition bits for Dr0
//                ctx.Dr7 &= ~(3 << 18); // Clear length bits for Dr0
//
//                SetThreadContext(hThread, &ctx);
//            }
//        }
//
//        hook_target = nullptr;
//    }
//
//    if (veh_handle) {
//        if (RemoveVectoredExceptionHandler(veh_handle)) {
//            veh_handle = nullptr;
//        }
//    }
//}

bool VEH_GetActors_Hook(BYTE* pTarget) {
    if (!pTarget)
        return false;

    /*VEH_GetActors_Cleanup();*/

    veh_handle = AddVectoredExceptionHandler(1, GetActors_VEHHandler);
    if (!veh_handle)
        return false;

    hook_target = pTarget;

    CONTEXT ctx = {};
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    
    HANDLE hThread = GetCurrentThread();
    if (!hThread || hThread == INVALID_HANDLE_VALUE) {
        RemoveVectoredExceptionHandler(veh_handle);
        veh_handle = nullptr;
        hook_target = nullptr;
        return false;
    }
    
    if (!GetThreadContext(hThread, &ctx)) {
        RemoveVectoredExceptionHandler(veh_handle);
        veh_handle = nullptr;
        hook_target = nullptr;
        return false;
    }

    ctx.Dr0 = (DWORD64)pTarget;
    ctx.Dr7 |= (1 << 0);      // Enable Dr0
    ctx.Dr7 |= (0 << 16);     // Execute condition (00)
    ctx.Dr7 |= (0 << 18);     // 1 byte length (00)

    if (!SetThreadContext(hThread, &ctx)) {
        RemoveVectoredExceptionHandler(veh_handle);
        veh_handle = nullptr;
        hook_target = nullptr;
        return false;
    }

    return true;
}

void StealthPatch(BYTE* addr, BYTE* bytes, unsigned int size) {
    if (!addr || !bytes || size == 0)
        return;

    Sleep(rand() % 100 + 50);

    DWORD oProc;
    if (!VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &oProc))
        return;
    
    for (unsigned int i = 0; i < size; i += 4) {
        unsigned int chunk_size = min(4, size - i);
        memcpy(addr + i, bytes + i, chunk_size);
        
        for (volatile int j = 0; j < 1000; j++);
    }
    
    VirtualProtect(addr, size, oProc, &oProc);
    
    FlushInstructionCache(GetCurrentProcess(), addr, size);
}

void Patch(BYTE* addr, BYTE* bytes, unsigned int size) {
	DWORD oProc;
	VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &oProc);
	memcpy(addr, bytes, size);
	VirtualProtect(addr, size, oProc, &oProc);
	FlushInstructionCache(GetCurrentProcess(), addr, size);
}