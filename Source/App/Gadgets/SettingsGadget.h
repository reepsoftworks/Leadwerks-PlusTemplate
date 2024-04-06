#pragma once
#include "../App.h"

namespace App
{
	class SettingsGadget : public Gadget
	{
		// Window
		std::vector<std::string> resolutions;
		GraphicWindowSettings currentwindowsettings;

		std::string GetCurrentResolution();
		GraphicWindowStyles GetCurrentWindowMode();

		// Graphics
		Leadwerks::Setting msaa;
		Leadwerks::Setting light;
		Leadwerks::Setting shadow;
		Leadwerks::Setting tessellation;
		Leadwerks::Setting terrain;
		Leadwerks::Setting water;

		// Camera
		float fov;
		bool hdr;

		void Apply();
		void Reset();

		bool window_opened;
	public:
		SettingsGadget();

		virtual void Hide();

		virtual void DrawUI();
	};
}