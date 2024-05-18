#pragma once
#include "App.h"
#include "Input/InputDevice.h"

namespace App
{
	class Input : public Leadwerks::Object
	{
	public:
		static void Initalize();
		static void SetActionSet(const std::string& setname);
		static bool ActionDown(const std::string& actionname);
		static bool ActionHit(const std::string& actionname);
		static Leadwerks::Vec2 ActionAxis(const std::string& actionname);
		static Leadwerks::Vec2 ActionsToAxis(const std::string& xa, const std::string& xb, const std::string& ya, const std::string& yb);
		static void Rumble(int duration = 120, const float left = 0.5f, const float right = 0.5f);

		static InputSystem::InputDevice* GetDevice(const int id = 0);
		static int GetDeviceID();
		static std::string GetActionSet();

		static void SaveBindings();
	};
}