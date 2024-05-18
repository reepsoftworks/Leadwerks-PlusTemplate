#include "pch.h"
#include "../App.h"

#include "../Input/ButtonCode.h"
#include "../Input/InputDevice.h"

extern Leadwerks::ConVar mousesmoothing;
extern Leadwerks::ConVar mousesensitivity;
extern Leadwerks::ConVar mouseinvert;

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

		m_mousesmoothing = mousesmoothing.GetFloat();
		m_mousesensitivity = mousesensitivity.GetFloat();
		m_mouseinvert = mouseinvert.GetBool();

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

		SetConVar("mousesmoothing", String(m_mousesmoothing));
		SetConVar("mousesensitivity", String(m_mousesensitivity));
		SetConVar("mouseinvert", String(m_mouseinvert));

		// Update window settings
		auto window = GraphicsWindow::GetCurrent();

		auto t = OS::GetDisplayString(item_current_display);
		auto vec2 = String::Split(t, "x");
		currentwindowsettings.size.x = String::Int(vec2[0]);
		currentwindowsettings.size.y = String::Int(vec2[1]);

		if (window) window->Resize(currentwindowsettings);

		// Save input.
		Input::SaveBindings();

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

	void ButtonAssignment(const char* action, InputSystem::ButtonCode data, InputSystem::InputDevice* device, const std::string& setname = "")
	{
		//static bool waitingforkey = false;
		static std::map<std::string, bool> waitingforkey;
		if (waitingforkey[action] == false)
		{
			if (ImGui::Button(InputSystem::ButtonCodeToString(data).c_str(), ImVec2(120, 20)))
			{
				device->Suspend(true);
				waitingforkey[action] = true;
				GraphicsWindow::GetCurrent()->Flush();
			}
		}
		else if (waitingforkey[action] == true)
		{
			ImGui::Button("...", ImVec2(120, 20));

			if (GraphicsWindow::GetCurrent()->ButtonAnyHit())
			{
				auto key = GraphicsWindow::GetCurrent()->LastButtonPressed();
				if (key > InputSystem::BUTTON_NONE)
				{
					device->SetAction(action, (InputSystem::ButtonCode)key, setname);
					waitingforkey[action] = false;
					GraphicsWindow::GetCurrent()->Flush();
					device->Suspend(false);
				}
			}
		}
	}


	void DrawButtonAssignment(const char* action, InputSystem::InputDevice* device, std::string setname = "")
	{
		ImGui::Text(action);
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - 400);

		if (!device->GetActionData(action, setname).buttons.empty())
		{
			auto data = device->GetActionData(action, setname).buttons[0];
			ButtonAssignment(action, data, device, setname);
		}
		else
		{
			//auto data = device->GetActionData(action, setname).btnaxis;
			//ButtonAssignment(action, data.up, device, setname);
			//ImGui::SameLine();
			//ButtonAssignment(action, data.down, device, setname);
			//ImGui::SameLine();
			//ButtonAssignment(action, data.left, device, setname);
			//ImGui::SameLine();
			//ButtonAssignment(action, data.right, device, setname);
		}
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

				if (ImGui::BeginTabItem("Keyboard"))
				{
					auto device = Input::GetDevice();

					ImGui::Text("Globals");
					ImGui::Separator();

					DrawButtonAssignment("Pause", device);
					DrawButtonAssignment("Console", device);
					DrawButtonAssignment("Screenshot", device);

					ImGui::Text("InGameControls");
					ImGui::Separator();

					DrawButtonAssignment("MoveForward", device, "InGameControls");
					DrawButtonAssignment("MoveBackward", device, "InGameControls");
					DrawButtonAssignment("MoveLeft", device, "InGameControls");
					DrawButtonAssignment("MoveRight", device, "InGameControls");
					DrawButtonAssignment("Crouch", device, "InGameControls");
					DrawButtonAssignment("Jump", device, "InGameControls");

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Mouse"))
				{
					ImGui::SliderFloat("Smoothing", &m_mousesmoothing, 0.0f, 10.0f, "%.0f", 0);
					ImGui::SliderFloat("Sensitivity", &m_mousesensitivity, 0.0f, 10.0f, "%.0f", 0);
					ImGui::Checkbox("Invert Mouse", &m_mouseinvert);

					ImGui::EndTabItem();
				}

				/*
				if (ImGui::BeginTabItem("Sound"))
				{
					ImGui::Text("TODO Sound");
					ImGui::EndTabItem();
				}
				*/

				ImGui::EndTabBar();
			}

			ImGui::EndChild();
		}

		ImVec2 buttonSize(80, 0);
		float widthNeeded = buttonSize.x + ImGui::GetStyle().ItemSpacing.x;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
		bool apply = ImGui::Button("OK", buttonSize);
		if (apply)
		{
			Apply();
		}

		ImGui::End();
	}
}