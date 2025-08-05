#pragma once
#include "include.h"

bool isInitialized = false;
bool isMenuVisible = true;
char HealthInfo[64];

HWND overlayWindow;

IDirect3DDevice9Ex* pDevice = nullptr;
IDirect3D9Ex* pDirect = nullptr;
D3DPRESENT_PARAMETERS gD3DPresentParams = { NULL };
IDirect3DTexture9* pTexture = nullptr;

void drawItem() {
	char fpsInfo[64];
	RGBA textColor = { 255,255,255,255 };
	snprintf(fpsInfo, sizeof(fpsInfo), "Overlay FPS: %.0f", ImGui::GetIO().Framerate);
	drawStrokeText(30, 44, &textColor, fpsInfo);
}

void renderEsp() {
	if (USettings.esp) {
		if (success) {
			float LHealth;
			if (read<float>(adresses.characterHealth + 0xD8, LHealth)) {
				if (LHealth > 1) {
					int numaiactors;
					if (read<int>(adresses.gameState + (offset::allai_actors + sizeof(uintptr_t)), numaiactors)) {
						for (int i = 0; i < numaiactors; i++) {
							DWORD64 currentActor;
							if (!read<DWORD64>(adresses.allAiActors + (sizeof(uintptr_t) * i), currentActor)) continue;

							DWORD64 actorHealth;
							if (!read<DWORD64>(currentActor + offset::character_health, actorHealth)) continue;

							float halthvalue;
							if (!read<float>(actorHealth + 0xD8, halthvalue)) continue;
							if (halthvalue < 1) continue;

							DWORD64 rootComponent;
							if (!read<DWORD64>(currentActor + offset::root_component, rootComponent)) continue;

							fvector actorLocation = *(fvector*)(rootComponent + 0x128);
							actorLocation.z -= 80;

							fvector2d screenlocation = w2s(actorLocation);
							if (screenlocation.x < 0 || screenlocation.x > widthscreen || screenlocation.y < 0 || screenlocation.y > heightscreen) continue;

							sprintf_s(HealthInfo, "Health: %.0f", halthvalue);

							DrawT({ screenlocation.x, screenlocation.y - 20 }, HealthInfo, 1, { 255, 255, 255, 255 });
							DrawLine({ widthscreen / 2, heightscreen }, { screenlocation.x, screenlocation.y }, { 255, 0, 0, 255 }, 0, true);
						}
					}
				}
			}
		}
	}
}

void renderMenu() {
	if (USettings.Backround_Animation)
		DrawBackgroundAnimation();
	inputHandler();
	if (USettings.Show_Fps)
		drawItem();

	// Animate menu
	static animator Animator = { 255, false, 0.5f };
	animatecontent(Animator);

	// Main window styling
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.11f, 0.11f, 0.11f, 1.00f));

	ImGui::Begin("CS Menu", nullptr, ImGuiWindowFlags_NoCollapse);
	ImVec2 windowpos = ImGui::GetWindowPos();
	ImVec2 windowsize = ImGui::GetWindowSize();

	// Navigation bar
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
	ImGui::BeginChild("##Navigation", ImVec2(ImGui::GetContentRegionAvail().x, 50), true);
	ImVec2 child_windowpos = ImGui::GetWindowPos();
	ImVec2 child_windowsize = ImGui::GetWindowSize();

	// Navigation buttons
	float windowWidth = ImGui::GetWindowSize().x;
	float buttonWidth = (ImGui::GetContentRegionAvail().x - 30) / 4;
	float spacing = (windowWidth - (buttonWidth * 4)) / 5;

	const char* tabs[] = { "Gun", "Legit", "Visuals", "Config" };
	int tabValues[] = { 1, 2, 3, 4 };

	for (int i = 0; i < 4; i++) {
		ImGui::SetCursorPosX(spacing * (i + 1) + buttonWidth * i);

		bool selected = (CSettings::MenuWindow == tabValues[i]);
		if (selected) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));

		if (ImGui::Button(tabs[i], ImVec2(buttonWidth, 0))) {
			CSettings::MenuWindow = tabValues[i];
		}

		if (selected) ImGui::PopStyleColor();

		if (i < 3) ImGui::SameLine();
	}

	ImGui::EndChild();
	ImGui::PopStyleVar();

	// Content area
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
	ImGui::BeginChild("##Content", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
	ImVec2 child_windowpos1 = ImGui::GetWindowPos();
	ImVec2 child_windowsize1 = ImGui::GetWindowSize();

	// Main menu / Welcome screen
	if (CSettings::MenuWindow == 0) {
		ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x / 2) - 100);
		ImGui::Image((void*)pTexture, ImVec2(200, 170));
		centertext<float>("CSGO 2 Menu by Nova", 0.0f, 0.0f);
		centertext<float>("Window Size X:%0.f Y:%0.f", windowsize.x, windowsize.y);
	}

	// Gun menu
	else if (CSettings::MenuWindow == 1) {
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));

		ImGui::Text("Combat Settings");

		ImGui::PopStyleColor();
	}

	else if (CSettings::MenuWindow == 2) {
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));

		// Movement & Utility Section
		ImGui::Text("Movement & Utility");
		ImGui::Checkbox("Fast run", &USettings.fast_run);
		if (USettings.fast_run && success) {
			USettings.super_run = false;
			write<float>(adresses.acknowledgedPawn + 0x280C, 1000);
			write<float>(adresses.acknowledgedPawn + 0x2810, 1000);
			defaultv = false;
		}
		ImGui::Checkbox("Super run", &USettings.super_run);
		if (USettings.super_run && success) {
			USettings.fast_run = false;
			write<float>(adresses.acknowledgedPawn + 0x280C, 2000);
			write<float>(adresses.acknowledgedPawn + 0x2810, 2000);
			defaultv = false;
		}
		ImGui::Checkbox("Able to jump", &USettings.jump);
		ImGui::PopStyleColor();
	}
	else if (CSettings::MenuWindow == 3) {
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));

		// View Settings Section
		ImGui::Text("View Settings Section");
		ImGui::Checkbox("Enable ESP", &USettings.esp);
		ImGui::Checkbox("Fov Changer", &USettings.fov_changer);
		if (USettings.fov_changer) {
			ImGui::SliderFloat("Fov Value", &USettings.fov_value, 40.0f, 170.0f, "%.0f");
			ImGui::SliderFloat("Base Fov Value", &USettings.base_fov_value, 90.0f, 170.0f, "%.0f");
			if (success) {
				write<float>(adresses.cameraComponent + 0x2A0, USettings.fov_value);
				write<float>(adresses.acknowledgedPawn + 0x22E4, USettings.base_fov_value);
			}
		}

		//ImGui::Checkbox("Enable Radar", &USettings.radar_hack);
		//if (USettings.radar_hack) {
		//	ImGui::Text("Radar Settings");
		//	ImGui::SliderFloat("Radar Scale", &USettings.scale, 0.05f, 0.5f, "%.2f");
		//	ImGui::SliderFloat("Radar Radius", &USettings.radarRadius, 10.0f, 200.0f, "%.0f px");

		//	ImGui::Text("Radar Position");
		//	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.45f);
		//	ImGui::SliderFloat("X Position", &USettings.center.x, 0, X_Screen, "%.0f");
		//	ImGui::SameLine();
		//	ImGui::SliderFloat("Y Position", &USettings.center.y, 0, Y_Screen, "%.0f");
		//	ImGui::PopItemWidth();

		//	ImGui::Separator();
		//}
		//ImGui::Checkbox("Night Mode", &USettings.Night_Mode);
		//ImGui::Checkbox("Full Bright", &USettings.FullBright_Mode);
		//ImGui::Checkbox("Eye Ray", &USettings.ShowEyeRay);
		//if (USettings.ShowEyeRay) {
		//	ImGui::SliderFloat("Eye Ray Length", &USettings.length, 5, 100, "%.0f");
		//	ImGui::ColorEdit3("Eye Ray Color", (float*)&USettings.EyeRay_Color);
		//	ImGui::SliderInt("Eye Ray Thickness", &USettings.EyeRay_Thickness, 0, 10);
		//}

		//ImGui::Spacing();
		//ImGui::Separator();
		//ImGui::Spacing();

		//// ESP Features Section
		//ImGui::Text("ESP Features");

		//// Snap Line ESP
		//ImGui::Checkbox("Snap Lines", &USettings.SnaplLine_Esp);
		//if (USettings.SnaplLine_Esp) {
		//	ImGui::ColorEdit3("Enemy Line Color", (float*)&USettings.Enemy_SnaplLine_Esp_Color);
		//	ImGui::ColorEdit3("Team Line Color", (float*)&USettings.Squad_SnaplLine_Esp_Color);
		//	ImGui::SliderInt("Line Thickness", &USettings.SnaplLine_Esp_Thickness, 0, 10);

		//	ImGui::Text("Line Start Position");
		//	if (ImGui::BeginTable("LinePositions", 2)) {
		//		ImGui::TableNextColumn();
		//		if (ImGui::Button("Bottom")) USettings.SnaplLine_Esp_Start_Point = { X_Screen / 2, Y_Screen };
		//		if (ImGui::Button("Left")) USettings.SnaplLine_Esp_Start_Point = { 0, Y_Screen / 2 };
		//		ImGui::TableNextColumn();
		//		if (ImGui::Button("Top")) USettings.SnaplLine_Esp_Start_Point = { X_Screen / 2, 0 };
		//		if (ImGui::Button("Right")) USettings.SnaplLine_Esp_Start_Point = { X_Screen, Y_Screen / 2 };
		//		ImGui::EndTable();
		//	}

		//	ImGui::SliderFloat("Custom X", &USettings.SnaplLine_Esp_Start_Point.x, 0, X_Screen);
		//	ImGui::SliderFloat("Custom Y", &USettings.SnaplLine_Esp_Start_Point.y, 0, Y_Screen);

		//	ImGui::Text("Line End Point");
		//	if (ImGui::Button(USettings.SnaplLine_Esp_End_Point ? "Head" : "Feet")) {
		//		USettings.SnaplLine_Esp_End_Point = !USettings.SnaplLine_Esp_End_Point;
		//	}
		//	ImGui::Separator();
		//}

		//// Box ESP Options
		//ImGui::Checkbox("Box ESP", &USettings.Box_ESP);
		//if (USettings.Box_ESP) {
		//	ImGui::ColorEdit3("Enemy Box Color", (float*)&USettings.Enemy_Box_Esp_Color);
		//	ImGui::ColorEdit3("Team Box Color", (float*)&USettings.Squad_Box_Esp_Color);
		//	ImGui::SliderInt("Box Thickness", &USettings.Box_Esp_Thickness, 0, 10);
		//	ImGui::Separator();
		//}

		//ImGui::Checkbox("Corner Box", &USettings.CornerBox_ESP);
		//if (USettings.CornerBox_ESP) {
		//	ImGui::ColorEdit3("Enemy Corner Color", (float*)&USettings.Enemy_CornerBox_Esp_Color);
		//	ImGui::ColorEdit3("Team Corner Color", (float*)&USettings.Squad_CornerBox_Esp_Color);
		//	ImGui::SliderInt("Corner Thickness", &USettings.Box_CornerEsp_Thickness, 0, 10);
		//	ImGui::Separator();
		//}

		//ImGui::Checkbox("3D Box", &USettings.Box3D_Esp);
		//if (USettings.Box3D_Esp) {
		//	ImGui::ColorEdit3("Enemy 3D Box Color", (float*)&USettings.Enemy_Box3D_Esp_Color);
		//	ImGui::ColorEdit3("Team 3D Box Color", (float*)&USettings.Squad_Box3D_Esp_Color);
		//	ImGui::SliderInt("3D Box Thickness", &USettings.Box3D_Esp_Thickness, 0, 10);
		//	ImGui::SliderFloat("3D Box Width", &USettings.Box3D_Width, 10, 40);
		//	ImGui::Separator();
		//}

		//ImGui::Checkbox("Filled Box", &USettings.FilledBox_Esp);
		//if (USettings.FilledBox_Esp) {
		//	ImGui::ColorEdit4("Enemy Fill Color", (float*)&USettings.Enemy_FilledBox_Esp_Color);
		//	ImGui::ColorEdit4("Team Fill Color", (float*)&USettings.Squad_FilledBox_Esp_Color);
		//	ImGui::Separator();
		//}

		//// Player Info ESP
		//ImGui::Checkbox("Show Names", &USettings.Name_ESP);
		//if (USettings.Name_ESP) {
		//	ImGui::ColorEdit3("Enemy Name Color", (float*)&USettings.Enemy_Name_ESP_Color);
		//	ImGui::ColorEdit3("Team Name Color", (float*)&USettings.Squad_Name_ESP_Color);
		//	ImGui::Separator();
		//}

		//ImGui::Checkbox("Show Distance", &USettings.Distance_Esp);
		//if (USettings.Distance_Esp) {
		//	ImGui::ColorEdit3("Enemy Distance Color", (float*)&USettings.Enemy_Distance_Esp_Color);
		//	ImGui::ColorEdit3("Team Distance Color", (float*)&USettings.Squad_Distance_Esp_Color);
		//	ImGui::Separator();
		//}

		//ImGui::Checkbox("Show Weapon", &USettings.GunName_Esp);
		//if (USettings.GunName_Esp) {
		//	ImGui::ColorEdit3("Enemy Weapon Color", (float*)&USettings.Enemy_GunName_Color);
		//	ImGui::ColorEdit3("Team Weapon Color", (float*)&USettings.Squad_GunName_Color);
		//	ImGui::Separator();
		//}

		//ImGui::Checkbox("Show Bones", &USettings.Bone_Esp);
		//if (USettings.Bone_Esp) {
		//	ImGui::ColorEdit3("Enemy Bone Color", (float*)&USettings.Enemy_Bone_Esp_Color);
		//	ImGui::ColorEdit3("Team Bone Color", (float*)&USettings.Squad_Bone_Esp_Color);
		//	ImGui::SliderInt("Bone Thickness", &USettings.Bone_Esp_Thickness, 0, 10);
		//	ImGui::Separator();
		//}

		//ImGui::Checkbox("Health Bar", &USettings.HealthBar_ESP);
		//ImGui::Checkbox("Armor Bar", &USettings.ArmorBar_ESP);

		//ImGui::Spacing();
		//ImGui::Separator();
		//ImGui::Spacing();

		//// Crosshair Settings
		//ImGui::Text("Crosshair Settings");
		//ImGui::Checkbox("Custom Crosshair", &USettings.DrawCrosshair);
		//if (USettings.DrawCrosshair) {
		//	ImGui::Checkbox("Show When Not Aiming", &USettings.whennotaiming);
		//	ImGui::ColorEdit3("Crosshair Color", (float*)&USettings.Crosshair_Color);
		//	ImGui::SliderFloat("Size", &USettings.Crosshair_size, 1, 30);
		//	ImGui::SliderInt("Thickness", &USettings.Crosshair_thickness, 0, 10);
		//	ImGui::Checkbox("Circle", &USettings.circle);
		//	ImGui::Checkbox("Cross", &USettings.Cross);
		//}

		//ImGui::Spacing();
		//ImGui::Separator();
		//ImGui::Spacing();

		//// Global ESP Settings
		//ImGui::Text("Global ESP Settings");
		//ImGui::SliderInt("Max ESP Distance", &USettings.ESP_Distance, 0, 100);
		//ImGui::SliderFloat("Text Size", &USettings.Text_Size, 0.4f, 2.0f, "%.2f");

		ImGui::PopStyleColor();
	}
	else if (CSettings::MenuWindow == 4) {
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));

		// UI Settings
		ImGui::Text("Menu Animations");
		ImGui::Checkbox("Window Animation", &USettings.window_animation);
		ImGui::Checkbox("Navigation Animation", &USettings.navigationwindow_animation);
		ImGui::Checkbox("Options Animation", &USettings.optionswindow_animation);
		ImGui::Checkbox("Background Animation", &USettings.Backround_Animation);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Menu Controls");
		ImGui::Text("Press INSERT to toggle menu");
		ImGui::Text("Press DELETE to exit");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Bypass");
		ImGui::Checkbox("OBS Bypass", &USettings.OBSBypass);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Save/Load Settings
		ImGui::Text("Configuration");
		ImGui::Checkbox("Show Overlay Fps", &USettings.Show_Fps);
		if (ImGui::Button("Save Settings")) {
			F->SaveSettings();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load Settings")) {
			if (std::filesystem::exists("sarilla.bin"))
				F->ReadSettings();
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// ESP Filters
		ImGui::Text("ESP Filters");
		ImGui::Checkbox("Show Teammates", &USettings.Show_Squad);
		ImGui::Checkbox("Show Enemies", &USettings.Show_Enemy);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Quick Actions
		ImGui::Text("Quick Actions");
		if (ImGui::Button("Disable All Features")) {

		}

		ImGui::SameLine();
		if (ImGui::Button("Reset to Default")) {

		}
		ImGui::PopStyleColor();
	}
	ImGui::EndChild();
	ImGui::PopStyleVar();

	// Window animations
	if (USettings.window_animation)
		ImGui::GetBackgroundDrawList()->AddRect(
			ImVec2(windowpos.x - 1, windowpos.y - 1),
			ImVec2(windowpos.x + windowsize.x + 1, windowpos.y + windowsize.y + 1),
			ImColor(255, 255, 255, (int)Animator.alpha), 12.0f
		);

	if (USettings.navigationwindow_animation)
		ImGui::GetForegroundDrawList()->AddRect(
			ImVec2(child_windowpos.x - 1, child_windowpos.y - 1),
			ImVec2(child_windowpos.x + child_windowsize.x + 1, child_windowpos.y + child_windowsize.y + 1),
			ImColor(255, 255, 255, (int)Animator.alpha), 12.0f
		);

	if (USettings.optionswindow_animation)
		ImGui::GetForegroundDrawList()->AddRect(
			ImVec2(child_windowpos1.x - 1, child_windowpos1.y - 1),
			ImVec2(child_windowpos1.x + child_windowsize1.x + 1, child_windowpos1.y + child_windowsize1.y + 1),
			ImColor(255, 255, 255, (int)Animator.alpha), 12.0f
		);

	if (USettings.show_watermark) {
		ImVec2 screenSize = ImGui::GetIO().DisplaySize;
		ImVec2 imageSize(128, 128); // Adjust size as needed
		ImVec2 pos(screenSize.x - imageSize.x - 10, 5); // 10px padding from edges
		ImGui::GetBackgroundDrawList()->AddImage((void*)pTexture, pos, ImVec2(pos.x + imageSize.x, pos.y + imageSize.y));
	}

	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor();
	ImGui::End();
	SetFocus(overlayWindow);
}

void renderImGui() {
	if (!isInitialized) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		D3DXCreateTextureFromFileInMemory(pDevice, NOVA_LOGO, sizeof(NOVA_LOGO), &pTexture);

		ImGui_ImplWin32_Init(overlayWindow);
		ImGui_ImplDX9_Init(pDevice);
		ImGui_ImplDX9_CreateDeviceObjects();
		isInitialized = true;
	}

	if (GetAsyncKeyState(VK_INSERT) & 1) {
		isMenuVisible = !isMenuVisible;
		ImGui::GetIO().MouseDrawCursor = isMenuVisible;
	}
	Colors();
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	SetWindowLong(overlayWindow, GWL_EXSTYLE, isMenuVisible ? WS_EX_TOOLWINDOW : (WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW));
	UpdateWindow(overlayWindow);
	if (isMenuVisible) {
		renderMenu();
	}
	renderEsp();
	ImGui::EndFrame();

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (pDevice->BeginScene() >= 0) {
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		pDevice->EndScene();
	}

	if (pDevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST && pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
		ImGui_ImplDX9_InvalidateDeviceObjects();
		pDevice->Reset(&gD3DPresentParams);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}