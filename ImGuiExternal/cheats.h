#pragma once
#include "include.h"
bool success = false;
bool spaceKeyPressed = false;
bool defaultv = false;

class Actors;
class Actor_Data;

struct ValidatedActorData {
	bool isValid;
	float health;
	float maxHealth;
	fvector position;
	bool arrestComplete;
	
	ValidatedActorData() : isValid(false), health(0.0f), maxHealth(0.0f), position({0,0,0}), arrestComplete(false) {}
};

class cheats {
public:
	void init();
	void unload();
};
cheats* c;

void loop();

bool IsValidActorPointer(Actors* actorPtr);
void CleanupInvalidActors();
void ManageActors();
bool SafeValidateActorData(Actors* actor, Actor_Data*& outActorData);
bool SafeGetActorHealth(Actor_Data* actorData, float& outHealth);
bool SafeGetActorHealthData(Actor_Data* actorData, float& outHealth, float& outMaxHealth);
bool SafeGetActorPosition(Actor_Data* actorData, fvector& outPosition);
bool SafeGetActorArrestStatus(Actor_Data* actorData, bool& outArrestComplete);
ValidatedActorData SafeGetActorCompleteData(Actors* actor);
bool FastGetActorPosition(Actors* actor, fvector& outPosition);

void UpdateActorCache(Actors* actor, bool isValid, const ValidatedActorData& data);
void UpdateActorCache(Actors* actor, bool isValid, const ValidatedActorData& data, DWORD currentTime);
void InvalidateCacheEntry(size_t index);
void InvalidateAllCache();