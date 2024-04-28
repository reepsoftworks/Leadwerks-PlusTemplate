#include "pch.h"
#include "../App.h"
#include "InputDevice.h"

namespace App
{
	namespace InputSystem
	{
		class KBMDevice : public InputDevice
		{
			GraphicsWindow* window;

			Leadwerks::Vec2 ButtonAxis(const ButtonCode& up, const ButtonCode& down, const ButtonCode& left, const ButtonCode& right);
			Leadwerks::Vec2 GetAxis(const AxisCode axis);

		protected:
			virtual void Initialize();

		public:
			virtual void SetActiveSet(const std::string& setname);
			virtual bool Hit(const std::string& actionname);
			virtual bool Down(const std::string& actionname);
			virtual Leadwerks::Vec2 Axis(const std::string& actionname);

			// Pointer Functions
			virtual void SetCursorPosition(const int x, const int y);
			virtual void SetCursorPosition(const Leadwerks::iVec2 xy);
			virtual void CenterCursor();
			Leadwerks::iVec2 GetCursorPosition();
			Leadwerks::Vec2 GetCursorFloatPosition();
			virtual void SetActionSetCursor(const std::string& setname, bool cursor = false);
			virtual bool GetActionSetCursor(const std::string& setname);

			//virtual bool ProcessEvent(const Leadwerks::Event& e);

			KBMDevice();

			static KBMDevice* Create();
		};
	}
}