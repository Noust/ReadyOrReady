#pragma once
#include "include.h"
bool success = false;
bool spaceKeyPressed = false;
bool defaultv = false;
bool defaultf = false;
char distStr[32];
char HealthStr[32];
float LHealth;
float fire_rate = 0;

class cheats {
public:
	void init();
	void unload();
};
cheats* c;

void loop();