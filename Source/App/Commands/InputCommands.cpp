#include "pch.h"
#include "../Input/ButtonCode.h"
#include "../App.h"

using namespace Leadwerks;

ConVar mousesmoothing("mousesmoothing", "0", CVAR_SAVE, "Usage: mousesmoothing <float>");
ConVar mousesensitivity("mousesensitivity", "1.0", CVAR_SAVE, "Usage: mousesensitivity <float>");
ConVar mouseinvert("mouseinvert", "0", CVAR_SAVE, "Usage: mouseinvert <bool>");

void CC_Bind(std::vector<std::string> pArgV)
{
	if (pArgV.empty())
		return;

	if (pArgV[0].empty())
		return;

	if (pArgV[1].empty())
		return;

	auto button = App::InputSystem::StringToButtonCode(pArgV[0]);
	auto cmd = pArgV[1];

	auto window = App::GraphicsWindow::GetCurrent();
	if (window) window->commandbinds[button] = cmd;
}
static ConCommand bindcommand("bind", CC_Bind, CVAR_DEFAULT, "Usage: CC_Bind <buttonname> <command/cvar name>");