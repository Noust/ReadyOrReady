#pragma once
#include "include.h"

namespace RonSettings {
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

		//gun
		bool Aimbot = false;
		float AimFov = 200;
		double Smooth = 0.7;
		bool ShowFov = false;
		int FovThickness = 0;
		ImColor FovColor = { 255,255,255 };
		bool ShowTarget = false;
		int TargetThickness = 0;
		ImColor TargetColor = { 255,0,0 };
		bool FilledCircle = false;
		ImColor FilledCircleColor = { 0,0,0,80 };
		int AimBotHotKey = 0;
		int HotKey = VK_RBUTTON;
		std::vector<int> HotKeyList{ VK_LBUTTON, VK_LMENU, VK_RBUTTON, VK_XBUTTON1, VK_XBUTTON2, VK_CAPITAL, VK_LSHIFT, VK_LCONTROL };// added new button VK_LBUTTON

		inline void SetHotKey(int Index)
		{
			HotKey = HotKeyList.at(Index);
		}
		bool No_Reload = false;
		bool No_Reload_LastState = false;
		bool High_FireRate = false;
		bool High_FireRate_LastState = false;
		bool Show_Magazines = false;

		//World
		bool Show_actors = false;

		//movement
		bool fast_run = false;
		bool fast_run_LastState = false;
		bool super_run = false;
		bool super_run_LastState = false;
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

#define USettings RonSettings::settings

void disable_all() {
	USettings.esp = false;

	USettings.Show_CurrentHealth = false;
	USettings.Show_headHealth = false;
	USettings.Show_leftArmHealth = false;
	USettings.Show_leftLegHealth = false;
	USettings.Show_rightArmHealth = false;
	USettings.Show_rightLegHealth = false;
	USettings.Show_Health = false;

	USettings.Box_ESP = false;

	USettings.CornerBox_ESP = false;

	USettings.Name_ESP = false;

	USettings.Type_ESP = false;

	USettings.Distance_Esp = false;

	USettings.Status_Esp = false;

	USettings.SnaplLine_Esp_End_Point = false;
	USettings.SnaplLine_Esp = false;

	USettings.FilledBox_Esp = false;

	USettings.Show_GunAmmo = false;
	USettings.Show_GunAmmoType = false;
	USettings.GunName_Esp = false;

	USettings.Box3D_Esp = false;


	USettings.Night_Mode = false;
	USettings.FullBright_Mode = false;

	//visuals
	USettings.fov_changer = false;
	USettings.DrawCrosshair = false;
	USettings.whennotaiming = false;

	//Player
	USettings.GodMode = false;
	USettings.GodMode_LastState = false;
	USettings.Unlimited_MeleeRange = false;
	USettings.Unlimited_MeleeRange_LastState = false;
	USettings.No_Reload = false;
	USettings.No_Reload_LastState = false;
	USettings.High_FireRate = false;
	USettings.High_FireRate_LastState = false;
	USettings.Show_Magazines = false;

	USettings.Aimbot = false;
	USettings.ShowFov = false;
	USettings.ShowTarget = false;
	USettings.FilledCircle = false;

	//World
	USettings.Show_actors = false;

	//movement
	USettings.fast_run = false;
	USettings.super_run = false;
	USettings.jump = false;

	USettings.navigationwindow_animation = false;
	USettings.optionswindow_animation = false;
	USettings.OBSBypass = false;
	USettings.Show_Fps = false;
}

void default_all() {
	USettings.Aimbot = false;
	USettings.AimFov = 200;
	USettings.Smooth = 0.7;
	USettings.ShowFov = false;
	USettings.FovThickness = 0;
	USettings.FovColor = { 255,255,255 };
	USettings.ShowTarget = false;
	USettings.TargetThickness = 0;
	USettings.TargetColor = { 255,0,0 };
	USettings.FilledCircle = false;
	USettings.FilledCircleColor = { 0,0,0,80 };
	USettings.AimBotHotKey = 0;
	USettings.HotKey = VK_RBUTTON;
	USettings.esp = false;
	USettings.Show_Enemy = true;
	USettings.Show_Civilian = true;
	USettings.Show_Squad = true;

	USettings.HealthBar_Esp_Thickness = 2;
	USettings.HealthBar_ESP = true;
	USettings.Show_CurrentHealth = false;
	USettings.Show_headHealth = false;
	USettings.Show_leftArmHealth = false;
	USettings.Show_leftLegHealth = false;
	USettings.Show_rightArmHealth = false;
	USettings.Show_rightLegHealth = false;
	USettings.Enemy_Health_Esp_Color = { 255,0,0 };
	USettings.Civilian_Health_Esp_Color = { 0,255,0 };
	USettings.Squad_Health_Esp_Color = { 0,0,255 };
	USettings.Show_Health = false;

	USettings.Enemy_Box_Esp_Color = { 255,0,0 };
	USettings.Civilian_Box_Esp_Color = { 0,255,0 };
	USettings.Squad_Box_Esp_Color = { 0,0,255 };
	USettings.Box_Esp_Thickness = 0;
	USettings.Box_ESP = false;

	USettings.Enemy_CornerBox_Esp_Color = { 255,0,0 };
	USettings.Civilian_CornerBox_Esp_Color = { 0,255,0 };
	USettings.Squad_CornerBox_Esp_Color = { 0,0,255 };
	USettings.Box_CornerEsp_Thickness = 0;
	USettings.CornerBox_ESP = false;

	USettings.Squad_Name_ESP_Color = { 255,255,255 };
	USettings.Name_ESP = false;

	USettings.Squad_Type_ESP_Color = { 0,0,255 };
	USettings.Enemy_Type_ESP_Color = { 255,0,0 };
	USettings.Civilian_Type_ESP_Color = { 0,255,0 };
	USettings.Type_ESP = false;

	USettings.Enemy_Distance_Esp_Color = { 255,0,0 };
	USettings.Civilian_Distance_Esp_Color = { 0,255,0 };
	USettings.Squad_Distance_Esp_Color = { 0,0,255 };
	USettings.Distance_Esp = false;

	USettings.Enemy_Status_Esp_Color = { 255,0,0 };
	USettings.Civilian_Status_Esp_Color = { 0,255,0 };
	USettings.Status_Esp = false;

	USettings.SnaplLine_Esp_Start_Point = { widthscreen / 2,heightscreen };
	USettings.SnaplLine_Esp_End_Point = false;
	USettings.Enemy_SnaplLine_Esp_Color = { 255,0,0 };
	USettings.Civilian_SnaplLine_Esp_Color = { 0,255,0 };
	USettings.Squad_SnaplLine_Esp_Color = { 0,0,255 };
	USettings.SnaplLine_Esp_Thickness = 0;
	USettings.SnaplLine_Esp = false;

	USettings.Enemy_FilledBox_Esp_Color = { 0,0,0,70 };
	USettings.Civilian_FilledBox_Esp_Color = { 0,0,0,70 };
	USettings.Squad_FilledBox_Esp_Color = { 0,0,0,70 };
	USettings.FilledBox_Esp = false;

	USettings.Enemy_GunName_Color = { 255,0,0 };
	USettings.Squad_GunName_Color = { 0,0,255 };
	USettings.Show_GunAmmo = false;
	USettings.Show_GunAmmoType = false;
	USettings.GunName_Esp = false;

	USettings.Enemy_Box3D_Esp_Color = { 255,0,0 };
	USettings.Civilian_Box3D_Esp_Color = { 0,255,0 };
	USettings.Squad_Box3D_Esp_Color = { 0,0,255 };
	USettings.Box3D_Esp_Thickness = 0;
	USettings.Box3D_Esp = false;
	USettings.Box3D_Width = 30;

	USettings.ESP_Distance = 50;
	USettings.Text_Size = 0.9f;

	USettings.Night_Mode = false;
	USettings.FullBright_Mode = false;

	//visuals
	USettings.fov_changer = false;
	USettings.fov_value = 100.0f;
	USettings.base_fov_value = 90.0f;
	USettings.DrawCrosshair = false;
	USettings.whennotaiming = false;
	USettings.Crosshair_Color = { 255,255,255 };
	USettings.Crosshair_size = 10;
	USettings.Crosshair_thickness = 0;
	USettings.circle = true;
	USettings.Cross = true;

	//Player
	USettings.GodMode = false;
	USettings.GodMode_LastState = false;
	USettings.Unlimited_MeleeRange = false;
	USettings.Unlimited_MeleeRange_LastState = false;
	USettings.No_Reload = false;
	USettings.No_Reload_LastState = false;
	USettings.High_FireRate = false;
	USettings.High_FireRate_LastState = false;
	USettings.Show_Magazines = false;

	//World
	USettings.Show_actors = false;

	//movement
	USettings.fast_run = false;
	USettings.super_run = false;
	USettings.jump = false;

	USettings.window_animation = true;
	USettings.navigationwindow_animation = false;
	USettings.optionswindow_animation = false;
	USettings.show_watermark = true;
	USettings.OBSBypass = false;
	USettings.Show_Fps = false;

	USettings.Backround_Animation = true;
}