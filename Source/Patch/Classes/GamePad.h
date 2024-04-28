#pragma once
#include "../LeadwerksPlus.h"
#ifdef _WIN32
#include <xinput.h>
#pragma comment(lib, "xinput.lib")
#endif

namespace Leadwerks
{
	class GamePad
	{
	public:
        static bool Connected(const int controller = 0);
        static bool ButtonDown(const int button, const int controller = 0);
        static bool ButtonHit(const int button, const int controller = 0);
        static Vec2 GetAxisPosition(const int axis, const int controller = 0);
        static void Rumble(const float left, const float right, const int controller = 0);

		static const int LeftStick;//lua
		static const int RightStick;//lua
		static const int LeftTrigger;//lua
		static const int RightTrigger;//lua
		static const int DPadUp;//lua
		static const int DPadDown;//lua
		static const int DPadLeft;//lua
		static const int DPadRight;//lua
		static const int StartButton;//lua
		static const int BackButton;//lua
		static const int LeftBumper;//lua
		static const int RightBumper;//lua
		static const int XButton;//lua
		static const int YButton;//lua
		static const int AButton;//lua
		static const int BButton;//lua
	};
}