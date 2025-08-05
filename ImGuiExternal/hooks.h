#pragma once
#include "include.h"
DWORD64 Actors_Hook;
DWORD64 jmpback;

class Actors
{
public:
	char pad_0000[32]; //0x0000
	class Actor_Data* PtrToActor; //0x0020
}; //Size: 0x0080

class Actor_Data
{
public:
	char pad_0000[416]; //0x0000
	class Pos_data* PtrToPos; //0x01A0
	char pad_01A8[1888]; //0x01A8
	class Health_Data* PtrToHealth; //0x0908
	char pad_0910[2417]; //0x0910
	bool arrest_complete; //0x1281
}; //Size: 0x1489

class Pos_data
{
public:
	char pad_0000[296]; //0x0000
	Vector3 Pos; //0x0128
}; //Size: 0x0190

class Health_Data
{
public:
	char pad_0000[216]; //0x0000
	float Health; //0x00D8
	float Max_Health; //0x00DC
}; //Size: 0x0508

Actors* Actor[255];
Actors* ActorsAddr;


__declspec(naked) void GetActors() {
	__asm {
		movups xmm0, xmmword ptr ds : [rdx + 0x128]
		movups xmmword ptr ds : [rbx + 0x80] , xmm0
		mov[ActorsAddr], rdx
		jmp[jmpback]
	}
}