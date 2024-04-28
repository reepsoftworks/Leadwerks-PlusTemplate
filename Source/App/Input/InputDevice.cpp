#include "pch.h"
#include "InputDevice.h"
#include "Glyphs.h"

namespace App
{
	using namespace Leadwerks;

	namespace InputSystem
	{
		int activedeviceid = 0;
		void SetDevice(const int type)
		{
			if (activedeviceid != type)
			{
				activedeviceid = type;
				EmitEvent(EVENT_DEVICE, GraphicsWindow::GetCurrent(), activedeviceid);
			}
		}

		const int GetDeviceID()
		{
			return activedeviceid;
		}

		InputDevice::InputDevice()
		{
			index = 0;
			suspend = false;
			devicename = "InputDevice";
			activeset = "";
			actionsets.clear();
		}

		void InputDevice::RegisterActionSet(const std::string& setname)
		{
			if (!setname.empty())
			{
				auto exists = std::find(actionsets.begin(), actionsets.end(), setname) != actionsets.end();
				if (!exists) actionsets.push_back(setname);
			}
		}

		const bool InputDevice::IsRegisteredActionSet(const std::string& setname)
		{
			return std::find(actionsets.begin(), actionsets.end(), setname) != actionsets.end();
		}

		bool InputDevice::LoadConfig(const std::string& filepath)
		{
			if (FileSystem::GetFileType(filepath) == 1)
			{
				config.clear();
				config = Table::Load(filepath);
			}

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

			return !config.empty();
		}

		bool InputDevice::SaveConfig(const std::string& filepath)
		{
			return Table::Save(config, filepath);
		}

		void InputDevice::SetAction(const std::string& actionname, ButtonCode button)
		{
			config[actionname] = button;
		}

		void InputDevice::SetAction(const std::string& actionname, ButtonCode button, const std::string& setname)
		{
			if (setname.empty())
			{
				config[actionname] = button;
				return;
			}

			RegisterActionSet(setname);
			config[setname][actionname] = button;
		}

		void InputDevice::SetAction(const std::string& actionname, std::vector<ButtonCode> buttons)
		{
			config[actionname] = {};
			for (const auto& p : buttons)
			{
				config[actionname].push_back(p);
			}
		}

		void InputDevice::SetAction(const std::string& actionname, std::vector<ButtonCode> buttons, const std::string& setname)
		{
			if (setname.empty())
			{
				SetAction(actionname, buttons);
				return;
			}

			RegisterActionSet(setname);
			config[setname][actionname] = {};
			for (const auto& p : buttons)
			{
				config[setname][actionname].push_back(p);
			}
		}

		void InputDevice::SetAction(const std::string& actionname, AxisCode axis)
		{
			if (config["axes"].is_null()) config["axes"] = {};
			config["axes"][actionname] = axis;
		}

		void InputDevice::SetAction(const std::string& actionname, AxisCode axis, const std::string& setname)
		{
			if (setname.empty())
			{
				SetAction(actionname, axis);
				return;
			}

			RegisterActionSet(setname);
			if (config[setname]["axes"].is_null()) config[setname]["axes"] = {};
			config[setname]["axes"][actionname] = axis;
		}

		void InputDevice::SetAction(const std::string& actionname, InputAction action, const std::string& setname)
		{
			if (setname.empty())
			{
				if (!action.buttons.empty())
				{
					if (action.buttons.size() == 1)
					{
						SetAction(actionname, action.buttons[0]);
					}
					else
					{
						SetAction(actionname, action.buttons);
					}
				}
				else if (action.axis != AXIS_INVAILD)
				{
					SetAction(actionname, action.axis);
				}
			}
			else
			{
				RegisterActionSet(setname);
				if (!action.buttons.empty())
				{
					if (action.buttons.size() == 1)
					{
						SetAction(actionname, action.buttons[0], setname);
					}
					else
					{
						SetAction(actionname, action.buttons, setname);
					}
				}
				else if (action.axis != AXIS_INVAILD)
				{
					SetAction(actionname, action.axis, setname);
				}
			}
		}

		InputAction InputDevice::GetActionData(const std::string& actionname, const std::string& setname)
		{
			InputAction data;

			if (setname.empty())
			{
				if (config[actionname].is_integer())
				{
					int d = config[actionname];
					data.buttons.push_back((ButtonCode)d);
				}
				else if (config[actionname].is_array())
				{
					for (std::size_t n = 0; n < config[actionname].size(); ++n)
					{
						data.buttons.push_back((ButtonCode)n);
					}
				}
				else if (config["axes"][actionname].is_integer())
				{
					int d = config["axes"][actionname];
					data.axis = (AxisCode)d;
				}
				else if (config["axes"][actionname].is_array() && config["axes"][actionname].size() == 4)
				{
					auto v = config["axes"][actionname];
					int up = v[0];
					int down = v[1];
					int left = v[2];
					int right = v[3];

					data.btnaxis.up = (ButtonCode)up;
					data.btnaxis.down = (ButtonCode)down;
					data.btnaxis.left = (ButtonCode)left;
					data.btnaxis.right = (ButtonCode)right;
				}
			}
			else
			{
				RegisterActionSet(setname);
				if (config[setname][actionname].is_integer())
				{
					int d = config[setname][actionname];
					data.buttons.push_back((ButtonCode)d);
				}
				else if (config[setname][actionname].is_array())
				{
					for (std::size_t n = 0; n < config[setname][actionname].size(); ++n)
					{
						data.buttons.push_back((ButtonCode)n);
					}
				}
				else if (config[setname]["axes"][actionname].is_integer())
				{
					int d = config[setname]["axes"][actionname];
					data.axis = (AxisCode)d;
				}
				else if (config[setname]["axes"][actionname].is_array() && config[setname]["axes"][actionname].size() == 4)
				{
					auto v = config[setname]["axes"][actionname];
					int up = v[0];
					int down = v[1];
					int left = v[2];
					int right = v[3];

					data.btnaxis.up = (ButtonCode)up;
					data.btnaxis.down = (ButtonCode)down;
					data.btnaxis.left = (ButtonCode)left;
					data.btnaxis.right = (ButtonCode)right;
				}
			}

			return data;
		}

		const std::vector<std::string> InputDevice::GetActionSets()
		{
			return actionsets;
		}

		const table InputDevice::GetActionSetTable(const std::string& setname)
		{
			if (setname.empty())
			{
				return config[setname];
			}
			else
			{
				return config;
			}
		}

		void InputDevice::SetActiveSet(const std::string& setname)
		{
			activeset = setname;
		}

		std::vector<Leadwerks::Texture*> InputDevice::GetGlyphs(const std::string& actionname, const std::string& setname)
		{
			std::vector<Leadwerks::Texture*> glyphs;

			// Buttons
			{
				// Setless Actions
				if (config[actionname].is_integer())
				{
					int button = config[actionname];
					glyphs.push_back(GetButtonGlyph((ButtonCode)button));
				}
				else if (config[actionname].is_array())
				{
					for (std::size_t n = 0; n < config[actionname].size(); ++n)
					{
						int button = config[actionname][n];
						glyphs.push_back(GetButtonGlyph((ButtonCode)button));
					}
				}

				// Actions within Sets
				if (!activeset.empty())
				{
					if (config[activeset][actionname].is_integer())
					{
						int button = config[activeset][actionname];
						glyphs.push_back(GetButtonGlyph((ButtonCode)button));
					}
					else if (config[activeset][actionname].is_array())
					{
						for (std::size_t n = 0; n < config[activeset][actionname].size(); ++n)
						{
							int button = config[actionname][n];
							glyphs.push_back(GetButtonGlyph((ButtonCode)button));
						}
					}
				}
			}

			// Axes
			{
				// Setless Actions
				if (config["axes"][actionname].is_integer())
				{
					int code = config["axes"][actionname];
					glyphs.push_back(GetAxisGlyph((AxisCode)code));
				}
				else if (config["axes"][actionname].is_array() && config["axes"][actionname].size() == 4)
				{
					auto v = config["axes"][actionname];
					int up = v[0];
					int down = v[1];
					int left = v[2];
					int right = v[3];

					glyphs.push_back(GetButtonGlyph((ButtonCode)up));
					glyphs.push_back(GetButtonGlyph((ButtonCode)down));
					glyphs.push_back(GetButtonGlyph((ButtonCode)left));
					glyphs.push_back(GetButtonGlyph((ButtonCode)right));
				}

				// Actions within Sets
				if (!activeset.empty())
				{
					if (config[activeset]["axes"][actionname].is_integer())
					{
						int code = config[activeset]["axes"][actionname];
						glyphs.push_back(GetAxisGlyph((AxisCode)code));
					}
					else if (config[activeset]["axes"][actionname].is_array() && config[activeset]["axes"][actionname].size() == 4)
					{
						auto v = config[activeset]["axes"][actionname];
						int up = v[0];
						int down = v[1];
						int left = v[2];
						int right = v[3];

						glyphs.push_back(GetButtonGlyph((ButtonCode)up));
						glyphs.push_back(GetButtonGlyph((ButtonCode)down));
						glyphs.push_back(GetButtonGlyph((ButtonCode)left));
						glyphs.push_back(GetButtonGlyph((ButtonCode)right));
					}
				}
			}

			return glyphs;
		}
	}
}