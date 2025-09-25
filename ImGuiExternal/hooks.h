#pragma once
#include <Windows.h>
#include <cstdint>
#include <cstring>
#include "vector.h"

extern DWORD64 Actors_Hook;
extern DWORD64 jmpback;
extern BYTE* hook_target;
extern PVOID veh_handle;

class Actors
{
public:
    char pad_0000[32]; //0x0000
    class Actor_Data* PtrToActor; //0x0020
    char pad_0028[88]; //0x0028
}; //Size: 0x0080

class Actor_Data
{
public:
    char pad_0000[416]; //0x0000
    class Pos_data* PtrToPos; //0x01A0
    char pad_01A8[272]; //0x01A8
    class Player_Data* PtrToPlayerState; //0x02B8
    char pad_02C0[96]; //0x02C0
    class Mesh_Data* PtrToMesh; //0x0320
    char pad_0328[1504]; //0x0328
    class Health_Data* PtrToHealth; //0x0908
    class Weapon_Data* PtrToWeapon; //0x0910
    char pad_0918[1600]; //0x0918
    class ArrestedBy_Data* PtrToArrestedBy; //0x0F58
    class KilledBy_Data* PtrToKilledBy; //0x0F60
    class IncapacitatedBy_Data* PtrToIncapacitatedBy; //0x0F68
    char pad_0F70[472]; //0x0F70
    float MeleeRange; //0x1148
    float MeleeDamage; //0x114C
    char pad_1150[137]; //0x1150
    bool surrender_complete; //0x11D9
    char pad_11DA[62]; //0x11DA
    bool Being_Carried; //0x1218
    char pad_1219[120]; //0x1219
    bool arrest_complete; //0x1291
    char pad_1292[806]; //0x1292
    bool reported_complete; //0x15B8
    char pad_15B9[4231]; //0x15B9
    class TakenHostageBy_Data* PtrToTakenHostageBy; //0x2640
    char pad_2648[2624]; //0x2648
}; //Size: 0x3088

class Pos_data
{
public:
    char pad_0000[296]; //0x0000
    fvector Pos; //0x0128
    char pad_0134[88]; //0x0134
}; //Size: 0x018C

class Player_Data
{
public:
    char pad_0000[816]; //0x0000
    class Name_Data* PtrToName; //0x0330
    char pad_0338[336]; //0x0338
}; //Size: 0x0488

class Name_Data
{
public:
    wchar_t Name[22]; //0x0000
    char pad_002C[128]; //0x002C
}; //Size: 0x00AC

class Mesh_Data
{
public:
    char pad_0000[1544]; //0x0000
    class Skeleton_Data* PtrToSkeleton; //0x0608
    char pad_0610[8]; //0x0610
    class Skeleton_Data1* PtrToSkeleton1; //0x0618
    char pad_0620[616]; //0x0620
}; //Size: 0x0888

class Skeleton_Data
{
public:
    char pad_0000[136]; //0x0000
}; //Size: 0x0088

class Skeleton_Data1
{
public:
    char pad_0000[136]; //0x0000
}; //Size: 0x0088

class Health_Data
{
public:
    char pad_0000[216]; //0x0000
    float Health; //0x00D8
    float Max_Health; //0x00DC
    char pad_00E0[136]; //0x00E0
    float RightLegHealth; //0x0168
    float RightLegMaxHealth; //0x016C
    char pad_0170[44]; //0x0170
    float LeftLegHealth; //0x019C
    float LeftLegMaxHealth; //0x01A0
    char pad_01A4[44]; //0x01A4
    float RightArmHealth; //0x01D0
    float RightArmMaxHealth; //0x01D4
    char pad_01D8[44]; //0x01D8
    float LeftArmHealth; //0x0204
    float LeftArmMaxHealth; //0x0208
    char pad_020C[44]; //0x020C
    float HeadHealth; //0x0238
    float HeadMaxHealth; //0x023C
    char pad_0240[592]; //0x0240
}; //Size: 0x0490

class Weapon_Data
{
public:
    char pad_0000[472]; //0x0000
    class LastWeapon_Data* PtrToLastWeapon; //0x01D8
    char pad_01E0[1704]; //0x01E0
}; //Size: 0x0888

class LastWeapon_Data
{
public:
    char pad_0000[728]; //0x0000
    class WeaponName_Data* PtrToWeaponName; //0x02D8
    char pad_02E0[4904]; //0x02E0
    class Magazine_Data* PtrToMagazine; //0x1608
    char pad_1610[1656]; //0x1610
}; //Size: 0x1C88

class WeaponName_Data
{
public:
    char pad_0000[40]; //0x0000
    class WeaponName_Data1* PtrToWeaponName1; //0x0028
    char pad_0030[88]; //0x0030
}; //Size: 0x0088

class WeaponName_Data1
{
public:
    wchar_t WeaponName[8]; //0x0000
    char pad_0010[128]; //0x0010
}; //Size: 0x0090

class Magazine_Data
{
public:
    uint16_t Ammo; //0x0000
    uint16_t Ammo_Type; //0x0002
    char pad_0004[132]; //0x0004
}; //Size: 0x0088

class ArrestedBy_Data
{
public:
    char pad_0000[136]; //0x0000
}; //Size: 0x0088

class KilledBy_Data
{
public:
    char pad_0000[136]; //0x0000
}; //Size: 0x0088

class IncapacitatedBy_Data
{
public:
    char pad_0000[136]; //0x0000
}; //Size: 0x0088

class TakenHostageBy_Data
{
public:
    char pad_0000[136]; //0x0000
}; //Size: 0x0088

extern Actors* Actor[255];
extern Actors* ActorsAddr;


//__declspec(naked) void GetActors() {
//	__asm {
//		movups xmm0, xmmword ptr ds : [rdx + 0x128]
//		movups xmmword ptr ds : [rbx + 0x80] , xmm0
//		mov[ActorsAddr], rdx
//		jmp[jmpback]
//	}
//}

// Function declarations - implementations are in hooks.cpp
void VEH_GetActors_Wrapper(PCONTEXT ctx);
LONG WINAPI GetActors_VEHHandler(PEXCEPTION_POINTERS ExceptionInfo);

struct Magazine {
	uint16_t Ammo;
	uint16_t Ammo_Type;
};