#pragma once
#include "include.h"
bool success = false;
bool spaceKeyPressed = false;
bool defaultv = false;
bool defaultf = false;
float fire_rate = 0;
bool defaulta = false;
uint16_t c_ammo = 0;
char distStr[32];
char HealthStr[32];
float LHealth;

class cheats {
public:
	void init();
	void unload();
};
cheats* c;

void AimBot(fvector AimPos);

void loop();

void aimbot(int index);

int FindClosestEnemy();