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

		// Mouse Settings
		float m_mousesmoothing;
		float m_mousesensitivity;
		bool m_mouseinvert;

		void Apply();
		void Reset();

		int item_current_display;
		bool window_opened;
	public:
		SettingsGadget();

		virtual void Hide();

		virtual void DrawUI();
	};
}