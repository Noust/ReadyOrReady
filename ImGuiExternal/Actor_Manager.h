#pragma once
#include "include.h"
class Actor_Data;
class IncapacitatedBy_Data;
class KilledBy_Data;
class ArrestedBy_Data;
class TakenHostageBy_Data;
class Skeleton_Data;
class Skeleton_Data1;

struct ValidatedActorData {
	bool isValid;
	float health;
	float maxHealth;
	fvector position;
	Actor_Data* ptrToActor;
	bool isSquad;
	bool isCivilian;
	bool isEnemy;
	
	bool beingCarried;
	float meleeDamage;
	float meleeRange;

	bool reportedComplete;

	bool surrenderComplete;
	bool arrestComplete;
	bool ActorIncapacitated;
	bool ActorTakenHostage;
	bool ActorKilled;
	
	float headHealth;
	float headMaxHealth;
	float leftArmHealth;
	float leftArmMaxHealth;
	float leftLegHealth;
	float leftLegMaxHealth;
	float rightArmHealth;
	float rightArmMaxHealth;
	float rightLegHealth;
	float rightLegMaxHealth;
	
	wchar_t playerName[22];
	uint16_t weaponAmmo;
	uint16_t weaponAmmoType;
	wchar_t weaponName[8];
	
	IncapacitatedBy_Data* ptrToIncapacitatedBy;
	KilledBy_Data* ptrToKilledBy;
	ArrestedBy_Data* ptrToArrestedBy;
	TakenHostageBy_Data* ptrToTakenHostageBy;
	Skeleton_Data* ptrToSkeleton;
	Skeleton_Data1* ptrToSkeleton1;

	ValidatedActorData() : isValid(false), health(0.0f), maxHealth(0.0f), position({ 0,0,0 }), arrestComplete(false), ptrToActor(nullptr), ActorIncapacitated(false), ActorTakenHostage(false), ActorKilled(false), isSquad(false), isCivilian(false), isEnemy(false),
		beingCarried(false), meleeDamage(0.0f), meleeRange(0.0f), reportedComplete(false), surrenderComplete(false),
		headHealth(0.0f), headMaxHealth(0.0f), leftArmHealth(0.0f), leftArmMaxHealth(0.0f), leftLegHealth(0.0f), leftLegMaxHealth(0.0f),
		rightArmHealth(0.0f), rightArmMaxHealth(0.0f), rightLegHealth(0.0f), rightLegMaxHealth(0.0f),
		weaponAmmo(0), weaponAmmoType(0), ptrToIncapacitatedBy(nullptr), ptrToKilledBy(nullptr), ptrToArrestedBy(nullptr),
		ptrToTakenHostageBy(nullptr), ptrToSkeleton(nullptr), ptrToSkeleton1(nullptr) {
		memset(playerName, 0, sizeof(playerName));
		memset(weaponName, 0, sizeof(weaponName));
	}
};

bool IsValidActorPointer(Actors* actorPtr);
void CleanupInvalidActors();
void ManageActors();
bool SafeValidateActorData(Actors* actor, Actor_Data*& outActorData);
bool SafeGetActorHealth(Actor_Data* actorData, float& outHealth);
bool SafeGetActorHealthData(Actor_Data* actorData, float& outHealth, float& outMaxHealth);
bool SafeGetActorPosition(Actor_Data* actorData, fvector& outPosition);
bool SafeGetActorArrestStatus(Actor_Data* actorData, bool& outArrestComplete);
bool SafeGetActorStateData(Actor_Data* actorData, bool& outBeingCarried, float& outMeleeDamage, float& outMeleeRange, bool& outReportedComplete, bool& outSurrenderComplete);
bool SafeGetActorDetailedHealthData(Actor_Data* actorData, float& outHeadHealth, float& outHeadMaxHealth, float& outLeftArmHealth, float& outLeftArmMaxHealth, 
	float& outLeftLegHealth, float& outLeftLegMaxHealth, float& outRightArmHealth, float& outRightArmMaxHealth, float& outRightLegHealth, float& outRightLegMaxHealth);
bool SafeGetActorPlayerName(Actor_Data* actorData, wchar_t outPlayerName[22]);
bool SafeGetActorWeaponData(Actor_Data* actorData, uint16_t& outAmmo, uint16_t& outAmmoType, wchar_t outWeaponName[8]);
ValidatedActorData SafeGetActorCompleteData(Actors* actor);
bool FastGetActorPosition(Actors* actor, fvector& outPosition);

// Safe write functions
bool SafeSetActorHealth(Actor_Data* actorData, float newHealth);
bool SafeSetActorMaxHealth(Actor_Data* actorData, float newMaxHealth);
bool SafeSetActorArrestStatus(Actor_Data* actorData, bool newArrestComplete);
bool SafeSetActorSurrenderStatus(Actor_Data* actorData, bool newSurrenderComplete);
bool SafeModifyActorData(Actors* actor, float newHealth, bool newArrestComplete, bool newSurrenderComplete);

void UpdateActorCache(Actors* actor, bool isValid, const ValidatedActorData& data);
void UpdateActorCache(Actors* actor, bool isValid, const ValidatedActorData& data, DWORD currentTime);
void InvalidateCacheEntry(size_t index);
void InvalidateAllCache();

bool shouldloop(ValidatedActorData actorData);