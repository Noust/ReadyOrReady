#include "include.h"

void cheats::init() {
	Actors_Hook = GetAddr(offset::actors_hook);
	jmpback = Actors_Hook + 14;
	P(GetAddr(offset::fov_hook), "\x90\x90\x90\x90\x90\x90\x90\x90", 8);
	VEH_GETACTORS(Actors_Hook);
}

void cheats::unload() {
	/*VEH_CLEANUP();*/
	
	P(GetAddr(offset::fov_hook), "\xF3\x0F\x11\x89\xA0\x02\x00\x00", 8);
}

void AimBot(fvector AimPos) {
	fvector2d screenPos = w2s(AimPos);

	if (screenPos.x != -1 && screenPos.y != -1) {
		int centerX = widthscreen / 2;
		int centerY = heightscreen / 2;

		double deltaX = screenPos.x - centerX;
		double deltaY = screenPos.y - centerY;

		const double smoothFactor = 1.0f - USettings.Smooth;
		deltaX *= smoothFactor;
		deltaY *= smoothFactor;

		std::this_thread::sleep_for(std::chrono::microseconds(50));
		mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(deltaX), static_cast<DWORD>(deltaY), 0, 0);
	}
}

void loop() {
	if (USettings.esp || USettings.fov_changer || USettings.fast_run || USettings.super_run || USettings.jump || USettings.GodMode || USettings.Unlimited_MeleeRange || USettings.GodMode_LastState || USettings.High_FireRate_LastState || USettings.No_Reload_LastState || USettings.Unlimited_MeleeRange_LastState || USettings.fast_run_LastState || USettings.super_run_LastState || USettings.Show_Magazines || USettings.Show_actors || USettings.Aimbot) {
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
			setBooleanBit(adresses.acknowledgedPawn, offset::jump, 2, true);
			spaceKeyPressed = true;
		}
		else if (!isSpaceCurrentlyPressed && spaceKeyPressed) {
			spaceKeyPressed = false;
		}
	}

	ManageActors();
}

void aimbot(int index) {
	if (!USettings.Aimbot)
		return;

	if (!success)
		return;

	float health;
	read<float>(adresses.characterHealth + offset::health, health);
	if (health < 1)
		return;

	if (GetAsyncKeyState(USettings.HotKey)) {
		int closestEnemyIndex = FindClosestEnemy();

		if (Actor[closestEnemyIndex] == nullptr) return;

		ValidatedActorData actorData = SafeGetActorCompleteData(Actor[closestEnemyIndex]);

		fvector aimPos = actorData.position;

		if (w2s(aimPos).distance({ widthscreen / 2, heightscreen / 2 }) < USettings.AimFov) {
			AimBot(aimPos);
		}
	}
}

int FindClosestEnemy() {
	if (!success) return 1000;
	float LHealth;
	if (!read<float>(adresses.characterHealth + offset::health, LHealth)) return 1000;
	if (LHealth < 1)
		return 1000;

	float closestDistance = FLT_MAX;
	int closestEntity = 1000;
	const fvector2d screenCenter = { widthscreen / 2.0f, heightscreen / 2.0f };

	for (int i = 0; i < 254; i++) {
		if (aactor == nullptr) continue;

		ValidatedActorData actorData = SafeGetActorCompleteData(aactor);

		if (!shouldloop(actorData, true)) continue;

		fvector aimPos = actorData.position;
		fvector2d screenPos = w2s(aimPos);
		float screenDist = screenPos.distance(screenCenter);

		if (screenDist <= USettings.AimFov && screenDist < closestDistance) {
			closestDistance = screenDist;
			closestEntity = i;
		}
	}

	return closestEntity;
}