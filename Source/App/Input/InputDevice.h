#pragma once
#include "pch.h"
#include "ButtonCode.h"

namespace App
{
	namespace InputSystem
	{
		class ActionController;
		class BaseInputDevice : public Leadwerks::Object
		{
		protected:
			bool suspend;
			virtual void Initialize() {};
			virtual void Update() {};

		public:
			virtual void SetActiveSet(const std::string& setname) {};
			virtual bool Hit(const std::string& actionname) { return false; };
			virtual bool Down(const std::string& actionname) { return false; };
			virtual Leadwerks::Vec2 Axis(const std::string& actionname) { return Leadwerks::Vec2(0); };
			virtual void Rumble(const float left, const float right, int time) {};

			virtual void Suspend(const bool mode)
			{
				suspend = mode;
			}

			// Pointer Functions
			virtual void SetCursorPosition(const int x, const int y) {};
			virtual void SetCursorPosition(const Leadwerks::iVec2 xy) {};
			virtual void CenterCursor() {};
			Leadwerks::iVec2 GetCursorPosition() { return Leadwerks::iVec2(0, 0); };
			Leadwerks::Vec2 GetCursorFloatPosition() { return Leadwerks::Vec2(0, 0); };
			virtual void SetActionSetCursor(const std::string& setname, bool cursor = false) {};
			virtual bool GetActionSetCursor(const std::string& setname) { return true; };

			virtual bool ProcessEvent(const Leadwerks::Event& e) { return false; };

			std::string devicename;

			BaseInputDevice() {};

			virtual bool LoadConfig(const std::string& filepath) { return true; };
			virtual bool SaveConfig(const std::string& filepath) { return true; };

			std::vector<Leadwerks::Texture*> GetGlyphs(const std::string& actionname, const std::string& setname = "")
			{
				std::vector<Leadwerks::Texture*> glyphs;
				return glyphs;
			}

			friend class ActionController;
		};

		class InputDevice : public BaseInputDevice
		{
		protected:
			int index;
			std::string activeset;
			std::vector<std::string> actionsets;

			void RegisterActionSet(const std::string& setname);
			const bool IsRegisteredActionSet(const std::string& setname);

		public:
			table config;

			InputDevice();

			virtual bool LoadConfig(const std::string& filepath);
			virtual bool SaveConfig(const std::string& filepath);

			virtual void SetActiveSet(const std::string& setname);
			virtual void SetAction(const std::string& actionname, ButtonCode button);
			virtual void SetAction(const std::string& actionname, ButtonCode button, const std::string& setname);
			virtual void SetAction(const std::string& actionname, std::vector<ButtonCode> buttons);
			virtual void SetAction(const std::string& actionname, std::vector<ButtonCode> buttons, const std::string& setname);
			virtual void SetAction(const std::string& actionname, AxisCode axis);
			virtual void SetAction(const std::string& actionname, AxisCode axis, const std::string& setname);
			virtual void SetAction(const std::string& actionname, InputAction action, const std::string& setname = "");
			InputAction GetActionData(const std::string& actionname, const std::string& setname = "");

			const std::vector<std::string> GetActionSets();
			const table GetActionSetTable(const std::string& setname);

			std::vector<Leadwerks::Texture*> GetGlyphs(const std::string& actionname, const std::string& setname = "");

			friend class ActionController;
		};

		extern void SetDevice(const int type);
		extern const int GetDeviceID();
	}
}