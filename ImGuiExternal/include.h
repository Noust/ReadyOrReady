#pragma once

#include <Windows.h>
#include <iostream>
#include <cstdio>
#include <TlHelp32.h>
#include <vector>
#include <dwmapi.h>
#include <fstream>
#include <filesystem>
#include <Psapi.h>
#include <string>

#include <DirectX/d3d9.h>
#include <DirectX/d3dx9math.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx9.h>
#include <ImGui/imgui_impl_win32.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")

DWORD64 Uworld;

double widthscreen;
double heightscreen;

#include "image.h"
#include "vector.h"
#include "UserSettings.h"
#include "FileManager.h"
#include "Overlay.hpp"
#include "WorldToScreen.hpp"

#define P(Addr,bytes,size) Patch((BYTE*)Addr,(BYTE*)bytes,size)
#define H(Addr,hook,size) Hook((BYTE*)Addr,(BYTE*)hook,size)
#define GetAddr(Addr) (DWORD64)GetModuleHandleA("ReadyOrNotSteam-Win64-Shipping.exe") + Addr

#include "HookFunc.hpp"
#include "hooks.h"
#define aactor Actor[i]
#include "reader.hpp"
#include "cheats.h"
#include "render.hpp"