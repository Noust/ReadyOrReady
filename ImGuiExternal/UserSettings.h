#pragma once
#include "include.h"

namespace CSettings {
	int MenuWindow = 0;

	struct Settings {
		//esp
		bool esp = false;
		bool Show_Enemy = true;
		bool Show_Squad = true;

		//visuals
		bool fov_changer = false;
		float fov_value = 100.0f;
		float base_fov_value = 90.0f;

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