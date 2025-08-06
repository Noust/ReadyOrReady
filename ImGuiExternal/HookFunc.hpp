#pragma once
#include "include.h"


enum offset {
	UworldOffset = 0x8B37258,
	game_instance = 0x1B8,
	localplayer = 0x38,
	player_controller = 0x30,
	acknowledged_pawn = 0x340,
	camera_component = 0x1B18,
	root_component = 0x1A0,
	character_health = 0x908,
	inventory_comp = 0x910,
	last_weapon = 0x1D8,
	camera_manager = 0x350,
	camera_cache = 0x1330
};
//"ReadyOrNotSteam-Win64-Shipping.exe" + 2996840
//"ReadyOrNotSteam-Win64-Shipping.exe"+2EE2CEF

bool Hook(BYTE* pTarget, BYTE* pHook, UINT Length)
{
	if (!pTarget || !pHook || Length < 14)
		return false;

	DWORD dwOld = 0;
	if (!VirtualProtect(pTarget, Length, PAGE_EXECUTE_READWRITE, &dwOld))
		return false;
	memset(pTarget, 0x90, Length);

#ifdef _WIN64
	memset(pTarget, 0x00, 14);
	*(pTarget + 0x00) = 0xFF;
	*(pTarget + 0x01) = 0x25;
	*reinterpret_cast<BYTE**>(pTarget + 0x06) = pHook;
#else
	* pTarget = 0xE9;
	*reinterpret_cast<DWORD*>(pTarget + 0x01) = pHook - pTarget - 5;
#endif
	VirtualProtect(pTarget, Length, dwOld, &dwOld);
	return false;
}

MODULEINFO GetModuleInfo(char* szModule)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);
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

void Patch(BYTE* addr, BYTE* bytes, unsigned int size) {
	DWORD oProc;
	VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &oProc);
	memcpy(addr, bytes, size);
	VirtualProtect(addr, size, oProc, &oProc);;
}