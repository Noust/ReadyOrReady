#pragma once
#include "include.h"

extern bool success;
extern bool spaceKeyPressed;
extern bool defaultf;
extern float saved_fire_rate;
extern bool defaulta;
extern uint16_t c_ammo;
extern char distStr[32];
extern char HealthStr[32];
extern float LHealth;

class cheats {
public:
	void init();
	void unload();
};

extern cheats* c;

void AimBot(fvector AimPos);

void loop();

void aimbot(int index);

int FindClosestEnemy();