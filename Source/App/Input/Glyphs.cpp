#include "pch.h"
#include "Glyphs.h"

namespace App
{
	using namespace Leadwerks;
	namespace InputSystem
	{
		table glyphconfig;
		bool loadedimages = false;
		std::map<std::string, Texture*> buttonglyphs;
		std::map<std::string, Texture*> axisglyphs;

		Texture* GetButtonGlyph(const ButtonCode code)
		{
			Texture* ret = NULL;
			if (glyphconfig.empty() || glyphconfig.is_null()) return ret;
			if (!glyphconfig["buttons"].empty() && !glyphconfig["buttons"].is_null())
			{
				auto str = ButtonCodeToString(code);
				if (code >= BUTTON_FIRST_KEY && code <= BUTTON_LAST_KEY)
				{
					// Longer names need a bigger key image.
					if (str.length() > 3)
					{
						ret = buttonglyphs["KeyRect"];
					}
					else
					{
						ret = buttonglyphs["Key"];
					}
				}
				else
				{
					ret = buttonglyphs[str];
				}
			}
			return ret;
		}

		Texture* GetAxisGlyph(const AxisCode code)
		{
			if (glyphconfig.empty() || glyphconfig.is_null()) return NULL;
			if (!glyphconfig["axes"].empty() && !glyphconfig["axes"].is_null())
			{
				auto str = AxisCodeToString(code);
				return axisglyphs[str];
			}
			return NULL;
		}

		bool LoadGlyphPixmaps(table data)
		{
			if (loadedimages) return true;
			if (data.empty() || data.is_null()) return false;

			std::vector<std::string> glyphentries;
			int flags = 0; //LOAD_QUIET;
			if (!data["buttons"].empty() && !data["buttons"].is_null())
			{
				// Keys
				glyphentries.push_back("Key");
				glyphentries.push_back("KeyRect");

				// Mouse
				for (int i = BUTTON_MOUSE_LBUTTON; i < BUTTON_FIRST_KEY; i++)
				{
					auto k = ButtonCodeToString((ButtonCode)i);
					glyphentries.push_back(k);
				}

				for (int i = BUTTON_MOUSE_WHEELUP; i < BUTTON_MOUSE_WHEELRIGHT + 1; i++)
				{
					auto k = ButtonCodeToString((ButtonCode)i);
					glyphentries.push_back(k);
				}

				// Gamepad
				for (int i = BUTTON_GAMEPAD_DPADUP; i < BUTTON_GAMEPAD_B + 1; i++)
				{
					auto k = ButtonCodeToString((ButtonCode)i);
					glyphentries.push_back(k);
				}

				// Load images as pixmaps.
				for (const auto& e : glyphentries)
				{
					if (data["buttons"][e].is_string())
					{
						auto icon = std::string(data["buttons"][e]);
						buttonglyphs[e] = AssetLoader::LoadTexture(icon, flags); //LoadPixmap(icon, flags);
					}
				}

				glyphentries.clear();
			}


			// Do the same for Axis
			if (!data["axes"].empty() && !data["axes"].is_null())
			{
				for (int i = AXIS_MOUSE; i <= AXIS_LAST; i++)
				{
					auto k = AxisCodeToString((AxisCode)i);
					glyphentries.push_back(k);
				}

				// Load images as pixmaps.
				for (const auto& e : glyphentries)
				{
					if (data["axes"][e].is_string())
					{
						auto icon = std::string(data["axes"][e]);
						axisglyphs[e] = AssetLoader::LoadTexture(icon, flags); //LoadPixmap(icon, flags);
					}
				}

				glyphentries.clear();
			}

			loadedimages = (!buttonglyphs.empty() || !axisglyphs.empty());
			return loadedimages;
		}

		bool InitGlyphs(const std::string& filepath)
		{
			if (!glyphconfig.empty() || !glyphconfig.is_null()) return true;

			glyphconfig = {};
			glyphconfig["buttons"] = {};
			glyphconfig["axes"] = {};

			// =================================
			// Button Glyphs
			// =================================
			auto button = glyphconfig["buttons"];

			// Keys
			button["Key"] = "UI/Icons/Glyphs/shared_key.png"; // TODO: Missing
			button["KeyRect"] = "UI/Icons/Glyphs/shared_keyrect.png"; // TODO: Missing

			// Mouse
			button[ButtonCodeToString(BUTTON_MOUSE_LBUTTON)] = "UI/Icons/Glyphs/shared_mouse_l_click.png";
			button[ButtonCodeToString(BUTTON_MOUSE_RBUTTON)] = "UI/Icons/Glyphs/shared_mouse_r_click.png";
			button[ButtonCodeToString(BUTTON_MOUSE_MBUTTON)] = "UI/Icons/Glyphs/shared_mouse_mid_click.png";
			button[ButtonCodeToString(BUTTON_MOUSE_XBUTTON1)] = "UI/Icons/Glyphs/shared_mouse_4.png";
			button[ButtonCodeToString(BUTTON_MOUSE_XBUTTON2)] = "UI/Icons/Glyphs/shared_mouse_5.png";
			button[ButtonCodeToString(BUTTON_MOUSE_WHEELUP)] = "UI/Icons/Glyphs/shared_mouse_scroll_up.png";
			button[ButtonCodeToString(BUTTON_MOUSE_WHEELDOWN)] = "UI/Icons/Glyphs/shared_mouse_scroll_down.png";
			button[ButtonCodeToString(BUTTON_MOUSE_WHEELLEFT)] = "UI/Icons/Glyphs/shared_mouse_4.png";
			button[ButtonCodeToString(BUTTON_MOUSE_WHEELRIGHT)] = "UI/Icons/Glyphs/shared_mouse_5.png";

			// Gamepad
			button[ButtonCodeToString(BUTTON_GAMEPAD_DPADUP)] = "UI/Icons/Glyphs/shared_dpad_up.png";
			button[ButtonCodeToString(BUTTON_GAMEPAD_DPADDOWN)] = "UI/Icons/Glyphs/shared_dpad_down.png";
			button[ButtonCodeToString(BUTTON_GAMEPAD_DPADLEFT)] = "UI/Icons/Glyphs/shared_dpad_left.png";
			button[ButtonCodeToString(BUTTON_GAMEPAD_DPADRIGHT)] = "UI/Icons/Glyphsshared_dpad_right.png";
			button[ButtonCodeToString(BUTTON_GAMEPAD_START)] = "UI/Icons/Glyphs/xbox_button_start.png";
			button[ButtonCodeToString(BUTTON_GAMEPAD_BACK)] = "UI/Icons/Glyphs/xbox_button_select.png";
			button[ButtonCodeToString(BUTTON_GAMEPAD_LTHUMB)] = "UI/Icons/Glyphs/shared_lstick_click.png";
			button[ButtonCodeToString(BUTTON_GAMEPAD_RTHUMB)] = "UI/Icons/Glyphs/shared_rstick_click.png";
			button[ButtonCodeToString(BUTTON_GAMEPAD_LSHOULDER)] = "UI/Icons/Glyphs/xbox_lb.png";
			button[ButtonCodeToString(BUTTON_GAMEPAD_RSHOULDER)] = "UI/Icons/Glyphs/xbox_rb.png";
			button[ButtonCodeToString(BUTTON_GAMEPAD_RTRIGGER)] = "UI/Icons/Glyphs/xbox_rt.png";
			button[ButtonCodeToString(BUTTON_GAMEPAD_LTRIGGER)] = "UI/Icons/Glyphs/xbox_lt.png";
			button[ButtonCodeToString(BUTTON_GAMEPAD_X)] = "UI/Icons/Glyphs/shared_color_button_x.png";
			button[ButtonCodeToString(BUTTON_GAMEPAD_Y)] = "UI/Icons/Glyphs/shared_color_button_y.png";
			button[ButtonCodeToString(BUTTON_GAMEPAD_A)] = "UI/Icons/Glyphs/shared_color_button_a.png";
			button[ButtonCodeToString(BUTTON_GAMEPAD_B)] = "UI/Icons/Glyphs/shared_color_button_b.png";

			// No VR button glyphs...

			// =================================
			// Axis Glyphs
			// =================================
			auto axes = glyphconfig["axes"];

			// Mouse
			axes[AxisCodeToString(AXIS_MOUSE)] = "UI/Icons/Glyphs/shared_mouse_move.png"; // TODO: Missing
			axes[AxisCodeToString(AXIS_MOUSEINVERT)] = "UI/Icons/Glyphs/shared_mouse_move.png"; // TODO: Missing

			// Gamepad
			axes[AxisCodeToString(AXIS_RTRIGGER)] = "UI/Icons/Glyphs/xbox_rt_soft.png";
			axes[AxisCodeToString(AXIS_LTRIGGER)] = "UI/Icons/Glyphs/xbox_lt_soft.png";
			axes[AxisCodeToString(AXIS_RSTICK)] = "UI/Icons/Glyphs/shared_rstick.png";
			axes[AxisCodeToString(AXIS_LSTICK)] = "UI/Icons/Glyphs/shared_lstick.png";

			// No VR axes glyphs...

			// Save the table.
			Table::Save(glyphconfig, filepath);

			// Load the images to pixmaps
			return LoadGlyphPixmaps(glyphconfig);
		}

		bool LoadGlyphs(const std::string& filepath)
		{
			if (FileSystem::GetFileType(filepath) == 1)
			{
				glyphconfig.clear();
				glyphconfig = Table::Load(filepath);
			}

			// If the file is bad, override it with the default one!
			if (glyphconfig.empty() || glyphconfig.is_null()) return InitGlyphs(filepath);

			// Load the images to pixmaps
			return LoadGlyphPixmaps(glyphconfig);
		}
	}
}