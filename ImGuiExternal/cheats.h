#pragma once
#include "include.h"
bool success = false;
bool spaceKeyPressed = false;
bool defaultv = false;
char distStr[32];

class cheats {
public:
	void init();
	void unload();
};
cheats* c;

void loop();