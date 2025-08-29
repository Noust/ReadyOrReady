#pragma once

#include <Windows.h>
#include <iostream>
#include <cstdio>
#include <TlHelp32.h>
#include <vector>
#include <thread>
#include <dwmapi.h>
#include <fstream>
#include <filesystem>
#include <Psapi.h>
#include <string>
#include <cctype>
#include <cmath>

#include <DirectX/d3d9.h>
#include <DirectX/d3dx9math.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx9.h>
#include <ImGui/imgui_impl_win32.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")

DWORD64 Uworld;

float widthscreen;
float heightscreen;

#include "image.h"
#include "vector.h"
#include "UserSettings.h"
#include "FileManager.h"
#include "WorldToScreen.hpp"

#define P(Addr,bytes,size) StealthPatch((BYTE*)Addr,(BYTE*)bytes,size)
#define VEH_GETACTORS(Addr) VEH_GetActors_Hook((BYTE*)Addr)
//#define VEH_CLEANUP() VEH_GetActors_Cleanup()
#define GetAddr(Addr) (DWORD64)GetModuleHandleA("ReadyOrNotSteam-Win64-Shipping.exe") + Addr

#include "HookFunc.hpp"
#include "hooks.h"
#define aactor Actor[i]
#include "reader.hpp"
#include "Actor_Manager.h"
#include "Overlay.hpp"
#include "cheats.h"
#include "render.hpp"