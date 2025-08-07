#pragma once
#include "include.h"

namespace CSettings {
	int MenuWindow = 0;

	struct Settings {
		//esp
		bool esp = false;
		bool Show_Enemy = true;
		bool Show_Civilian = true;
		bool Show_Squad = true;

		int HealthBar_Esp_Thickness = 2;
		bool HealthBar_ESP = true;
		bool Show_CurrentHealth = false;
		bool Show_headHealth = false;
		bool Show_leftArmHealth = false;
		bool Show_leftLegHealth = false;
		bool Show_rightArmHealth = false;
		bool Show_rightLegHealth = false;
		ImColor Enemy_Health_Esp_Color = { 255,0,0 };
		ImColor Civilian_Health_Esp_Color = { 0,255,0 };
		ImColor Squad_Health_Esp_Color = { 0,0,255 };
		bool Show_Health = false;

		ImColor Enemy_Box_Esp_Color = { 255,0,0 };
		ImColor Civilian_Box_Esp_Color = { 0,255,0 };
		ImColor Squad_Box_Esp_Color = { 0,0,255 };
		int Box_Esp_Thickness = 0;
		bool Box_ESP = false;

		ImColor Enemy_CornerBox_Esp_Color = { 255,0,0 };
		ImColor Civilian_CornerBox_Esp_Color = { 0,255,0 };
		ImColor Squad_CornerBox_Esp_Color = { 0,0,255 };
		int Box_CornerEsp_Thickness = 0;
		bool CornerBox_ESP = false;

		ImColor Squad_Name_ESP_Color = { 255,255,255 };
		bool Name_ESP = false;

		ImColor Squad_Type_ESP_Color = { 0,0,255 };
		ImColor Enemy_Type_ESP_Color = { 255,0,0 };
		ImColor Civilian_Type_ESP_Color = { 0,255,0 };
		bool Type_ESP = false;

		ImColor Enemy_Distance_Esp_Color = { 255,0,0 };
		ImColor Civilian_Distance_Esp_Color = { 0,255,0 };
		ImColor Squad_Distance_Esp_Color = { 0,0,255 };
		bool Distance_Esp = false;

		ImColor Enemy_Status_Esp_Color = { 255,0,0 };
		ImColor Civilian_Status_Esp_Color = { 0,255,0 };
		bool Status_Esp = false;

		fvector2d SnaplLine_Esp_Start_Point = { widthscreen / 2,heightscreen };
		bool SnaplLine_Esp_End_Point = false;
		ImColor Enemy_SnaplLine_Esp_Color = { 255,0,0 };
		ImColor Civilian_SnaplLine_Esp_Color = { 0,255,0 };
		ImColor Squad_SnaplLine_Esp_Color = { 0,0,255 };
		int SnaplLine_Esp_Thickness = 0;
		bool SnaplLine_Esp = false;

		ImColor Enemy_FilledBox_Esp_Color = { 0,0,0,70 };
		ImColor Civilian_FilledBox_Esp_Color = { 0,0,0,70 };
		ImColor Squad_FilledBox_Esp_Color = { 0,0,0,70 };
		bool FilledBox_Esp = false;

		ImColor Enemy_GunName_Color = { 255,0,0 };
		ImColor Squad_GunName_Color = { 0,0,255 };
		bool Show_GunAmmo = false;
		bool Show_GunAmmoType = false;
		bool GunName_Esp = false;

		ImColor Enemy_Box3D_Esp_Color = { 255,0,0 };
		ImColor Civilian_Box3D_Esp_Color = { 0,255,0 };
		ImColor Squad_Box3D_Esp_Color = { 0,0,255 };
		int Box3D_Esp_Thickness = 0;
		bool Box3D_Esp = false;
		float Box3D_Width = 30;

		int ESP_Distance = 50;
		float Text_Size = 0.9f;

		bool Night_Mode = false;
		bool FullBright_Mode = false;

		//visuals
		bool fov_changer = false;
		float fov_value = 100.0f;
		float base_fov_value = 90.0f;
		bool DrawCrosshair = false;
		bool whennotaiming = false;
		ImColor Crosshair_Color = { 255,255,255 };
		float Crosshair_size = 10;
		int Crosshair_thickness = 0;
		bool circle = true;
		bool Cross = true;

		//Player
		bool GodMode = false;
		bool GodMode_LastState = false;
		bool Unlimited_MeleeRange = false;
		bool Unlimited_MeleeRange_LastState = false;
		bool No_Reload = false;
		bool No_Reload_LastState = false;
		bool High_FireRate = false;
		bool High_FireRate_LastState = false;
		bool Show_Magazines = false;

		//World

		//movement
		bool fast_run = false;
		bool super_run = false;
		bool jump = false;

		bool window_animation = true;
		bool navigationwindow_animation = false;
		bool optionswindow_animation = false;
		bool show_watermark = true;
		bool OBSBypass = false;
		bool Show_Fps = false;

		bool Backround_Animation = true;
	};

	inline Settings settings;
};

#define USettings CSettings::settings