#include "pch.h"
#include "KBMDevice.h"

namespace App
{
	using namespace Leadwerks;

	namespace InputSystem
	{
		KBMDevice::KBMDevice()
		{
			index = 0;
			suspend = false;
			devicename = "keyboard/mouse";
			activeset = "";
			actionsets.clear();
		}

		void KBMDevice::Initialize()
		{
			Print("Initialized " + devicename);

			// We need to cycle through the config to register the actionsets.
			if (!config.empty())
			{
				for (const auto& p : config)
				{
					std::string key = p.first;
					if (config[key].is_object())
					{
						if (key == "mousecursor") continue;
						if (key == "axes") continue;
						if (key == "settings") continue;
						RegisterActionSet(key);
					}
				}
			}

			// Poll to see if any controllers are conected!
			Update();
		}

		void KBMDevice::SetActiveSet(const std::string& setname)
		{
			if (setname != activeset)
			{
				window->Flush();

				if (GetDeviceID() == INPUTDEVICE_KBM)
				{
					if (!setname.empty())
					{
						if (config[setname]["mousecursor"].is_integer())
						{
							bool m = config[setname]["mousecursor"];
							bool currentcursor = m;
							window->SetCursor(currentcursor);
						}
						else
						{
							// If none exist, resort to default.
							bool currentcursor = true;
							window->SetCursor(currentcursor);
						}
					}
					else
					{
						// If the set is global (blank) use the default cursor.
						int m = config["mousecursor"];
						bool currentcursor = m;
						window->SetCursor(currentcursor);
					}
				}
				else
				{
					// Always hide the cursor if the gamepad is active.
					window->SetCursor(false);
					CenterCursor();
				}

				activeset = setname;
				//if (controller) controller->SetActiveSet(activeset);
				EmitEvent(EVENT_ACTIONSET, this, 0, 0, 0, 0, 0, 0);
			}
		}

		bool KBMDevice::Hit(const std::string& actionname)
		{
			bool ret = false;

			if (suspend) return ret;

			// Setless Actions
			if (config[actionname].is_integer())
			{
				int button = config[actionname];
				ret = window->ButtonHit(button);
			}
			else if (config[actionname].is_array())
			{
				for (std::size_t n = 0; n < config[actionname].size(); ++n)
				{
					int button = config[actionname][n];
					ret = window->ButtonHit(button);

					if (ret) break;
				}
			}

			// Actions within Sets
			if (!activeset.empty() && !ret)
			{
				if (config[activeset][actionname].is_integer())
				{
					int button = config[activeset][actionname];
					ret = window->ButtonHit(button);
				}
				else if (config[activeset][actionname].is_array())
				{
					for (std::size_t n = 0; n < config[activeset][actionname].size(); ++n)
					{
						int button = config[actionname][n];
						ret = window->ButtonHit((ButtonCode)button);

						if (ret) break;
					}
				}
			}

			if (ret) SetDevice(INPUTDEVICE_KBM);
			return ret;
		}

		bool KBMDevice::Down(const std::string& actionname)
		{
			bool ret = false;

			if (suspend) return false;

			// Setless Actions
			if (config[actionname].is_integer())
			{
				int button = config[actionname];
				ret = window->ButtonDown((ButtonCode)button);
			}
			else if (config[actionname].is_array())
			{
				for (std::size_t n = 0; n < config[actionname].size(); ++n)
				{
					int button = config[actionname][n];
					ret = window->ButtonDown((ButtonCode)button);

					if (ret) break;
				}
			}

			// Actions within Sets
			if (!activeset.empty() && !ret)
			{
				if (config[activeset][actionname].is_integer())
				{
					int button = config[activeset][actionname];
					ret = window->ButtonDown((ButtonCode)button);
				}
				else if (config[activeset][actionname].is_array())
				{
					for (std::size_t n = 0; n < config[activeset][actionname].size(); ++n)
					{
						int button = config[actionname][n];
						ret = window->ButtonDown((ButtonCode)button);

						if (ret) break;
					}
				}
			}

			if (ret) SetDevice(INPUTDEVICE_KBM);
			return ret;
		}

		Vec2 KBMDevice::ButtonAxis(const ButtonCode& up, const ButtonCode& down, const ButtonCode& left, const ButtonCode& right)
		{
			const bool bUp = window->ButtonDown(up);
			const bool bDwn = window->ButtonDown(down);
			const bool bLft = window->ButtonDown(left);
			const bool bRt = window->ButtonDown(right);
			return Vec2(static_cast<float>(bRt - bLft), static_cast<float>(bUp - bDwn));
		}

		Vec2 KBMDevice::GetAxis(const AxisCode axis)
		{
			Vec2 ret = Vec2(0);
			if (window == NULL) return ret;
			auto mouseaxis = window->GetMouseAxis();
			if (axis == AXIS_MOUSE)
			{
				ret = Vec2(mouseaxis.x, mouseaxis.y);
			}
			else if (axis == AXIS_MOUSEINVERT)
			{
				ret = Vec2(mouseaxis.x, (mouseaxis.y * -1));
			}

			return ret;
		}

		Vec2 KBMDevice::Axis(const std::string& actionname)
		{
			// Only tie device 0 to the main controller.
			Vec2 ret = Vec2(0,0);

			if (suspend) return ret;

			// Setless Actions
			if (config["axes"][actionname].is_integer())
			{
				int code = config["axes"][actionname];
				ret = GetAxis((AxisCode)code);
			}
			else if (config["axes"][actionname].is_array() && config["axes"][actionname].size() == 4)
			{
				auto v = config["axes"][actionname];
				int up = v[0];
				int down = v[1];
				int left = v[2];
				int right = v[3];
				ret = ButtonAxis((ButtonCode)up, (ButtonCode)down, (ButtonCode)left, (ButtonCode)right);
			}

			// Actions within Sets
			if (!activeset.empty() && (ret.x == 0 && ret.y == 0))
			{
				if (config[activeset]["axes"][actionname].is_integer())
				{
					int code = config[activeset]["axes"][actionname];
					ret = GetAxis((AxisCode)code);
				}
				else if (config[activeset]["axes"][actionname].is_array() && config[activeset]["axes"][actionname].size() == 4)
				{
					auto v = config[activeset]["axes"][actionname];
					int up = v[0];
					int down = v[1];
					int left = v[2];
					int right = v[3];
					ret = ButtonAxis((ButtonCode)up, (ButtonCode)down, (ButtonCode)left, (ButtonCode)right);
				}
			}

			if (ret.x != 0.0f || ret.y != 0.0f) SetDevice(INPUTDEVICE_KBM);
			return ret;
		}

		void KBMDevice::SetCursorPosition(const int x, const int y)
		{
			window->SetMousePosition(x, y);
		}

		void KBMDevice::SetCursorPosition(const iVec2 xy)
		{
			window->SetMousePosition(xy.x, xy.y);
		}

		void KBMDevice::CenterCursor()
		{
			if (window != NULL)
			{
				auto center = iVec2(window->GetSize().x / 2, window->GetSize().y / 2);
				SetCursorPosition(center.x, center.y);
			}
		}

		iVec2 KBMDevice::GetCursorPosition()
		{
			auto v = window->GetMousePosition();
			return iVec2((int)v.x, (int)v.y);
		}

		Vec2 KBMDevice::GetCursorFloatPosition()
		{
			return window->GetMousePosition();
		}

		void KBMDevice::SetActionSetCursor(const std::string& setname, bool cursor)
		{
			if (!setname.empty())
			{
				RegisterActionSet(setname);
				config[setname]["mousecursor"] = cursor;
			}
			else
			{
				config["mousecursor"] = cursor;
			}
		}

		bool KBMDevice::GetActionSetCursor(const std::string& setname)
		{
			bool cursor = true;
			if (!setname.empty())
			{
				cursor = config[setname]["mousecursor"];
			}
			else
			{
				cursor = config["mousecursor"];
			}

			return cursor;
		}

		KBMDevice* KBMDevice::Create()
		{
			auto controller = new KBMDevice();
			controller->index = 0;
			controller->window = GraphicsWindow::GetCurrent();
			controller->Initialize();
			return controller;
		}
	}
}