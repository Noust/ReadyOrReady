#pragma once
#include "include.h"
#include <vector>
#include <locale>
#include <codecvt>

bool isInitialized = false;
bool isMenuVisible = true;

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
		if (read<float>(adresses.characterHealth + offset::health, LHealth)) {
			if (LHealth > 1) {
				for (int i = 0; i < 254; i++) {
					if (aactor == nullptr) continue;

					// OPCIÓN 1: Obtener todos los datos con caché inteligente (recomendado)
					ValidatedActorData actorData = SafeGetActorCompleteData(aactor);

					// OPCIÓN 2: Si solo necesitas posición actualizada frecuentemente:
					// fvector freshPosition;
					// if (FastGetActorPosition(aactor, freshPosition)) {
					//     // Usar freshPosition para movimiento más fluido
					// }

					if (!shouldloop(actorData, false)) continue;

					double distance = POV.Location.distance(actorData.position) / 100;

					fvector actorFeetLocation = actorData.position;
					actorFeetLocation.z -= 85;
					fvector actorHeadLocation = actorData.position;
					actorHeadLocation.z += 85;

					fvector2d feetpos = w2s(actorFeetLocation);
					fvector2d headpos = w2s(actorHeadLocation);
					float height = feetpos.y - headpos.y;

					if (USettings.FilledBox_Esp) {
						if (actorData.isCivilian)
							DrawFilledRect(feetpos, height, height / 4, USettings.Civilian_FilledBox_Esp_Color);
						if (actorData.isEnemy)
							DrawFilledRect(feetpos, height, height / 4, USettings.Enemy_FilledBox_Esp_Color);
						if (actorData.isSquad)
							DrawFilledRect(feetpos, height, height / 4, USettings.Squad_FilledBox_Esp_Color);
					}

					if (USettings.SnaplLine_Esp) {
						fvector2d endPoint = USettings.SnaplLine_Esp_End_Point ? headpos : feetpos;
						if (actorData.isCivilian)
							DrawLine(USettings.SnaplLine_Esp_Start_Point, endPoint, USettings.Civilian_SnaplLine_Esp_Color, USettings.SnaplLine_Esp_Thickness, true);
						if (actorData.isEnemy)
							DrawLine(USettings.SnaplLine_Esp_Start_Point, endPoint, USettings.Enemy_SnaplLine_Esp_Color, USettings.SnaplLine_Esp_Thickness, true);
						if (actorData.isSquad)
							DrawLine(USettings.SnaplLine_Esp_Start_Point, endPoint, USettings.Squad_SnaplLine_Esp_Color, USettings.SnaplLine_Esp_Thickness, true);
					}

					if (USettings.Box_ESP) {
						if (actorData.isCivilian)
							drawbox(feetpos, height, height / 4, USettings.Civilian_Box_Esp_Color, USettings.Box_Esp_Thickness);
						if (actorData.isEnemy)
							drawbox(feetpos, height, height / 4, USettings.Enemy_Box_Esp_Color, USettings.Box_Esp_Thickness);
						if (actorData.isSquad)
							drawbox(feetpos, height, height / 4, USettings.Squad_Box_Esp_Color, USettings.Box_Esp_Thickness);
					}

					if (USettings.HealthBar_ESP && USettings.Show_Health) {
						float healthRatio = actorData.health / actorData.maxHealth;
						drawhealthbar(feetpos, height * healthRatio, height / 3.6f, HealthBarColor(actorData), USettings.HealthBar_Esp_Thickness);
					}

					if (USettings.CornerBox_ESP) {
						if (actorData.isCivilian)
							DrawCornerEsp(height / 2, height, feetpos, USettings.Civilian_CornerBox_Esp_Color, USettings.Box_CornerEsp_Thickness);
						if (actorData.isEnemy)
							DrawCornerEsp(height / 2, height, feetpos, USettings.Enemy_CornerBox_Esp_Color, USettings.Box_CornerEsp_Thickness);
						if (actorData.isSquad)
							DrawCornerEsp(height / 2, height, feetpos, USettings.Squad_CornerBox_Esp_Color, USettings.Box_CornerEsp_Thickness);
					}

					if (USettings.Box3D_Esp) {
						if (actorData.isCivilian)
							Draw3DBox(actorData, USettings.Civilian_Box3D_Esp_Color, USettings.Box3D_Esp_Thickness, USettings.Box3D_Width);
						if (actorData.isEnemy)
							Draw3DBox(actorData, USettings.Enemy_Box3D_Esp_Color, USettings.Box3D_Esp_Thickness, USettings.Box3D_Width);
						if (actorData.isSquad)
							Draw3DBox(actorData, USettings.Squad_Box3D_Esp_Color, USettings.Box3D_Esp_Thickness, USettings.Box3D_Width);
					}

					if (USettings.Distance_Esp || USettings.Name_ESP || USettings.Type_ESP || USettings.GunName_Esp || USettings.Show_Health || USettings.Status_Esp) {
						fvector toppos = actorHeadLocation;
						fvector bottompos = actorFeetLocation;

						toppos.z += 5;
						bottompos.z -= 10;

						fvector2d topActor = w2s(toppos);
						fvector2d bottomActor = w2s(bottompos);

						bool topVisible = topActor.x >= 0 && topActor.x <= widthscreen &&
							topActor.y >= 0 && topActor.y <= heightscreen;

						if (topVisible) {
							fvector2d squadtop = topActor;

							if (USettings.Status_Esp) {
								topActor.y -= 15;
								std::string StatusDisplay;

								if (actorData.surrenderComplete && !actorData.ActorIncapacitated && !actorData.arrestComplete && !actorData.ActorKilled)
									StatusDisplay += "Surrendered";
								if (actorData.arrestComplete)
									StatusDisplay += "Arrested";
								if (actorData.ActorIncapacitated && !actorData.arrestComplete && !actorData.ActorKilled)
									StatusDisplay += "Incapacitated";
								if (actorData.ActorTakenHostage && !actorData.ActorKilled)
									StatusDisplay += "Taken Hostage";
								if (actorData.ActorKilled && !actorData.arrestComplete)
									StatusDisplay += "Killed";
								if (!actorData.arrestComplete && !actorData.surrenderComplete && !actorData.ActorIncapacitated && !actorData.ActorTakenHostage && !actorData.ActorKilled)
									StatusDisplay += "None";

								if (actorData.isCivilian)
									DrawString(topActor, USettings.Civilian_Status_Esp_Color, USettings.Text_Size, StatusDisplay.c_str());
								if (actorData.isEnemy)
									DrawString(topActor, USettings.Enemy_Status_Esp_Color, USettings.Text_Size, StatusDisplay.c_str());
							}

							if (USettings.Type_ESP) {
								topActor.y -= 15;
								squadtop.y -= 15;
								if (actorData.isCivilian)
									DrawString(topActor, USettings.Civilian_Type_ESP_Color, USettings.Text_Size, "Civilian");
								if (actorData.isEnemy)
									DrawString(topActor, USettings.Enemy_Type_ESP_Color, USettings.Text_Size, "Enemy");
								if (actorData.isSquad)
									DrawString(squadtop, USettings.Squad_Type_ESP_Color, USettings.Text_Size, "Squad");
							}

							if (USettings.Name_ESP) {
								squadtop.y -= 15;
								if (actorData.playerName[0] != L'\0') {
									std::string playerDisplay = WideToString(actorData.playerName);

									std::string filteredPlayerDisplay;
									for (char c : playerDisplay) {
										if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
											(c >= '0' && c <= '9') || c == ' ' || c == '-') {
											filteredPlayerDisplay += c;
										}
									}

									DrawString(squadtop, USettings.Squad_Name_ESP_Color, USettings.Text_Size, filteredPlayerDisplay.c_str());
								}
							}
						}

						if (USettings.Distance_Esp) {
							sprintf_s(distStr, "[%0.fm]", distance);
							if (actorData.isCivilian)
								DrawString(bottomActor, USettings.Civilian_Distance_Esp_Color, USettings.Text_Size, distStr);
							if (actorData.isEnemy)
								DrawString(bottomActor, USettings.Enemy_Distance_Esp_Color, USettings.Text_Size, distStr);
							if (actorData.isSquad)
								DrawString(bottomActor, USettings.Squad_Distance_Esp_Color, USettings.Text_Size, distStr);
						}

						if (USettings.GunName_Esp) {
							if (actorData.weaponName[0] != L'\0') {
								bottomActor.y += 15;
								std::string weaponDisplay = WideToString(actorData.weaponName);

								std::string filteredWeaponDisplay;
								for (char c : weaponDisplay) {
									if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
										(c >= '0' && c <= '9') || c == ' ' || c == '-') {
										filteredWeaponDisplay += c;
									}
								}
								weaponDisplay = filteredWeaponDisplay;

								if (USettings.Show_GunAmmo) {
									weaponDisplay += " [" + std::to_string(actorData.weaponAmmo) + "]";
								}

								if (USettings.Show_GunAmmoType) {
									std::string ammoType = (actorData.weaponAmmoType == 0) ? "AP" : "JHP";
									weaponDisplay += " (" + ammoType + ")";
								}

								if (actorData.isSquad)
									DrawString(bottomActor, USettings.Squad_GunName_Color, USettings.Text_Size, weaponDisplay.c_str());
								if (actorData.isEnemy)
									DrawString(bottomActor, USettings.Enemy_GunName_Color, USettings.Text_Size, weaponDisplay.c_str());
							}
						}

						if (USettings.Show_Health && (USettings.Show_headHealth || USettings.Show_leftArmHealth || USettings.Show_leftLegHealth || USettings.Show_rightArmHealth || USettings.Show_rightLegHealth)) {
							bottomActor.y += 15;

							if (USettings.Show_headHealth) {
								std::string headHealthText = "Head: " + std::to_string((int)actorData.headHealth) + "/" + std::to_string((int)actorData.headMaxHealth);
								if (actorData.isSquad)
									DrawString(bottomActor, USettings.Squad_Health_Esp_Color, USettings.Text_Size, headHealthText.c_str());
								if (actorData.isCivilian)
									DrawString(bottomActor, USettings.Civilian_Health_Esp_Color, USettings.Text_Size, headHealthText.c_str());
								if (actorData.isEnemy)
									DrawString(bottomActor, USettings.Enemy_Health_Esp_Color, USettings.Text_Size, headHealthText.c_str());
								bottomActor.y += 15;
							}

							if (USettings.Show_leftArmHealth) {
								std::string leftArmHealthText = "L.Arm: " + std::to_string((int)actorData.leftArmHealth) + "/" + std::to_string((int)actorData.leftArmMaxHealth);
								if (actorData.isSquad)
									DrawString(bottomActor, USettings.Squad_Health_Esp_Color, USettings.Text_Size, leftArmHealthText.c_str());
								if (actorData.isCivilian)
									DrawString(bottomActor, USettings.Civilian_Health_Esp_Color, USettings.Text_Size, leftArmHealthText.c_str());
								if (actorData.isEnemy)
									DrawString(bottomActor, USettings.Enemy_Health_Esp_Color, USettings.Text_Size, leftArmHealthText.c_str());
								bottomActor.y += 15;
							}

							if (USettings.Show_rightArmHealth) {
								std::string rightArmHealthText = "R.Arm: " + std::to_string((int)actorData.rightArmHealth) + "/" + std::to_string((int)actorData.rightArmMaxHealth);
								if (actorData.isSquad)
									DrawString(bottomActor, USettings.Squad_Health_Esp_Color, USettings.Text_Size, rightArmHealthText.c_str());
								if (actorData.isCivilian)
									DrawString(bottomActor, USettings.Civilian_Health_Esp_Color, USettings.Text_Size, rightArmHealthText.c_str());
								if (actorData.isEnemy)
									DrawString(bottomActor, USettings.Enemy_Health_Esp_Color, USettings.Text_Size, rightArmHealthText.c_str());
								bottomActor.y += 15;
							}

							if (USettings.Show_leftLegHealth) {
								std::string leftLegHealthText = "L.Leg: " + std::to_string((int)actorData.leftLegHealth) + "/" + std::to_string((int)actorData.leftLegMaxHealth);
								if (actorData.isSquad)
									DrawString(bottomActor, USettings.Squad_Health_Esp_Color, USettings.Text_Size, leftLegHealthText.c_str());
								if (actorData.isCivilian)
									DrawString(bottomActor, USettings.Civilian_Health_Esp_Color, USettings.Text_Size, leftLegHealthText.c_str());
								if (actorData.isEnemy)
									DrawString(bottomActor, USettings.Enemy_Health_Esp_Color, USettings.Text_Size, leftLegHealthText.c_str());
								bottomActor.y += 15;
							}

							if (USettings.Show_rightLegHealth) {
								std::string rightLegHealthText = "R.Leg: " + std::to_string((int)actorData.rightLegHealth) + "/" + std::to_string((int)actorData.rightLegMaxHealth);
								if (actorData.isSquad)
									DrawString(bottomActor, USettings.Squad_Health_Esp_Color, USettings.Text_Size, rightLegHealthText.c_str());
								if (actorData.isCivilian)
									DrawString(bottomActor, USettings.Civilian_Health_Esp_Color, USettings.Text_Size, rightLegHealthText.c_str());
								if (actorData.isEnemy)
									DrawString(bottomActor, USettings.Enemy_Health_Esp_Color, USettings.Text_Size, rightLegHealthText.c_str());
								bottomActor.y += 15;
							}
						}
					}
				}
				if (USettings.Show_CurrentHealth) {
					sprintf_s(HealthStr, "Current HP: %.0f", LHealth);
					DrawString({ 60, 25 }, ImColor(255, 255, 255), 1, HealthStr);
				}
				if (USettings.DrawCrosshair) {
					bool shouldDraw = !USettings.whennotaiming && !GetAsyncKeyState(VK_RBUTTON);
					if (shouldDraw) {
						fvector2d screenCenter{ widthscreen / 2, heightscreen / 2 };
						float size = USettings.Crosshair_size;
						float thickness = USettings.Crosshair_thickness;
						ImColor color = USettings.Crosshair_Color;

						if (USettings.Cross) {
							DrawLine(
								{ screenCenter.x, screenCenter.y - size - 0.5f },
								{ screenCenter.x, screenCenter.y + size - 0.5f },
								color, thickness, false
							);
							DrawLine(
								{ screenCenter.x - size - 0.5f, screenCenter.y },
								{ screenCenter.x + size - 0.5f, screenCenter.y },
								color, thickness, false
							);
						}
						if (USettings.circle) {
							DrawCircle(screenCenter, size, thickness, color);
						}
					}
				}
			}
		}
	}
	if (USettings.Aimbot) {
		if (success) {
			float LHealth;
			if (read<float>(adresses.characterHealth + offset::health, LHealth)) {
				if (LHealth > 1) {
					if (USettings.ShowFov || USettings.FilledCircle) {
						ImVec2 screenCenter{ widthscreen / 2, heightscreen / 2 };
						if (USettings.ShowFov) {
							DrawCircle({ widthscreen / 2, heightscreen / 2 }, USettings.AimFov, USettings.FovThickness, USettings.FovColor);
						}
						if (USettings.FilledCircle) {
							ImGui::GetBackgroundDrawList()->AddCircleFilled(screenCenter, USettings.AimFov, USettings.FilledCircleColor);
						}
					}

					if (USettings.ShowTarget) {
						int closestEnemyIndex = FindClosestEnemy();
						if (closestEnemyIndex != 1000) {
							if (Actor[closestEnemyIndex] != nullptr) {

								ValidatedActorData actorData = SafeGetActorCompleteData(Actor[closestEnemyIndex]);

								fvector aimPos = actorData.position;

								fvector2d screenPos = w2s(aimPos);
								DrawLine({ widthscreen / 2, heightscreen / 2 }, screenPos, USettings.TargetColor, USettings.TargetThickness, true);
							}
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

	ImGui::Begin("RON Menu", nullptr, ImGuiWindowFlags_NoCollapse);
	ImVec2 windowpos = ImGui::GetWindowPos();
	ImVec2 windowsize = ImGui::GetWindowSize();

	// Navigation bar
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
	ImGui::BeginChild("##Navigation", ImVec2(ImGui::GetContentRegionAvail().x, 50), true);
	ImVec2 child_windowpos = ImGui::GetWindowPos();
	ImVec2 child_windowsize = ImGui::GetWindowSize();

	// Navigation buttons
	float windowWidth = ImGui::GetWindowSize().x;
	float buttonWidth = (ImGui::GetContentRegionAvail().x - 30) / 5;
	float spacing = (windowWidth - (buttonWidth * 5)) / 6;

	const char* tabs[] = { "Player", "Legit", "Visuals", "World", "Config" };
	int tabValues[] = { 1, 2, 3, 5, 4 };

	for (int i = 0; i < 5; i++) {
		ImGui::SetCursorPosX(spacing * (i + 1) + buttonWidth * i);

		bool selected = (RonSettings::MenuWindow == tabValues[i]);
		if (selected) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));

		if (ImGui::Button(tabs[i], ImVec2(buttonWidth, 0))) {
			RonSettings::MenuWindow = tabValues[i];
		}

		if (selected) ImGui::PopStyleColor();

		if (i < 4) ImGui::SameLine();
	}

	ImGui::EndChild();
	ImGui::PopStyleVar();

	// Content area
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
	ImGui::BeginChild("##Content", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
	ImVec2 child_windowpos1 = ImGui::GetWindowPos();
	ImVec2 child_windowsize1 = ImGui::GetWindowSize();

	// Main menu / Welcome screen
	if (RonSettings::MenuWindow == 0) {
		ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x / 2) - 100);
		ImGui::Image((void*)pTexture, ImVec2(200, 170));
		centertext<float>("RON Menu by Noust", 0.0f, 0.0f);
		centertext<float>("Window Size X:%0.f Y:%0.f", windowsize.x, windowsize.y);
	}

	// Gun menu
	else if (RonSettings::MenuWindow == 1) {
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));

		ImGui::Text("Player Settings");
		ImGui::Checkbox("God Mode", &USettings.GodMode);
		if (USettings.GodMode && success) {
			setBooleanBit(adresses.acknowledgedPawn, offset::god_mode, 0, true);
			setBooleanBit(adresses.characterHealth, offset::unlimited_health, 0, true);
			write<float>(adresses.characterHealth + offset::health, 999999);
		}
		else if (!USettings.GodMode && USettings.GodMode_LastState && success) {
			setBooleanBit(adresses.acknowledgedPawn, offset::god_mode, 0, false);
			setBooleanBit(adresses.characterHealth, offset::unlimited_health, 0, false);
			write<float>(adresses.characterHealth + offset::health, 160);
		}
		USettings.GodMode_LastState = USettings.GodMode;
		ImGui::Checkbox("Unlimited Melee Range", &USettings.Unlimited_MeleeRange);
		if (USettings.Unlimited_MeleeRange && success) {
			write<float>(adresses.acknowledgedPawn + offset::melee_reange, 999999);
		}
		else if (!USettings.Unlimited_MeleeRange && USettings.Unlimited_MeleeRange_LastState && success) {
			write<float>(adresses.acknowledgedPawn + offset::melee_reange, 125);
		}
		USettings.Unlimited_MeleeRange_LastState = USettings.Unlimited_MeleeRange;
		ImGui::Text("Gun Settings");

		ImGui::Checkbox("Enable Aimbot", &USettings.Aimbot);
		if (USettings.Aimbot) {
			if (ImGui::Combo("Aim Key", &USettings.AimBotHotKey, "LBUTTON\0MENU\0RBUTTON\0XBUTTON1\0XBUTTON2\0CAPITAL\0SHIFT\0CONTROL"))
				USettings.SetHotKey(USettings.AimBotHotKey);

			ImGui::Text("Core Settings");

			double minVal = 0.4;
			double maxValX = 0.8f;

			ImGui::SliderScalar("Smoothness", ImGuiDataType_Double, &USettings.Smooth, &minVal, &maxValX);
			ImGui::SliderFloat("FOV", &USettings.AimFov, 10, 1920);

			ImGui::Text("Visual Feedback");
			ImGui::Checkbox("Show FOV", &USettings.ShowFov);
			if (USettings.ShowFov) {
				ImGui::SliderInt("FOV Circle Thickness", &USettings.FovThickness, 0, 10);
				ImGui::ColorEdit3("FOV Circle Color", (float*)&USettings.FovColor);
				ImGui::Checkbox("Fill FOV Circle", &USettings.FilledCircle);
				ImGui::ColorEdit3("Fill Color", (float*)&USettings.FilledCircleColor);
			}

			ImGui::Checkbox("Show Target", &USettings.ShowTarget);
			if (USettings.ShowTarget) {
				ImGui::SliderInt("Target Indicator Thickness", &USettings.TargetThickness, 0, 10);
				ImGui::ColorEdit3("Target Indicator Color", (float*)&USettings.TargetColor);
			}
			ImGui::Separator();
		}
		ImGui::Checkbox("Inifinite Ammo", &USettings.No_Reload);
		if (USettings.No_Reload && success) {
			setBooleanBit(adresses.lastweapon, offset::unlimited_ammo, 0, true);
            if (c_ammo == 0 && !defaulta) {
                read<uint16_t>(adresses.magazines + offsetof(Magazine, Ammo), c_ammo);
				defaulta = true;
			}
			int numMagazines;
			if (read<int>(adresses.lastweapon + (offset::magazines + sizeof(uintptr_t)), numMagazines)) {
				for (int i = 0; i < numMagazines; i++) {
					uintptr_t currentMagAddress = adresses.magazines + i * sizeof(Magazine);
					write<uint16_t>(currentMagAddress + offsetof(Magazine, Ammo), static_cast<uint16_t>(999));
				}
			}
		}
		else if (!USettings.No_Reload && USettings.No_Reload_LastState && success && c_ammo != 0) {
			setBooleanBit(adresses.lastweapon, offset::unlimited_ammo, 0, false);
			int numMagazines;
			if (read<int>(adresses.lastweapon + (offset::magazines + sizeof(uintptr_t)), numMagazines)) {
				for (int i = 0; i < numMagazines; i++) {
					uintptr_t currentMagAddress = adresses.magazines + i * sizeof(Magazine);
					write<uint16_t>(currentMagAddress + offsetof(Magazine, Ammo), static_cast<uint16_t>(c_ammo));
				}
				c_ammo = 0;
				defaulta = false;
			}
		}
		USettings.No_Reload_LastState = USettings.No_Reload;
        ImGui::Checkbox("High Fire Rate", &USettings.High_FireRate);
        if (USettings.High_FireRate && success) {
            if (saved_fire_rate == 0.0f && !defaultf) {
                read<float>(adresses.lastweapon + offset::fire_rate, saved_fire_rate);
                defaultf = true;
            }
            write<float>(adresses.lastweapon + offset::fire_rate, 0.001f);
        }
        else if (!USettings.High_FireRate && USettings.High_FireRate_LastState && success && saved_fire_rate != 0.0f) {
            write<float>(adresses.lastweapon + offset::fire_rate, saved_fire_rate);
            saved_fire_rate = 0.0f; 
            defaultf = false;
        }
		USettings.High_FireRate_LastState = USettings.High_FireRate;
		ImGui::Checkbox("Show Magazines", &USettings.Show_Magazines);
		if (USettings.Show_Magazines && RonSettings::MenuWindow == 1 && success) {
			int numMagazines;
			if (read<int>(adresses.lastweapon + (offset::magazines + sizeof(uintptr_t)), numMagazines)) {
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.15f, 0.15f, 0.9f));
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.28f, 0.28f, 0.28f, 0.8f));
				
				// Calculate dynamic height based on content
				float baseHeight = 80.0f; // Header, separator, spacing, footer
				float rowHeight = 25.0f;   // Height per magazine row
				float dynamicHeight = baseHeight + (numMagazines * rowHeight);
				
				if (ImGui::BeginChild("##magazines_window", ImVec2(0, dynamicHeight), true, ImGuiWindowFlags_NoScrollbar)) {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::Text("Magazine Information");
					ImGui::PopStyleColor();
					ImGui::Separator();
					ImGui::Spacing();
					
					ImGui::Columns(3, "magazine_columns", true);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::Text("Magazine #");
					ImGui::NextColumn();
					ImGui::Text("Ammo Count");
					ImGui::NextColumn();
					ImGui::Text("Ammo Type");
					ImGui::PopStyleColor();
					ImGui::NextColumn();
					ImGui::Separator();
					
					for (int i = 0; i < numMagazines; i++) {
						Magazine weapon_magazine = *(Magazine*)(adresses.magazines + i * sizeof(Magazine));
						uintptr_t currentMagAddress = adresses.magazines + i * sizeof(Magazine);
						
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.33f, 0.67f, 0.86f, 1.0f));
						ImGui::Text("Mag %d", i + 1);
						ImGui::PopStyleColor();
						ImGui::NextColumn();
						
						// Editable Ammo Count
						ImVec4 ammoColor;
						if (weapon_magazine.Ammo > 25) {
							ammoColor = ImVec4(0.2f, 1.0f, 0.2f, 1.0f);
						} else if (weapon_magazine.Ammo > 10) {
							ammoColor = ImVec4(1.0f, 1.0f, 0.2f, 1.0f);
						} else {
							ammoColor = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
						}
						
						ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.2f, 0.8f));
						ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.8f));
						ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.4f, 0.4f, 0.4f, 0.8f));
						ImGui::PushStyleColor(ImGuiCol_Text, ammoColor);
						
						char ammoBuffer[8];
						snprintf(ammoBuffer, sizeof(ammoBuffer), "%d", weapon_magazine.Ammo);
						ImGui::SetNextItemWidth(50);
						
						char inputId[32];
						snprintf(inputId, sizeof(inputId), "##ammo_%d", i);
						
						if (ImGui::InputText(inputId, ammoBuffer, sizeof(ammoBuffer), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue)) {
							int newAmmo = atoi(ammoBuffer);
							if (newAmmo >= 0 && newAmmo <= 9999) {
								if (write<uint16_t>(currentMagAddress + offsetof(Magazine, Ammo), static_cast<uint16_t>(newAmmo))) {

								}
							}
						}
						
						ImGui::PopStyleColor(4);
						ImGui::NextColumn();
						
						const char* ammoTypeStr;
						ImVec4 typeColor;
						switch (weapon_magazine.Ammo_Type) {
							case 0:
								ammoTypeStr = "AP";
								typeColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
								break;
							case 1:
								ammoTypeStr = "JHP";
								typeColor = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
								break;
							default:
								ammoTypeStr = "Unknown";
								typeColor = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);
								break;
						}
						
						char buttonId[32];
						snprintf(buttonId, sizeof(buttonId), "%s##type_%d", ammoTypeStr, i);
						
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 0.8f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.8f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 0.8f));
						ImGui::PushStyleColor(ImGuiCol_Text, typeColor);
						
						if (ImGui::Button(buttonId, ImVec2(60, 0))) {
							uint16_t newType = (weapon_magazine.Ammo_Type == 0) ? 1 : 0;
							if (write<uint16_t>(currentMagAddress + offsetof(Magazine, Ammo_Type), newType)) {

							}
						}
						
						ImGui::PopStyleColor(4);
						ImGui::NextColumn();
					}
					
					ImGui::Columns(1);
					ImGui::Spacing();
					
					ImGui::Separator();
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::Text("Total Magazines: %d", numMagazines);
					ImGui::PopStyleColor();
				}
				ImGui::EndChild();
				
				// Cleanup styles
				ImGui::PopStyleColor(2);
				ImGui::PopStyleVar(2);
			}
		}

		ImGui::PopStyleColor();
	}

	else if (RonSettings::MenuWindow == 2) {
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));

		// Movement & Utility Section
		ImGui::Text("Movement & Utility");
		ImGui::Checkbox("Fast run", &USettings.fast_run);
		if (USettings.fast_run && success) {
			USettings.super_run = false;
			write<float>(adresses.acknowledgedPawn + offset::speed, 1000);
			write<float>(adresses.acknowledgedPawn + offset::acceleration, 1000);
		}else if (!USettings.fast_run && USettings.fast_run_LastState && success) {
			write<float>(adresses.acknowledgedPawn + offset::speed, 320);
			write<float>(adresses.acknowledgedPawn + offset::acceleration, 500);
		}
		USettings.fast_run_LastState = USettings.fast_run;
		ImGui::Checkbox("Super run", &USettings.super_run);
		if (USettings.super_run && success) {
			USettings.fast_run = false;
			write<float>(adresses.acknowledgedPawn + offset::speed, 2000);
			write<float>(adresses.acknowledgedPawn + offset::acceleration, 2000);
		}
		else if (!USettings.super_run && USettings.super_run_LastState && success) {
			write<float>(adresses.acknowledgedPawn + offset::speed, 320);
			write<float>(adresses.acknowledgedPawn + offset::acceleration, 500);
		}
		USettings.super_run_LastState = USettings.super_run;
		ImGui::Checkbox("Able to jump", &USettings.jump);
		ImGui::PopStyleColor();
	}
	else if (RonSettings::MenuWindow == 3) {
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));

		// View Settings Section
		ImGui::Text("View Settings Section");
		ImGui::Checkbox("Fov Changer", &USettings.fov_changer);
		if (USettings.fov_changer) {
			ImGui::SliderFloat("Fov Value", &USettings.fov_value, 40.0f, 170.0f, "%.0f");
			ImGui::SliderFloat("Base Fov Value", &USettings.base_fov_value, 90.0f, 170.0f, "%.0f");
			if (success) {
				write<float>(adresses.cameraComponent + offset::fov, USettings.fov_value);
				write<float>(adresses.acknowledgedPawn + offset::base_fov, USettings.base_fov_value);
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

		//ImGui::Spacing();
		//ImGui::Separator();
		//ImGui::Spacing();

		//// ESP Features Section
		ImGui::Text("ESP Features");
		ImGui::Checkbox("Render Overlay", &USettings.esp);
		if (USettings.esp) {
			// Snap Line ESP
			ImGui::Checkbox("Snap Lines", &USettings.SnaplLine_Esp);
			if (USettings.SnaplLine_Esp) {
				ImGui::ColorEdit3("Enemy Line Color", (float*)&USettings.Enemy_SnaplLine_Esp_Color);
				ImGui::ColorEdit3("Civilian Line Color", (float*)&USettings.Civilian_SnaplLine_Esp_Color);
				ImGui::ColorEdit3("Squad Line Color", (float*)&USettings.Squad_SnaplLine_Esp_Color);
				ImGui::SliderInt("Line Thickness", &USettings.SnaplLine_Esp_Thickness, 0, 10);

				ImGui::Text("Line Start Position");
				if (ImGui::BeginTable("LinePositions", 2)) {
					ImGui::TableNextColumn();
					if (ImGui::Button("Bottom")) USettings.SnaplLine_Esp_Start_Point = { widthscreen / 2, heightscreen };
					if (ImGui::Button("Left")) USettings.SnaplLine_Esp_Start_Point = { 0, heightscreen / 2 };
					ImGui::TableNextColumn();
					if (ImGui::Button("Top")) USettings.SnaplLine_Esp_Start_Point = { widthscreen / 2, 0 };
					if (ImGui::Button("Right")) USettings.SnaplLine_Esp_Start_Point = { widthscreen, heightscreen / 2 };
					ImGui::EndTable();
				}

				double minVal = 0.0;
				double maxValX = static_cast<double>(widthscreen);
				double maxValY = static_cast<double>(heightscreen);
				ImGui::SliderScalar("Custom X", ImGuiDataType_Double, &USettings.SnaplLine_Esp_Start_Point.x, &minVal, &maxValX);
				ImGui::SliderScalar("Custom Y", ImGuiDataType_Double, &USettings.SnaplLine_Esp_Start_Point.y, &minVal, &maxValY);

				ImGui::Text("Line End Point");
				if (ImGui::Button(USettings.SnaplLine_Esp_End_Point ? "Head" : "Feet")) {
					USettings.SnaplLine_Esp_End_Point = !USettings.SnaplLine_Esp_End_Point;
				}
				ImGui::Separator();
			}

			// Box ESP Options
			ImGui::Checkbox("Box ESP", &USettings.Box_ESP);
			if (USettings.Box_ESP) {
				ImGui::ColorEdit3("Enemy Box Color", (float*)&USettings.Enemy_Box_Esp_Color);
				ImGui::ColorEdit3("Civilian Box Color", (float*)&USettings.Civilian_Box_Esp_Color);
				ImGui::ColorEdit3("Squad Box Color", (float*)&USettings.Squad_Box_Esp_Color);
				ImGui::SliderInt("Box Thickness", &USettings.Box_Esp_Thickness, 0, 10);
				ImGui::Separator();
			}

			ImGui::Checkbox("Corner Box", &USettings.CornerBox_ESP);
			if (USettings.CornerBox_ESP) {
				ImGui::ColorEdit3("Enemy Corner Color", (float*)&USettings.Enemy_CornerBox_Esp_Color);
				ImGui::ColorEdit3("Civilian Corner Color", (float*)&USettings.Civilian_CornerBox_Esp_Color);
				ImGui::ColorEdit3("Squad Corner Color", (float*)&USettings.Squad_CornerBox_Esp_Color);
				ImGui::SliderInt("Corner Thickness", &USettings.Box_CornerEsp_Thickness, 0, 10);
				ImGui::Separator();
			}

			ImGui::Checkbox("3D Box", &USettings.Box3D_Esp);
			if (USettings.Box3D_Esp) {
				ImGui::ColorEdit3("Enemy 3D Box Color", (float*)&USettings.Enemy_Box3D_Esp_Color);
				ImGui::ColorEdit3("Civilian 3D Box Color", (float*)&USettings.Civilian_Box3D_Esp_Color);
				ImGui::ColorEdit3("Squad 3D Box Color", (float*)&USettings.Squad_Box3D_Esp_Color);
				ImGui::SliderInt("3D Box Thickness", &USettings.Box3D_Esp_Thickness, 0, 10);
				ImGui::SliderFloat("3D Box Width", &USettings.Box3D_Width, 10, 40);
				ImGui::Separator();
			}

			ImGui::Checkbox("Filled Box", &USettings.FilledBox_Esp);
			if (USettings.FilledBox_Esp) {
				ImGui::ColorEdit4("Enemy Fill Color", (float*)&USettings.Enemy_FilledBox_Esp_Color);
				ImGui::ColorEdit4("Civilian Fill Color", (float*)&USettings.Civilian_FilledBox_Esp_Color);
				ImGui::ColorEdit4("Squad Fill Color", (float*)&USettings.Squad_FilledBox_Esp_Color);
				ImGui::Separator();
			}

			// Player Info ESP
			ImGui::Checkbox("Show Player Names", &USettings.Name_ESP);
			if (USettings.Name_ESP) {
				ImGui::ColorEdit3("Player Name Color", (float*)&USettings.Squad_Name_ESP_Color);
				ImGui::Separator();
			}

			ImGui::Checkbox("Show Types", &USettings.Type_ESP);
			if (USettings.Type_ESP) {
				ImGui::ColorEdit3("Squad Type Color", (float*)&USettings.Squad_Type_ESP_Color);
				ImGui::ColorEdit3("Enemy Type Color", (float*)&USettings.Enemy_Type_ESP_Color);
				ImGui::ColorEdit3("Civilian Type Color", (float*)&USettings.Civilian_Type_ESP_Color);
				ImGui::Separator();
			}

			ImGui::Checkbox("Show Distance", &USettings.Distance_Esp);
			if (USettings.Distance_Esp) {
				ImGui::ColorEdit3("Enemy Distance Color", (float*)&USettings.Enemy_Distance_Esp_Color);
				ImGui::ColorEdit3("Civilian Distance Color", (float*)&USettings.Civilian_Distance_Esp_Color);
				ImGui::ColorEdit3("Squad Distance Color", (float*)&USettings.Squad_Distance_Esp_Color);
				ImGui::Separator();
			}

			ImGui::Checkbox("Show Status", &USettings.Status_Esp);
			if (USettings.Status_Esp) {
				ImGui::ColorEdit3("Enemy Status Color", (float*)&USettings.Enemy_Status_Esp_Color);
				ImGui::ColorEdit3("Civilian Status Color", (float*)&USettings.Civilian_Status_Esp_Color);
				ImGui::Separator();
			}

			ImGui::Checkbox("Show Weapon", &USettings.GunName_Esp);
			if (USettings.GunName_Esp) {
				ImGui::ColorEdit3("Enemy Weapon Color", (float*)&USettings.Enemy_GunName_Color);
				ImGui::ColorEdit3("Squad Weapon Color", (float*)&USettings.Squad_GunName_Color);
				ImGui::Separator();
				ImGui::Text("Weapon Info");
				ImGui::Checkbox("Show Weapon Ammo", &USettings.Show_GunAmmo);
				ImGui::Checkbox("Show Weapon Ammo Type", &USettings.Show_GunAmmoType);
				ImGui::Separator();
			}

			//ImGui::Checkbox("Show Bones", &USettings.Bone_Esp);
			//if (USettings.Bone_Esp) {
			//	ImGui::ColorEdit3("Enemy Bone Color", (float*)&USettings.Enemy_Bone_Esp_Color);
			//	ImGui::ColorEdit3("Squad Bone Color", (float*)&USettings.Squad_Bone_Esp_Color);
			//	ImGui::SliderInt("Bone Thickness", &USettings.Bone_Esp_Thickness, 0, 10);
			//	ImGui::Separator();
			//}

			ImGui::Checkbox("Show Health", &USettings.Show_Health);
			if (USettings.Show_Health) {
				ImGui::Text("Health Bar");
				ImGui::Checkbox("Show Health Bar", &USettings.HealthBar_ESP);
				ImGui::Text("Especific Health Info");
				ImGui::Checkbox("Show Current Health", &USettings.Show_CurrentHealth);
				ImGui::Checkbox("Show Head Health", &USettings.Show_headHealth);
				ImGui::Checkbox("Show Left Arm Health", &USettings.Show_leftArmHealth);
				ImGui::Checkbox("Show Right Arm Health", &USettings.Show_rightArmHealth);
				ImGui::Checkbox("Show Left Leg Health", &USettings.Show_leftLegHealth);
				ImGui::Checkbox("Show Right Leg Health", &USettings.Show_rightLegHealth);
				ImGui::ColorEdit3("Enemy Halth Color", (float*)&USettings.Enemy_Health_Esp_Color);
				ImGui::ColorEdit3("Civilian Halth Color", (float*)&USettings.Civilian_Health_Esp_Color);
				ImGui::ColorEdit3("Squad Halth Color", (float*)&USettings.Squad_Health_Esp_Color);
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			// Crosshair Settings
			ImGui::Text("Crosshair Settings");
			ImGui::Checkbox("Custom Crosshair", &USettings.DrawCrosshair);
			if (USettings.DrawCrosshair) {
				ImGui::Checkbox("Show When Not Aiming", &USettings.whennotaiming);
				ImGui::ColorEdit3("Crosshair Color", (float*)&USettings.Crosshair_Color);
				ImGui::SliderFloat("Size", &USettings.Crosshair_size, 1, 30);
				ImGui::SliderInt("Thickness", &USettings.Crosshair_thickness, 0, 10);
				ImGui::Checkbox("Circle", &USettings.circle);
				ImGui::Checkbox("Cross", &USettings.Cross);
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			// Global ESP Settings
			ImGui::Text("Global ESP Settings");
			ImGui::SliderInt("Max ESP Distance", &USettings.ESP_Distance, 0, 100);
			ImGui::SliderFloat("Text Size", &USettings.Text_Size, 0.4f, 2.0f, "%.2f");

		}

		ImGui::PopStyleColor();
	}
	else if (RonSettings::MenuWindow == 5) {
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));

		// World Settings Section
		ImGui::Text("World Settings");
		ImGui::Checkbox("Night Mode", &USettings.Night_Mode);
		ImGui::Checkbox("Full Bright", &USettings.FullBright_Mode);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Actors Settings");
		ImGui::Checkbox("Show Actors", &USettings.Show_actors);
		if (USettings.Show_actors && RonSettings::MenuWindow == 5) {
			// First collect all actors by type
			std::vector<std::pair<int, ValidatedActorData>> squadActors;
			std::vector<std::pair<int, ValidatedActorData>> civilianActors;
			std::vector<std::pair<int, ValidatedActorData>> enemyActors;
			ValidatedActorData actorData;

			for (int i = 0; i < 254; i++) {
				if (Actor[i] == nullptr) continue;

				actorData = SafeGetActorCompleteData(Actor[i]);
				if (!actorData.isValid) continue;

				if (adresses.acknowledgedPawn == reinterpret_cast<DWORD64>(actorData.ptrToActor)) continue;

				// Calculate distance
				double distance = POV.Location.distance(actorData.position) / 100;

				if (actorData.isSquad) {
					squadActors.push_back({ i, actorData });
				}
				else if (actorData.isCivilian) {
					civilianActors.push_back({ i, actorData });
				}
				else if (actorData.isEnemy) {
					enemyActors.push_back({ i, actorData });
				}
			}

			// Create main container with professional styling
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.15f, 0.15f, 0.9f));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.28f, 0.28f, 0.28f, 0.8f));

			// Calculate content height dynamically
			float baseHeight = 0.0f;
			float itemHeight = ImGui::GetTextLineHeightWithSpacing();
			float buttonHeight = ImGui::GetFrameHeight();
			float spacing = ImGui::GetStyle().ItemSpacing.y;
			
			// Base elements: title, separator, spacing
			baseHeight += itemHeight + spacing + spacing; // Title + separator + spacing
			
			// Squad section height
			if (!squadActors.empty()) {
				baseHeight += itemHeight; // Section title
				baseHeight += buttonHeight + spacing; // Heal button
				baseHeight += squadActors.size() * (itemHeight + spacing * 2); // Collapsing headers
			}
			
			// Civilian section height
			if (!civilianActors.empty()) {
				baseHeight += itemHeight; // Section title
				baseHeight += buttonHeight + spacing; // Action buttons
				baseHeight += civilianActors.size() * (itemHeight + spacing * 2); // Collapsing headers
			}
			
			// Enemy section height
			if (!enemyActors.empty()) {
				baseHeight += itemHeight; // Section title
				baseHeight += buttonHeight + spacing; // Action buttons
				baseHeight += enemyActors.size() * (itemHeight + spacing * 2); // Collapsing headers
			}
			
			// Summary section
			baseHeight += spacing + itemHeight; // Summary text
			
			// Add some padding and limit maximum height
			baseHeight += 40.0f; // Extra padding
			float maxHeight = ImGui::GetContentRegionAvail().y; // Leave space for other elements
			float finalHeight = (baseHeight < maxHeight) ? baseHeight : maxHeight;
			
			if (ImGui::BeginChild("##actors_window", ImVec2(0, finalHeight), true, (baseHeight > maxHeight) ? 0 : ImGuiWindowFlags_NoScrollbar)) {
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
				ImGui::Text("Actor Management System");
				ImGui::PopStyleColor();
				ImGui::Separator();
				ImGui::Spacing();

				// Squad Section
				if (!squadActors.empty()) {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 1.0f, 1.0f));
					ImGui::Text("SQUAD MEMBERS (%d)", (int)squadActors.size());
					ImGui::PopStyleColor();

					// Squad action buttons with proper ID scoping
					ImGui::PushID("squad_actions");
					
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					if (ImGui::Button("Heal All Squad", ImVec2(120, 0))) {
						for (auto& actorPair : squadActors) {
							if (actorPair.second.isValid && actorPair.second.ptrToActor && actorPair.second.isSquad) {
								float maxHealth = actorPair.second.maxHealth;
								SafeSetActorHealth(actorPair.second.ptrToActor, maxHealth);
							}
						}
					}
					ImGui::PopStyleColor(4);
					
					ImGui::PopID(); // End squad_actions ID scope

					ImGui::Spacing();

					// Squad individuals
					int squadCounter = 1;
					for (auto& actorPair : squadActors) {
						int actorIndex = actorPair.first;
						ValidatedActorData& actorData = actorPair.second;
						double distance = POV.Location.distance(actorData.position) / 100;

						char squadId[32];
						snprintf(squadId, sizeof(squadId), "##squad_%d", actorIndex);

						if (ImGui::CollapsingHeader(("Squad Member " + std::to_string(squadCounter)).c_str())) {
							ImGui::Indent();

							// Status information
							ImGui::Text("Health: %.1f/%.1f", actorData.health, actorData.maxHealth);
							ImGui::Text("Distance: %.1fm (%s)", distance, distance > 30.0 ? "Far" : "Near");

							// Player name if exists
							if (actorData.playerName[0] != L'\0') {
								std::string playerDisplay = WideToString(actorData.playerName);
								std::string filteredPlayerDisplay;
								for (char c : playerDisplay) {
									if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
										(c >= '0' && c <= '9') || c == ' ' || c == '-') {
										filteredPlayerDisplay += c;
									}
								}
								ImGui::Text("Player: %s", filteredPlayerDisplay.c_str());
							}

							// Status
							std::string StatusDisplay;
							if (actorData.ActorKilled)
								StatusDisplay += "Killed";
							if (!actorData.ActorKilled)
								StatusDisplay += "None";
							ImGui::Text("Status: %s", StatusDisplay.c_str());

							// Weapon info for squad
							if (actorData.weaponName[0] != L'\0') {
								std::string weaponDisplay = WideToString(actorData.weaponName);
								std::string filteredWeaponDisplay;
								for (char c : weaponDisplay) {
									if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
										(c >= '0' && c <= '9') || c == ' ' || c == '-') {
										filteredWeaponDisplay += c;
									}
								}
								ImGui::Text("Weapon: %s", filteredWeaponDisplay.c_str());
								ImGui::Text("Ammo: %d (Type: %d)", actorData.weaponAmmo, actorData.weaponAmmoType);
							}

							// Individual actions
							ImGui::Spacing();
							ImGui::PushID(actorIndex);
							static char healthBuffer[8];
							snprintf(healthBuffer, sizeof(healthBuffer), "%.0f", actorData.health);
							ImGui::SetNextItemWidth(80);
							if (ImGui::InputText("Health", healthBuffer, sizeof(healthBuffer), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue)) {
								float newHealth = (float)atof(healthBuffer);
								if (newHealth >= 0.0f && newHealth <= actorData.maxHealth) {
									SafeSetActorHealth(actorData.ptrToActor, newHealth);
								}
							}

							ImGui::Spacing();
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

							if (ImGui::Button("Kill", ImVec2(60, 0))) {
								SafeSetActorHealth(actorData.ptrToActor, 0.0f);
							}
							ImGui::SameLine();
							if (ImGui::Button("Heal", ImVec2(60, 0))) {
								SafeSetActorHealth(actorData.ptrToActor, actorData.maxHealth);
							}

							ImGui::PopStyleColor(4);
							ImGui::PopID();

							ImGui::Unindent();
						}
						squadCounter++;
					}
					ImGui::Separator();
				}

				// Civilian Section
				if (!civilianActors.empty()) {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 1.0f, 0.2f, 1.0f));
					ImGui::Text("CIVILIANS (%d)", (int)civilianActors.size());
					ImGui::PopStyleColor();

					// Civilian action buttons with proper ID scoping
					ImGui::PushID("civilian_actions");
					
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					if (ImGui::Button("Arrest All", ImVec2(100, 0))) {
						for (auto& actorPair : civilianActors) {
							if (actorPair.second.isValid && actorPair.second.ptrToActor && actorPair.second.isCivilian) {
								SafeSetActorArrestStatus(actorPair.second.ptrToActor, true);
							}
						}
					}
					ImGui::PopStyleColor(4);

					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					if (ImGui::Button("Force Surrender All", ImVec2(140, 0))) {
						for (auto& actorPair : civilianActors) {
							if (actorPair.second.isValid && actorPair.second.ptrToActor && actorPair.second.isCivilian) {
								SafeSetActorSurrenderStatus(actorPair.second.ptrToActor, true);
							}
						}
					}
					ImGui::PopStyleColor(4);

					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					if (ImGui::Button("Kill All", ImVec2(100, 0))) {
						for (auto& actorPair : civilianActors) {
							if (actorPair.second.isValid && actorPair.second.ptrToActor && actorPair.second.isCivilian) {
								SafeSetActorHealth(actorPair.second.ptrToActor, 0.0f);
							}
						}
					}
					ImGui::PopStyleColor(4);
					
					ImGui::PopID(); // End civilian_actions ID scope

					ImGui::Spacing();

					// Civilian individuals
					int civilianCounter = 1;
					for (auto& actorPair : civilianActors) {
						int actorIndex = actorPair.first;
						ValidatedActorData& actorData = actorPair.second;
						double distance = POV.Location.distance(actorData.position) / 100;

						if (ImGui::CollapsingHeader(("Civilian " + std::to_string(civilianCounter)).c_str())) {
							ImGui::Indent();

							// Status information
							ImGui::Text("Health: %.1f/%.1f", actorData.health, actorData.maxHealth);
							ImGui::Text("Distance: %.1fm (%s)", distance, distance > 30.0 ? "Far" : "Near");

							// Status
							std::string StatusDisplay;
							if (actorData.surrenderComplete && !actorData.ActorIncapacitated && !actorData.arrestComplete && !actorData.ActorKilled)
								StatusDisplay += "Surrendered";
							if (actorData.arrestComplete)
								StatusDisplay += "Arrested";
							if (actorData.ActorIncapacitated && !actorData.arrestComplete && !actorData.ActorKilled)
								StatusDisplay += "Incapacitated";
							if (actorData.ActorTakenHostage && !actorData.ActorKilled)
								StatusDisplay += "Taken Hostage";
							if (actorData.ActorKilled && !actorData.arrestComplete)
								StatusDisplay += "Killed";
							if (!actorData.arrestComplete && !actorData.surrenderComplete && !actorData.ActorIncapacitated && !actorData.ActorTakenHostage && !actorData.ActorKilled)
								StatusDisplay += "None";
							ImGui::Text("Status: %s", StatusDisplay.c_str());

							// Individual actions
							ImGui::Spacing();
							ImGui::PushID(actorIndex);

							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

							if (ImGui::Button("Arrest", ImVec2(60, 0))) {
								SafeSetActorArrestStatus(actorData.ptrToActor, true);
							}
							ImGui::SameLine();
							if (ImGui::Button("Surrender", ImVec2(80, 0))) {
								SafeSetActorSurrenderStatus(actorData.ptrToActor, true);
							}
							ImGui::SameLine();
							if (ImGui::Button("Kill", ImVec2(60, 0))) {
								SafeSetActorHealth(actorData.ptrToActor, 0.0f);
							}

							ImGui::PopStyleColor(4);
							ImGui::PopID();
							ImGui::Unindent();
						}
						civilianCounter++;
					}
					ImGui::Separator();
				}

				// Enemy Section
				if (!enemyActors.empty()) {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
					ImGui::Text("ENEMIES (%d)", (int)enemyActors.size());
					ImGui::PopStyleColor();

					// Enemy action buttons with proper ID scoping
					ImGui::PushID("enemy_actions");
					
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					if (ImGui::Button("Kill All", ImVec2(100, 0))) {
						for (auto& actorPair : enemyActors) {
							if (actorPair.second.isValid && actorPair.second.ptrToActor && actorPair.second.isEnemy) {
								SafeSetActorHealth(actorPair.second.ptrToActor, 0.0f);
							}
						}
					}
					ImGui::PopStyleColor(4);

					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					if (ImGui::Button("Arrest All", ImVec2(100, 0))) {
						for (auto& actorPair : enemyActors) {
							if (actorPair.second.isValid && actorPair.second.ptrToActor && actorPair.second.isEnemy) {
								SafeSetActorArrestStatus(actorPair.second.ptrToActor, true);
							}
						}
					}
					ImGui::PopStyleColor(4);

					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					if (ImGui::Button("Force Surrender All", ImVec2(140, 0))) {
						for (auto& actorPair : enemyActors) {
							if (actorPair.second.isValid && actorPair.second.ptrToActor && actorPair.second.isEnemy) {
								SafeSetActorSurrenderStatus(actorPair.second.ptrToActor, true);
							}
						}
					}
					ImGui::PopStyleColor(4);
					
					ImGui::PopID(); // End enemy_actions ID scope

					ImGui::Spacing();

					// Enemy individuals
					int enemyCounter = 1;
					for (auto& actorPair : enemyActors) {
						int actorIndex = actorPair.first;
						ValidatedActorData& actorData = actorPair.second;
						double distance = POV.Location.distance(actorData.position) / 100;

						if (ImGui::CollapsingHeader(("Enemy " + std::to_string(enemyCounter)).c_str())) {
							ImGui::Indent();

							// Status information
							ImGui::Text("Health: %.1f/%.1f", actorData.health, actorData.maxHealth);
							ImGui::Text("Distance: %.1fm (%s)", distance, distance > 30.0 ? "Far" : "Near");

							// Status
							std::string StatusDisplay;
							if (actorData.surrenderComplete && !actorData.ActorIncapacitated && !actorData.arrestComplete && !actorData.ActorKilled)
								StatusDisplay += "Surrendered";
							if (actorData.arrestComplete)
								StatusDisplay += "Arrested";
							if (actorData.ActorIncapacitated && !actorData.arrestComplete && !actorData.ActorKilled)
								StatusDisplay += "Incapacitated";
							if (actorData.ActorTakenHostage && !actorData.ActorKilled)
								StatusDisplay += "Taken Hostage";
							if (actorData.ActorKilled && !actorData.arrestComplete)
								StatusDisplay += "Killed";
							if (!actorData.arrestComplete && !actorData.surrenderComplete && !actorData.ActorIncapacitated && !actorData.ActorTakenHostage && !actorData.ActorKilled)
								StatusDisplay += "None";
							ImGui::Text("Status: %s", StatusDisplay.c_str());

							// Weapon info for enemies
							if (actorData.weaponName[0] != L'\0') {
								std::string weaponDisplay = WideToString(actorData.weaponName);
								std::string filteredWeaponDisplay;
								for (char c : weaponDisplay) {
									if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
										(c >= '0' && c <= '9') || c == ' ' || c == '-') {
										filteredWeaponDisplay += c;
									}
								}
								ImGui::Text("Weapon: %s", filteredWeaponDisplay.c_str());
								ImGui::Text("Ammo: %d (Type: %d)", actorData.weaponAmmo, actorData.weaponAmmoType);
							}

							// Individual actions
							ImGui::Spacing();
							ImGui::PushID(actorIndex);

							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
							if (ImGui::Button("Kill", ImVec2(50, 0))) {
								SafeSetActorHealth(actorData.ptrToActor, 0.0f);
							}
							ImGui::SameLine();
							if (ImGui::Button("Arrest", ImVec2(60, 0))) {
								SafeSetActorArrestStatus(actorData.ptrToActor, true);
							}
							ImGui::SameLine();
							if (ImGui::Button("Surrender", ImVec2(80, 0))) {
								SafeSetActorSurrenderStatus(actorData.ptrToActor, true);
							}
							ImGui::PopStyleColor(4);

							ImGui::PopID();
							ImGui::Unindent();
						}
						enemyCounter++;
					}
					ImGui::Separator();
				}

				// Summary
				ImGui::Spacing();
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
				ImGui::Text("Total Actors: %d | Squad: %d | Civilians: %d | Enemies: %d",
					(int)(squadActors.size() + civilianActors.size() + enemyActors.size()),
					(int)squadActors.size(), (int)civilianActors.size(), (int)enemyActors.size());
				ImGui::PopStyleColor();
			}
			ImGui::EndChild();

			// Cleanup styles
			ImGui::PopStyleColor(2);
			ImGui::PopStyleVar(2);
		}
		
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::PopStyleColor();
	}
	else if (RonSettings::MenuWindow == 4) {
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
		ImGui::Checkbox("Show Squad", &USettings.Show_Squad);
		ImGui::Checkbox("Show Civilians", &USettings.Show_Civilian);
		ImGui::Checkbox("Show Enemies", &USettings.Show_Enemy);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Quick Actions
		ImGui::Text("Quick Actions");
		if (ImGui::Button("Disable All Features")) {
			disable_all();
		}

		ImGui::SameLine();
		if (ImGui::Button("Reset to Default")) {
			default_all();
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