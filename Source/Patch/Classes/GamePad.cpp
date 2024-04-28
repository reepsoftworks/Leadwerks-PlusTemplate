#include "pch.h"
#include "GamePad.h"

namespace Leadwerks
{
    const int GamePad::LeftStick = 0;
    const int GamePad::RightStick = 1;
    const int GamePad::LeftTrigger = 2;
    const int GamePad::RightTrigger = 3;
    const int GamePad::DPadUp = 512;
    const int GamePad::DPadDown = 513;
    const int GamePad::DPadLeft = 514;
    const int GamePad::DPadRight = 515;
    const int GamePad::StartButton = 516;
    const int GamePad::BackButton = 517;
    const int GamePad::LeftBumper = 518;
    const int GamePad::RightBumper = 519;
    const int GamePad::XButton = 526;
    const int GamePad::YButton = 527;
    const int GamePad::AButton = 524;
    const int GamePad::BButton = 525;

    struct GamepadState
    {
        bool connected;
        std::array<Vec2, 4> axisposition;
        std::array<bool, 16> buttondownstate;
        std::array<bool, 16> buttonhitstate;

        GamepadState()
        {
            connected = false;
            buttondownstate.fill(false);
            buttonhitstate.fill(false);
        }
    };

    std::array<GamepadState, 4> gpstate;
    //https://docs.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_gamepad
    void UpdateGamepadState(const int controller)
    {
#ifdef _WIN32
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));

        DWORD dwResult = XInputGetState(controller, &state);
        Vec2 axis;

        if (dwResult == ERROR_SUCCESS)
        {
            gpstate[controller].connected = true;

            // Triggers:
            gpstate[controller].axisposition[GamePad::RightTrigger - GamePad::RightTrigger] = Math::Clamp(float(state.Gamepad.bRightTrigger - XINPUT_GAMEPAD_TRIGGER_THRESHOLD) / float(255 - XINPUT_GAMEPAD_TRIGGER_THRESHOLD), 0.0f, 1.0f);
            gpstate[controller].axisposition[GamePad::LeftTrigger - GamePad::RightTrigger] = Math::Clamp(float(state.Gamepad.bLeftTrigger - XINPUT_GAMEPAD_TRIGGER_THRESHOLD) / float(255 - XINPUT_GAMEPAD_TRIGGER_THRESHOLD), 0.0f, 1.0f);
             
            // Left stick
            axis = Vec2(state.Gamepad.sThumbLX, state.Gamepad.sThumbLY);
            if (Math::Abs(axis.x) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) axis.x = 0;
            if (Math::Abs(axis.y) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) axis.y = 0;
            axis.x = axis.x / float(32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
            axis.y = axis.y / float(32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
            axis.x = Math::Clamp(axis.x, -1.0f, 1.0f);
            axis.y = Math::Clamp(axis.y, -1.0f, 1.0f);
            gpstate[controller].axisposition[GamePad::LeftStick - GamePad::RightTrigger] = axis;

            // Right stick
            axis = Vec2(state.Gamepad.sThumbRX, state.Gamepad.sThumbRY);
            if (Math::Abs(axis.x) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) axis.x = 0;
            if (Math::Abs(axis.y) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) axis.y = 0;
            axis.x = axis.x / float(32767 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
            axis.y = axis.y / float(32767 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
            axis.x = Math::Clamp(axis.x, -1.0f, 1.0f);
            axis.y = Math::Clamp(axis.y, -1.0f, 1.0f);
            gpstate[controller].axisposition[GamePad::LeftStick - GamePad::RightTrigger] = axis;

            // Buttons
            for (int n = 0; n < 16; ++n)
            {
                bool hit = !gpstate[controller].buttondownstate[n];             
                gpstate[controller].buttondownstate[n] = ((state.Gamepad.wButtons & int(Math::Round((float)pow(2, n)))) != 0);
                if (gpstate[controller].buttondownstate[n] and hit)
                {
                    gpstate[controller].buttonhitstate[n] = true;
                }
            }

            // Virtual trigger buttons
            gpstate[controller].buttondownstate[14] = gpstate[controller].axisposition[GamePad::RightTrigger - GamePad::RightTrigger].x > 0.0f;
            if (gpstate[controller].buttondownstate[14]) gpstate[controller].buttonhitstate[14] = true;

            gpstate[controller].buttondownstate[15] = gpstate[controller].axisposition[GamePad::LeftTrigger - GamePad::LeftTrigger].x > 0.0f;
            if (gpstate[controller].buttondownstate[15]) gpstate[controller].buttonhitstate[15] = true;
        }
        else
        {
            gpstate[controller] = {};
        }
#endif
    }

    bool GamePad::Connected(const int controller)
    {
        if (controller < 0 or controller >= XUSER_MAX_COUNT) Debug::Error("Controller index out of range.");
        UpdateGamepadState(controller);
        return gpstate[controller].connected;
    }

    bool GamePad::ButtonDown(const int button, const int controller)
    {
        if (button == GamePad::LeftTrigger)
        {
            UpdateGamepadState(controller);
            return gpstate[controller].buttondownstate[15];
        }
        else if (button == GamePad::RightTrigger)
        {
            UpdateGamepadState(controller);
            return gpstate[controller].buttondownstate[14];
        }
        else if (button == GamePad::LeftStick)
        {
            UpdateGamepadState(controller);
            return gpstate[controller].buttondownstate[518 - GamePad::DPadUp];
        }
        else if (button == GamePad::RightStick)
        {
            UpdateGamepadState(controller);
            return gpstate[controller].buttondownstate[519 - GamePad::DPadUp];
        }

        if (controller < 0 or controller >= XUSER_MAX_COUNT) Debug::Error("Controller index out of range.");
        if (button - GamePad::DPadUp < 0 or button - GamePad::DPadUp >= (int)gpstate[controller].buttondownstate.size()) Debug::Error("Button index out of range.");
        UpdateGamepadState(controller);
        return gpstate[controller].buttondownstate[button - GamePad::DPadUp];
    }

    bool GamePad::ButtonHit(const int button, const int controller)
    {
        if (button == GamePad::LeftTrigger)
        {
            UpdateGamepadState(controller);
            bool hit = gpstate[controller].buttonhitstate[15];
            gpstate[controller].buttonhitstate[15] = false;
            return hit;
        }
        else if (button == GamePad::RightTrigger)
        {
            UpdateGamepadState(controller);
            bool hit = gpstate[controller].buttonhitstate[14];
            gpstate[controller].buttonhitstate[14] = false;
            return hit;
        }
        else if (button == GamePad::LeftStick)
        {
            UpdateGamepadState(controller);
            bool hit = gpstate[controller].buttonhitstate[518 - GamePad::DPadUp];
            gpstate[controller].buttonhitstate[518 - GamePad::DPadUp] = false;
            return hit;
        }
        else if (button == GamePad::RightStick)
        {
            UpdateGamepadState(controller);
            bool hit = gpstate[controller].buttonhitstate[519 - GamePad::DPadUp];
            gpstate[controller].buttonhitstate[519 - GamePad::DPadUp] = false;
            return hit;
        }

        if (controller < 0 or controller >= XUSER_MAX_COUNT) Debug::Error("Controller index out of range.");
        if (button - GamePad::DPadUp < 0 or button - GamePad::DPadUp >= (int)gpstate[controller].buttondownstate.size()) Debug::Error("Button index out of range.");
        UpdateGamepadState(controller);
        bool hit = gpstate[controller].buttonhitstate[button - GamePad::DPadUp];
        gpstate[controller].buttonhitstate[button - GamePad::DPadUp] = false;
        return hit;
    }

    Vec2 GamePad::GetAxisPosition(const int axis, const int controller)
    {
        if (controller < 0 or controller >= XUSER_MAX_COUNT) Debug::Error("Controller index out of range.");
        if (axis - GamePad::RightTrigger < 0 or axis - GamePad::RightTrigger >= (int)gpstate[controller].axisposition.size()) Debug::Error("Axis index out of range.");
        UpdateGamepadState(controller);
        return gpstate[controller].axisposition[axis - GamePad::RightTrigger];
    }

    void GamePad::Rumble(const float left, const float right, const int controller)
    {
        _XINPUT_VIBRATION input = {};
        input.wLeftMotorSpeed = Math::Clamp(Math::Round(left * 65535.0f), 0, 65535);
        input.wRightMotorSpeed = Math::Clamp(Math::Round(right * 65535.0f), 0, 65535);
        XInputSetState(controller, &input);
    }
}