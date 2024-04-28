#pragma once
//#include "../App.h"

namespace App
{
	namespace InputSystem
	{
		//-------------------------------------------------------------------------------
		// Devices
		//-------------------------------------------------------------------------------

		const enum
		{
			INPUTDEVICE_KBM = 0,
			INPUTDEVICE_GAMEPAD,
			INPUTDEVICE_MAX
		};

		//-------------------------------------------------------------------------------
		// Button Code
		//-------------------------------------------------------------------------------

		enum ButtonCode
		{
			BUTTON_INVAILD = -1,
			BUTTON_NONE = 0,

			// Mouse
			BUTTON_MOUSE_LBUTTON = 0x1,
			BUTTON_MOUSE_RBUTTON = 0x2,
			BUTTON_MOUSE_MBUTTON = 4,
			BUTTON_MOUSE_XBUTTON1 = 5,
			BUTTON_MOUSE_XBUTTON2,

			// Keyboard
			BUTTON_FIRST_KEY = 8,
			BUTTON_KEY_BACKSPACE = 8,
			BUTTON_KEY_TAB,
			BUTTON_KEY_ENTER = 13,
			BUTTON_KEY_SHIFT = 16,
			BUTTON_KEY_CONTROL,
			BUTTON_KEY_ALT,
			BUTTON_KEY_CAPSLOCK = 20,
			BUTTON_KEY_ESCAPE = 27,
			BUTTON_KEY_SPACE = 32,
			BUTTON_KEY_PAGEUP = 33,
			BUTTON_KEY_PAGEDOWN = 34,
			BUTTON_KEY_END = 35,
			BUTTON_KEY_HOME = 36,
			BUTTON_KEY_LEFT = 37,
			BUTTON_KEY_UP = 38,
			BUTTON_KEY_RIGHT = 39,
			BUTTON_KEY_DOWN = 40,
			BUTTON_KEY_INSERT = 45,
			BUTTON_KEY_DELETE,
			BUTTON_KEY_D0 = 48,
			BUTTON_KEY_D1 = 49,
			BUTTON_KEY_D2 = 50,
			BUTTON_KEY_D3 = 51,
			BUTTON_KEY_D4 = 52,
			BUTTON_KEY_D5 = 53,
			BUTTON_KEY_D6 = 54,
			BUTTON_KEY_D7 = 55,
			BUTTON_KEY_D8 = 56,
			BUTTON_KEY_D9 = 57,
			BUTTON_KEY_A = 65,
			BUTTON_KEY_B = 66,
			BUTTON_KEY_C = 67,
			BUTTON_KEY_D = 68,
			BUTTON_KEY_E = 69,
			BUTTON_KEY_F = 70,
			BUTTON_KEY_G = 71,
			BUTTON_KEY_H = 72,
			BUTTON_KEY_I = 73,
			BUTTON_KEY_J = 74,
			BUTTON_KEY_K = 75,
			BUTTON_KEY_L = 76,
			BUTTON_KEY_M = 77,
			BUTTON_KEY_N = 78,
			BUTTON_KEY_O = 79,
			BUTTON_KEY_P = 80,
			BUTTON_KEY_Q = 81,
			BUTTON_KEY_R = 82,
			BUTTON_KEY_S = 83,
			BUTTON_KEY_T = 84,
			BUTTON_KEY_U = 85,
			BUTTON_KEY_V = 86,
			BUTTON_KEY_W = 87,
			BUTTON_KEY_X = 88,
			BUTTON_KEY_Y = 89,
			BUTTON_KEY_Z = 90,
			BUTTON_KEY_COMMAND = 91,
			//BUTTON_KEY_WINDOWS = 91,
			BUTTON_KEY_F1 = 112,
			BUTTON_KEY_F2 = 113,
			BUTTON_KEY_F3 = 114,
			BUTTON_KEY_F4 = 115,
			BUTTON_KEY_F5 = 116,
			BUTTON_KEY_F6 = 117,
			BUTTON_KEY_F7 = 118,
			BUTTON_KEY_F8 = 119,
			BUTTON_KEY_F9 = 120,
			BUTTON_KEY_F10 = 121,
			BUTTON_KEY_F11 = 122,
			BUTTON_KEY_F12 = 123,
			BUTTON_KEY_NUMLOCK = 144,
			BUTTON_KEY_SEMICOLON = 186,
			BUTTON_KEY_EQUALS = 187,
			BUTTON_KEY_COMMA = 188,
			BUTTON_KEY_SUBTRACT = 189,
			BUTTON_KEY_PERIOD = 190,
			BUTTON_KEY_SLASH = 191,
			BUTTON_KEY_TILDE = 192,
			BUTTON_KEY_OPENBRACKET = 219,
			BUTTON_KEY_BACKSLASH = 220,
			BUTTON_KEY_CLOSEBRACKET = 221,
			BUTTON_KEY_QUOTES = 222,
			BUTTON_KEY_NUMPAD0 = 96,
			BUTTON_KEY_NUMPAD1,
			BUTTON_KEY_NUMPAD2,
			BUTTON_KEY_NUMPAD3,
			BUTTON_KEY_NUMPAD4,
			BUTTON_KEY_NUMPAD5,
			BUTTON_KEY_NUMPAD6,
			BUTTON_KEY_NUMPAD7,
			BUTTON_KEY_NUMPAD8,
			BUTTON_KEY_NUMPAD9,
			BUTTON_KEY_NUMPADPERIOD = 110,
			BUTTON_KEY_NUMPADDIVIDE = 111,
			BUTTON_KEY_NUMPADMULTIPLY = 106,
			BUTTON_KEY_NUMPADSUBTRACT = 109,
			BUTTON_KEY_NUMPADADDITION = 107,
			BUTTON_LAST_KEY = BUTTON_KEY_NUMPADADDITION - 1,

			// Virtual Mouse
			BUTTON_MOUSE_WHEELUP = 257,
			BUTTON_MOUSE_WHEELDOWN,
			BUTTON_MOUSE_WHEELLEFT,
			BUTTON_MOUSE_WHEELRIGHT,

			// GamePad
			BUTTON_FIRST_GAMEPAD = 512,
			BUTTON_GAMEPAD_DPADUP = BUTTON_FIRST_GAMEPAD,
			BUTTON_GAMEPAD_DPADDOWN,
			BUTTON_GAMEPAD_DPADLEFT,
			BUTTON_GAMEPAD_DPADRIGHT,
			BUTTON_GAMEPAD_START,
			BUTTON_GAMEPAD_BACK,
			BUTTON_GAMEPAD_LTHUMB,
			BUTTON_GAMEPAD_RTHUMB,
			BUTTON_GAMEPAD_LSHOULDER,
			BUTTON_GAMEPAD_RSHOULDER,
			BUTTON_GAMEPAD_RTRIGGER,
			BUTTON_GAMEPAD_LTRIGGER,
			BUTTON_GAMEPAD_X = 526,
			BUTTON_GAMEPAD_Y = 527,
			BUTTON_GAMEPAD_A = 524,
			BUTTON_GAMEPAD_B = 525,

			// VR
			BUTTON_FIRST_VR = 530,
			BUTTON_VR_LEFT_SYSTEM = BUTTON_FIRST_VR,
			BUTTON_VR_LEFT_B,
			BUTTON_VR_LEFT_MENU = BUTTON_VR_LEFT_B,
			BUTTON_VR_LEFT_GRIP,
			BUTTON_VR_LEFT_LEFT,
			BUTTON_VR_LEFT_UP,
			BUTTON_VR_LEFT_RIGHT,
			BUTTON_VR_LEFT_DOWN,
			BUTTON_VR_LEFT_A,
			BUTTON_VR_LEFT_TOUCHPAD,
			BUTTON_VR_LEFT_TRIGGER,

			BUTTON_VR_RIGHT_SYSTEM,
			BUTTON_VR_RIGHT_B,
			BUTTON_VR_RIGHT_MENU = BUTTON_VR_RIGHT_B,
			BUTTON_VR_RIGHT_GRIP,
			BUTTON_VR_RIGHT_LEFT,
			BUTTON_VR_RIGHT_UP,
			BUTTON_VR_RIGHT_RIGHT,
			BUTTON_VR_RIGHT_DOWN,
			BUTTON_VR_RIGHT_A,
			BUTTON_VR_RIGHT_TOUCHPAD,
			BUTTON_VR_RIGHT_TRIGGER,

			BUTTON_LAST = BUTTON_VR_RIGHT_TRIGGER
		};

		//-------------------------------------------------------------------------------
		// Axis Code
		//-------------------------------------------------------------------------------

		enum AxisCode
		{
			AXIS_INVAILD = -1,
			AXIS_MOUSE = 0,
			AXIS_MOUSEINVERT,

			// XInput
			AXIS_RTRIGGER = 8,
			AXIS_LTRIGGER,
			AXIS_RSTICK,
			AXIS_LSTICK,

			AXIS_LAST = AXIS_LSTICK
		};

		//-------------------------------------------------------------------------------
		// Button Axis
		//-------------------------------------------------------------------------------

		struct AxisButton
		{
			ButtonCode up = BUTTON_INVAILD;
			ButtonCode down = BUTTON_INVAILD;
			ButtonCode left = BUTTON_INVAILD;
			ButtonCode right = BUTTON_INVAILD;

			bool empty()
			{
				if (up == BUTTON_INVAILD && down == BUTTON_INVAILD && left == BUTTON_INVAILD && right == BUTTON_INVAILD)
					return true;

				return false;
			}
		};

		const enum
		{
			BUTTONAXIS_UP = 0,
			BUTTONAXIS_DOWN,
			BUTTONAXIS_LEFT,
			BUTTONAXIS_RIGHT
		};

		//-------------------------------------------------------------------------------
		// Input Action
		//-------------------------------------------------------------------------------

		struct InputAction
		{
			std::vector<ButtonCode> buttons;
			AxisCode axis = AXIS_INVAILD;
			AxisButton btnaxis =
			{
				BUTTON_INVAILD,
				BUTTON_INVAILD,
				BUTTON_INVAILD,
				BUTTON_INVAILD
			};
		};

		extern ButtonCode StringToButtonCode(const std::string& btnstring);
		extern std::string ButtonCodeToString(const ButtonCode& btncode);
		extern AxisCode StringToAxisCode(const std::string& btnstring);
		extern std::string AxisCodeToString(const AxisCode& btncode);
	}
}