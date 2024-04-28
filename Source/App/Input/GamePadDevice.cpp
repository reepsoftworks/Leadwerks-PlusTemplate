#include "pch.h"
#include "ButtonCode.h"
#include "GamePadDevice.h"

namespace App
{
	using namespace Leadwerks;

	namespace InputSystem
	{
		GamePadDevice::GamePadDevice()
		{
			index = 0;
			rumbletimer = NULL;
			devicename = "gamepad";
			connected = false;
			activeset = "";
			actionsets.clear();
		}

		void GamePadDevice::Initialize()
		{
			Print("Initialized " + devicename);

			// Poll to see if any controllers are conected!
			Update();
		}

		bool GamePadDevice::IsConnected()
		{
			return GamePad::Connected(index);
		}

		void GamePadDevice::Update()
		{
			if (connected == false)
			{
				if (GamePad::Connected(index))
				{
					connected = true;
					SetDevice(INPUTDEVICE_GAMEPAD);
				}
			}
			else
			{
				if (!GamePad::Connected(index))
				{
					connected = false;
					SetDevice(INPUTDEVICE_KBM);
				}
			}
		}

		bool GamePadDevice::Hit(const std::string& actionname)
		{
			if (!IsConnected()) return false;

			bool ret = false;

			// Setless Actions
			if (config[actionname].is_integer())
			{
				int button = config[actionname];
				ret = GamePad::ButtonHit(button);
			}
			else if (config[actionname].is_array())
			{
				for (std::size_t n = 0; n < config[actionname].size(); ++n)
				{
					int button = config[actionname][n];
					ret = GamePad::ButtonHit(button);

					if (ret) break;
				}
			}

			// Actions within Sets
			if (!activeset.empty() && !ret)
			{
				if (config[activeset][actionname].is_integer())
				{
					int button = config[activeset][actionname];
					ret = GamePad::ButtonHit(button);
				}
				else if (config[activeset][actionname].is_array())
				{
					for (std::size_t n = 0; n < config[activeset][actionname].size(); ++n)
					{
						int button = config[actionname][n];
						ret = GamePad::ButtonHit(button);

						if (ret) break;
					}
				}
			}

			if (ret) SetDevice(INPUTDEVICE_GAMEPAD);
			return ret;
		}

		bool GamePadDevice::Down(const std::string& actionname)
		{
			if (!IsConnected()) return false;

			bool ret = false;

			// Setless Actions
			if (config[actionname].is_integer())
			{
				int button = config[actionname];
				ret = GamePad::ButtonDown(button);
			}
			else if (config[actionname].is_array())
			{
				for (std::size_t n = 0; n < config[actionname].size(); ++n)
				{
					int button = config[actionname][n];
					ret = GamePad::ButtonDown(button);

					if (ret) break;
				}
			}

			// Actions within Sets
			if (!activeset.empty() && !ret)
			{
				if (config[activeset][actionname].is_integer())
				{
					int button = config[activeset][actionname];
					ret = GamePad::ButtonDown(button);
				}
				else if (config[activeset][actionname].is_array())
				{
					for (std::size_t n = 0; n < config[activeset][actionname].size(); ++n)
					{
						int button = config[actionname][n];
						ret = GamePad::ButtonDown(button);

						if (ret) break;
					}
				}
			}

			if (ret) SetDevice(INPUTDEVICE_GAMEPAD);
			return ret;
		}

		Vec2 GamePadDevice::Axis(const std::string& actionname)
		{
			Vec2 ret = Vec2(0);
			if (!IsConnected()) return ret;

			// Setless Actions
			if (config["axes"][actionname].is_integer())
			{
				int code = config["axes"][actionname];
				ret = GamePad::GetAxisPosition(code);
			}

			// Actions within Sets
			if (!activeset.empty() && (ret.x == 0 && ret.y == 0))
			{
				if (config[activeset]["axes"][actionname].is_integer())
				{
					int code = config[activeset]["axes"][actionname];
					ret = GamePad::GetAxisPosition(code);
				}
			}

			if (ret.x != 0.0f || ret.y != 0.0f) SetDevice(INPUTDEVICE_GAMEPAD);
			return ret;
		}

		void GamePadDevice::Rumble(const float left, const float right, int time)
		{
			// Only rumble if the gamepad is active!
			if (IsConnected() && GetDeviceID() == INPUTDEVICE_GAMEPAD)
			{
				rumbletimer = Timer::Create(time);
				GamePad::Rumble(left, right);
			}
		}

		bool GamePadDevice::ProcessEvent(const Event& e)
		{
			if (e.id == Event::TimerTick)
			{
				if (e.source == rumbletimer)
				{
					if (IsConnected()) GamePad::Rumble(0, 0);
					FreeObject(rumbletimer);
				}
			}

			return true;
		}

		GamePadDevice* GamePadDevice::Create(const int controllerid)
		{
			auto controller = new GamePadDevice();
			controller->index = controllerid;
			controller->Initialize();
			return controller;
		}
	}
}