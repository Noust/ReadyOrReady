#pragma once
#include "include.h"

namespace CSettings {
	int MenuWindow = 0;

	struct Settings {
		//esp
		bool esp = false;
		bool Show_Enemy = true;
		bool Show_Squad = true;


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