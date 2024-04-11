#include "pch.h"
#include "../App.h"

namespace App
{
	using namespace Leadwerks;

	SettingsGadget::SettingsGadget()
	{
		auto gfx = OS::GetDisplayModes();
		for (const auto& p : gfx)
		{
			std::string n = String(p.x) + "x" + String(p.y);
			resolutions.push_back(n);
		}

		auto window = GraphicsWindow::GetCurrent();
		if (window)
		{
			currentwindowsettings = window->CurrentSettings();
		}

		msaa = Settings::GetMSAA();
		light = Settings::GetLightQuality();
		shadow = Settings::GetShadowQuality();
		tessellation = Settings::GetTessellationQuality();
		terrain = Settings::GetTerrainQuality();
		water = Settings::GetWaterQuality();
		fov = Settings::GetFov();
		hdr = Settings::GetHDR();

		item_current_display = OS::FindDisplayOption(GetCurrentResolution());

		window_opened = false;
	}

	std::string SettingsGadget::GetCurrentResolution()
	{
		auto size = currentwindowsettings.size;
		return String(size.x) + "x" + String(size.y);
	}

	void SettingsGadget::Apply()
	{
		/*
		Settings::SetMSAA(msaa);
		Settings::SetLightQuality(light);
		Settings::SetShadowQuality(shadow);
		Settings::SetTessellationQuality(tessellation);
		Settings::SetTerrainQuality(terrain);
		Settings::SetWaterQuality(water);
		Settings::SetFov(fov);
		Settings::SetHDR(hdr);
		*/

		// Push settings as convars.
		// The convar will apply the setting for us.
		SetConVar("r_msaa", String(msaa));
		SetConVar("r_lightquality", String(light));
		SetConVar("r_shadowquality", String(shadow));
		SetConVar("r_tessellationquality", String(tessellation));
		SetConVar("r_terrainquality", String(terrain));
		SetConVar("r_waterquality", String(water));
		SetConVar("fov", String(fov));
		SetConVar("r_hdr", String(hdr));

		// Update window settings
		auto window = GraphicsWindow::GetCurrent();

		auto t = OS::GetDisplayString(item_current_display);
		auto vec2 = String::Split(t, "x");
		currentwindowsettings.size.x = String::Int(vec2[0]);
		currentwindowsettings.size.y = String::Int(vec2[1]);

		if (window) window->Resize(currentwindowsettings);

		// Ensure settings are synced!
		Reset();
	}

	void SettingsGadget::Reset()
	{
		msaa = Settings::GetMSAA();
		light = Settings::GetLightQuality();
		shadow = Settings::GetShadowQuality();
		tessellation = Settings::GetTessellationQuality();
		terrain = Settings::GetTerrainQuality();
		water = Settings::GetWaterQuality();
		fov = Settings::GetFov();
		hdr = Settings::GetHDR();
	}

	void SettingsGadget::Hide()
	{
		shown = false;
		if (window_opened) EmitEvent(Event::WidgetClose, this);
		window_opened = false;
		
	}

	GraphicWindowStyles SettingsGadget::GetCurrentWindowMode()
	{
		GraphicWindowStyles ret = GRAPHICSWINDOW_TITLEBAR;
		auto window = GraphicsWindow::GetCurrent();
		if (window)
		{
			auto style = window->CurrentSettings().style;
			ret = style;
		}
		return ret;
	}

	void SettingsGadget::DrawUI()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
		ImVec2 windowsize = ImVec2(550, 400);
		ImGui::SetNextWindowSize(windowsize, ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->GetCenter().x - (windowsize.x / 2), ImGui::GetMainViewport()->GetCenter().y - (windowsize.y / 2)), ImGuiCond_Appearing);

		if (!ImGui::Begin("Settings", &shown, window_flags))
		{
			ImGui::End();
			return;
		}
		else
		{
			if (window_opened == false && shown)
			{
				EmitEvent(Event::WidgetOpen, this);
				window_opened = true;
			}
		}

		// Handle closing
		if (!shown)
		{
			Hide();
			ImGui::End();
			return;
		}

		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		if (ImGui::BeginChild("SettingsMainRegion", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_None | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
			{
				if (ImGui::BeginTabItem("Display"))
				{
					// Resolution
					//static int item_current_display = OS::FindDisplayOption(GetCurrentResolution());
					std::string combo_preview_displayvalue = resolutions[item_current_display];
					if (ImGui::BeginCombo("Resolution", combo_preview_displayvalue.c_str()))
					{
						for (const auto& p : resolutions)
						{
							const bool is_selected = (item_current_display == OS::FindDisplayOption(p));
							if (ImGui::Selectable(p.c_str(), is_selected))
							{
								item_current_display = OS::FindDisplayOption(p);
							}

							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					// Window Mode
					static int item_current_mode = GetCurrentWindowMode();
					std::string combo_preview_modevalue;
					switch (item_current_mode)
					{
					case GRAPHICSWINDOW_TITLEBAR:
						combo_preview_modevalue = "Windowed";
						break;

					case GRAPHICSWINDOW_BORDERLESS:
						combo_preview_modevalue = "Borderless Window";
						break;

					case GRAPHICSWINDOW_FULLSCREEN:
						combo_preview_modevalue = "Fullscreen";
						break;

					case GRAPHICSWINDOW_FULLSCREENNATIVE:
						combo_preview_modevalue = "Native Fullscreen";
						break;

					case GRAPHICSWINDOW_FULLSCREENBORDERLESS:
						combo_preview_modevalue = "Fullscreen Borderless Window";
						break;
					}

					const char* modeitems[] = { "Windowed", "Borderless Window", "Fullscreen", "Native Fullscreen", "Fullscreen Borderless Window" };
					if (ImGui::BeginCombo("Window Mode", combo_preview_modevalue.c_str()))
					{
						for (int n = 0; n < IM_ARRAYSIZE(modeitems); n++)
						{
							const bool is_selected = (item_current_mode == n);
							if (ImGui::Selectable(modeitems[n], is_selected))
								item_current_mode = n;

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					if (item_current_mode != currentwindowsettings.style) currentwindowsettings.style = (GraphicWindowStyles)item_current_mode;

					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Graphics"))
				{
					// MSAA
					std::string combo_preview = Settings::GetSettingString(msaa);
					if (ImGui::BeginCombo("Multisampling", combo_preview.c_str()))
					{
						const char* msaaitems[] = { "Low", "Medium", "High" };
						for (int n = 0; n < IM_ARRAYSIZE(msaaitems); n++)
						{
							const bool is_selected = (msaa == (Setting)n);
							if (ImGui::Selectable(msaaitems[n], is_selected))
								msaa = (Setting)n;

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					// Lights
					const char* graphicitems[] = { "Low", "Medium", "High" };
					combo_preview = Settings::GetSettingString(light);
					if (ImGui::BeginCombo("Light Quality", combo_preview.c_str()))
					{
						for (int n = 0; n < IM_ARRAYSIZE(graphicitems); n++)
						{
							const bool is_selected = (light == (Setting)n);
							if (ImGui::Selectable(graphicitems[n], is_selected))
								light = (Setting)n;

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					// Shadows
					combo_preview = Settings::GetSettingString(shadow);
					if (ImGui::BeginCombo("Shadow Quality", combo_preview.c_str()))
					{
						for (int n = 0; n < IM_ARRAYSIZE(graphicitems); n++)
						{
							const bool is_selected = (shadow == (Setting)n);
							if (ImGui::Selectable(graphicitems[n], is_selected))
								shadow = (Setting)n;

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					// Tessellation
					combo_preview = Settings::GetSettingString(tessellation);
					if (ImGui::BeginCombo("Tessellation Quality", combo_preview.c_str()))
					{
						for (int n = 0; n < IM_ARRAYSIZE(graphicitems); n++)
						{
							const bool is_selected = (tessellation == (Setting)n);
							if (ImGui::Selectable(graphicitems[n], is_selected))
								tessellation = (Setting)n;

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					// Terrain
					combo_preview = Settings::GetSettingString(terrain);
					if (ImGui::BeginCombo("Terrain Quality", combo_preview.c_str()))
					{
						for (int n = 0; n < IM_ARRAYSIZE(graphicitems); n++)
						{
							const bool is_selected = (terrain == (Setting)n);
							if (ImGui::Selectable(graphicitems[n], is_selected))
								terrain = (Setting)n;

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					// Water
					combo_preview = Settings::GetSettingString(water);
					if (ImGui::BeginCombo("Water Quality", combo_preview.c_str()))
					{
						for (int n = 0; n < IM_ARRAYSIZE(graphicitems); n++)
						{
							const bool is_selected = (water == (Setting)n);
							if (ImGui::Selectable(graphicitems[n], is_selected))
								water = (Setting)n;

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					// Camera
					ImGui::SliderFloat("Field of View (FOV)", &fov, 50.0f, 90.0f, "%.0f", 0);
					ImGui::Checkbox("HDR", &hdr);

					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Controls"))
				{
					ImGui::Text("TODO Controls");
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Sound"))
				{
					ImGui::Text("TODO Sound");
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			ImGui::EndChild();
		}


		/*
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		if (ImGui::BeginChild("SettingsGraphicsRegion", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_None | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::SetNextItemOpen(true);
			if (ImGui::TreeNode("Display"))
			{
				// Resolution
				static int item_current_display = OS::FindDisplayOption(GetCurrentResolution());
				std::string combo_preview_displayvalue = resolutions[item_current_display];
				if (ImGui::BeginCombo("Resolution", combo_preview_displayvalue.c_str()))
				{
					for (const auto& p : resolutions)
					{
						const bool is_selected = (item_current_display == OS::FindDisplayOption(p));
						if (ImGui::Selectable(p.c_str(), is_selected))
						{
							item_current_display = OS::FindDisplayOption(p);
						}

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (item_current_display != OS::FindDisplayOption(String(currentwindowsettings.size.x) + "x" + String(currentwindowsettings.size.y)))
				{
					std::string s = OS::GetDisplayString(item_current_display);
					auto vec2 = String::Split(s, "x");
					currentwindowsettings.size = iVec2(String::Int(vec2[0]), String::Int(vec2[1]));
				}

				// Window Mode
				static int item_current_mode = GetCurrentWindowMode();
				std::string combo_preview_modevalue;
				switch (item_current_mode)
				{
				case GRAPHICSWINDOW_TITLEBAR:
					combo_preview_modevalue = "Windowed";
					break;

				case GRAPHICSWINDOW_BORDERLESS:
					combo_preview_modevalue = "Borderless Window";
					break;

				case GRAPHICSWINDOW_FULLSCREEN:
					combo_preview_modevalue = "Fullscreen";
					break;

				case GRAPHICSWINDOW_FULLSCREENNATIVE:
					combo_preview_modevalue = "Native Fullscreen";
					break;

				case GRAPHICSWINDOW_FULLSCREENBORDERLESS:
					combo_preview_modevalue = "Fullscreen Borderless Window";
					break;
				}

				const char* modeitems[] = { "Windowed", "Borderless Window", "Fullscreen", "Native Fullscreen", "Fullscreen Borderless Window" };
				if (ImGui::BeginCombo("Window Mode", combo_preview_modevalue.c_str()))
				{
					for (int n = 0; n < IM_ARRAYSIZE(modeitems); n++)
					{
						const bool is_selected = (item_current_mode == n);
						if (ImGui::Selectable(modeitems[n], is_selected))
							item_current_mode = n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (item_current_mode != currentwindowsettings.style) currentwindowsettings.style = (GraphicWindowStyles)item_current_mode;

				ImGui::TreePop();
			}

			// Graphics
			ImGui::SetNextItemOpen(true);
			if (ImGui::TreeNode("Graphics"))
			{
				std::string combo_preview = Settings::GetSettingString(msaa);
				if (ImGui::BeginCombo("Multisampling", combo_preview.c_str()))
				{
					const char* msaaitems[] = { "Low", "Medium", "High" };
					for (int n = 0; n < IM_ARRAYSIZE(msaaitems); n++)
					{
						const bool is_selected = (msaa == (Setting)n);
						if (ImGui::Selectable(msaaitems[n], is_selected))
							msaa = (Setting)n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				// Lights
				const char* graphicitems[] = { "Low", "Medium", "High" };
				combo_preview = Settings::GetSettingString(light);
				if (ImGui::BeginCombo("Light Quality", combo_preview.c_str()))
				{
					for (int n = 0; n < IM_ARRAYSIZE(graphicitems); n++)
					{
						const bool is_selected = (light == (Setting)n);
						if (ImGui::Selectable(graphicitems[n], is_selected))
							light = (Setting)n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				// Shadows
				combo_preview = Settings::GetSettingString(shadow);
				if (ImGui::BeginCombo("Shadow Quality", combo_preview.c_str()))
				{
					for (int n = 0; n < IM_ARRAYSIZE(graphicitems); n++)
					{
						const bool is_selected = (shadow == (Setting)n);
						if (ImGui::Selectable(graphicitems[n], is_selected))
							shadow = (Setting)n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				// Tessellation
				combo_preview = Settings::GetSettingString(tessellation);
				if (ImGui::BeginCombo("Tessellation Quality", combo_preview.c_str()))
				{
					for (int n = 0; n < IM_ARRAYSIZE(graphicitems); n++)
					{
						const bool is_selected = (tessellation == (Setting)n);
						if (ImGui::Selectable(graphicitems[n], is_selected))
							tessellation = (Setting)n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				// Terrain
				combo_preview = Settings::GetSettingString(terrain);
				if (ImGui::BeginCombo("Terrain Quality", combo_preview.c_str()))
				{
					for (int n = 0; n < IM_ARRAYSIZE(graphicitems); n++)
					{
						const bool is_selected = (terrain == (Setting)n);
						if (ImGui::Selectable(graphicitems[n], is_selected))
							terrain = (Setting)n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				// Water
				combo_preview = Settings::GetSettingString(water);
				if (ImGui::BeginCombo("Water Quality", combo_preview.c_str()))
				{
					for (int n = 0; n < IM_ARRAYSIZE(graphicitems); n++)
					{
						const bool is_selected = (water == (Setting)n);
						if (ImGui::Selectable(graphicitems[n], is_selected))
							water = (Setting)n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				ImGui::TreePop();
			}

			// Camera
			ImGui::SetNextItemOpen(true);
			if (ImGui::TreeNode("Camera"))
			{
				ImGui::SliderFloat("Field of View (FOV)", &fov, 50.0f, 90.0f, "%.0f", 0);
				ImGui::Checkbox("HDR", &hdr);

				ImGui::TreePop();
			}
			ImGui::EndChild();
		}
		*/

		ImVec2 buttonSize(80, 0);
		float widthNeeded = buttonSize.x + ImGui::GetStyle().ItemSpacing.x;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
		bool apply = ImGui::Button("Apply", buttonSize);
		if (apply)
		{
			Apply();
		}

		ImGui::End();
	}
}