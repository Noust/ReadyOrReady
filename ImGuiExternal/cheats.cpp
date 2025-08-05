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

constexpr DWORD CACHE_TTL_MS = 100;        // 100ms para datos estáticos (salud, arresto)
constexpr DWORD POSITION_TTL_MS = 16;      // 16ms para posición (~60 FPS)
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
			Actor[i] = nullptr; // Limpiar puntero inválido
			InvalidateCacheEntry(i); // También invalidar entrada de caché correspondiente
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
		outPosition = {0, 0, 0};
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

ValidatedActorData SafeGetActorCompleteData(Actors* actor) {
	ValidatedActorData result;
	
	if (!actor) return result;
	
	// Buscar en caché primero
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
	
	// Actualizar entrada de caché
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

void cheats::init() {
	Actors_Hook = GetAddr(0x2EE2CEF);
	jmpback = Actors_Hook + 14;
	P(GetAddr(0x2996840), "\x90\x90\x90\x90\x90\x90\x90\x90", 8);
	H(Actors_Hook, GetActors, 14);
}

void cheats::unload() {
	P(GetAddr(0x2996840), "\xF3\x0F\x11\x89\xA0\x02\x00\x00", 8);
	P(Actors_Hook, "\x0F\x10\x82\x28\x01\x00\x00\x0F\x11\x83\x80\x00\x00\x00", 14);
}


void loop() {
	if (USettings.esp || USettings.fov_changer || USettings.fast_run || USettings.super_run || USettings.jump || !defaultv) {
		if (ReadValues())
			success = true;
		else
			success = false;
	}
	else {
		success = false;
	}

	if (USettings.jump && success) {
		bool isSpaceCurrentlyPressed = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
		
		if (isSpaceCurrentlyPressed && !spaceKeyPressed) {
			setBooleanBit(adresses.acknowledgedPawn, 0x46C, 2, true);
			spaceKeyPressed = true;
		}
		else if (!isSpaceCurrentlyPressed && spaceKeyPressed) {
			spaceKeyPressed = false;
		}
	}

	if (!USettings.fast_run && !USettings.super_run) {
		if (success && !defaultv) {
			write<float>(adresses.acknowledgedPawn + 0x280C, 320);
			write<float>(adresses.acknowledgedPawn + 0x2810, 500);
			defaultv = true;
		}
	}

	ManageActors();
}