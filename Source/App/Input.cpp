#include "pch.h"
#include "App.h"

#include "Input/KBMDevice.h"
#include "Input/GamePadDevice.h"
namespace App
{
	using namespace Leadwerks;
	using namespace InputSystem;

	bool initalized = false;
	static std::map<int, InputDevice*> device;
	static std::string activeset = "";

	static std::string default_bindings_kbm = "Config/bindings_default.json";
	static std::string default_bindings_gamepad = "Config/bindings_gamepad_default.json";
	static std::string user_bindings_kbm = "Config/bindings_user.json";
	static std::string user_bindings_gamepad = "Config/bindings_gamepad_user.json";

	void Input::Initalize()
	{
		if (initalized) return;
		device[INPUTDEVICE_KBM] = KBMDevice::Create();

		std::string cfg = user_bindings_kbm;
		if (FileSystem::GetFileType(cfg) == 0) cfg = default_bindings_kbm;
		device[INPUTDEVICE_KBM]->LoadConfig(cfg);

		cfg = user_bindings_gamepad;
		if (FileSystem::GetFileType(cfg) == 0) cfg = default_bindings_gamepad;
		if (FileSystem::GetFileType(cfg) != 0)
		{
			device[INPUTDEVICE_GAMEPAD] = GamePadDevice::Create();
			device[INPUTDEVICE_GAMEPAD]->LoadConfig(cfg);
		}

		initalized = true;
	}

	void Input::SaveBindings()
	{
		if (!initalized) return;
		if (device[INPUTDEVICE_KBM]) device[INPUTDEVICE_KBM]->SaveConfig(user_bindings_kbm);
		if (device[INPUTDEVICE_GAMEPAD]) device[INPUTDEVICE_GAMEPAD]->SaveConfig(user_bindings_gamepad);
	}

	void Input::SetActionSet(const std::string& setname)
	{
		for (int i = 0; i < INPUTDEVICE_MAX; i++)
		{
			auto current_device = device[i];
			if (current_device != NULL)
			{
				current_device->SetActiveSet(setname);
			}
		}

		activeset = setname;
	}

	bool Input::ActionDown(const std::string& actionname)
	{
		Initalize();
		if (device.empty()) return false;
		auto current_device = device[InputSystem::GetDeviceID()];
		if (current_device == NULL) return false;
		return current_device->Down(actionname);
	}

	bool Input::ActionHit(const std::string& actionname)
	{
		Initalize();
		if (device.empty()) return false;
		auto current_device = device[InputSystem::GetDeviceID()];
		if (current_device == NULL) return false;
		return current_device->Hit(actionname);
	}

	Leadwerks::Vec2 Input::ActionAxis(const std::string& actionname)
	{
		Initalize();
		if (device.empty()) return Vec2(0,0);
		auto current_device = device[InputSystem::GetDeviceID()];
		if (current_device == NULL) return Vec2(0, 0);
		return current_device->Axis(actionname);
	}

	Vec2 Input::ActionsToAxis(const std::string& xa, const std::string& xb, const std::string& ya, const std::string& yb)
	{
		Initalize();
		const bool bUp = ActionDown(xa);
		const bool bDwn = ActionDown(xb);
		const bool bLft = ActionDown(ya);
		const bool bRt = ActionDown(yb);
		return Vec2(static_cast<float>(bRt - bLft), static_cast<float>(bUp - bDwn));
	}

	void Input::Rumble(int duration, const float left, const float right)
	{
		Initalize();
		if (!device.empty())
		{
			auto current_device = device[InputSystem::GetDeviceID()];
			if (current_device) current_device->Rumble(left, right, duration);
		}
	}

	InputSystem::InputDevice* Input::GetDevice(const int id)
	{
		Initalize();
		return device[id];
	}

	int Input::GetDeviceID()
	{
		Initalize();
		return InputSystem::GetDeviceID();
	}

	std::string Input::GetActionSet()
	{
		return activeset;
	}
}