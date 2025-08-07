#include "include.h"

void cheats::init() {
	Actors_Hook = GetAddr(0x2EE2CEF);
	jmpback = Actors_Hook + 14;
	P(GetAddr(0x2996840), "\x90\x90\x90\x90\x90\x90\x90\x90", 8);
	VEH_GETACTORS(Actors_Hook);
}

void cheats::unload() {
	VEH_CLEANUP();
	
	P(GetAddr(0x2996840), "\xF3\x0F\x11\x89\xA0\x02\x00\x00", 8);
}


void loop() {
	if (USettings.esp || USettings.fov_changer || USettings.fast_run || USettings.super_run || USettings.jump || USettings.GodMode || USettings.Unlimited_MeleeRange || USettings.GodMode_LastState || USettings.High_FireRate_LastState || USettings.No_Reload_LastState || USettings.Unlimited_MeleeRange_LastState || USettings.Show_Magazines || !defaultv) {
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