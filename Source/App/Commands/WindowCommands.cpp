#include "pch.h"
#include "../App.h"

using namespace Leadwerks;
using namespace App;

void CC_WindowSize(std::vector<std::string> pArgV)
{
	if (pArgV.size() <= 0)
		return;

	if (pArgV[0].empty())
		return;

	if (pArgV[1].empty())
		return;

	auto window = GraphicsWindow::GetCurrent();
	if (window)
	{
		auto settings = window->CurrentSettings();
		auto x = settings.size.x;
		auto y = settings.size.y;
		if (String::Int(pArgV[0]) > 0) x = String::Int(pArgV[0]);
		if (String::Int(pArgV[1]) > 0) y = String::Int(pArgV[1]);
		settings.size = iVec2(x, y);
		window->Resize(settings);
	}
}
static ConCommand windowsize("windowsize", CC_WindowSize, CVAR_HIDDEN, "Usage: windowsize <iVec2 (x,y)>");

void CC_WindowMode(std::string pArgV)
{
	if (pArgV.empty())
		return;

	auto window = GraphicsWindow::GetCurrent();
	if (window)
	{
		auto settings = window->CurrentSettings();
		auto value = (GraphicWindowStyles)String::Int(pArgV);
		settings.style = value;
		window->Resize(settings);
	}
}
ConVar windowmode("windowmode", "0", CVAR_SAVE, "Usage: fullscreen <int (0 = Normal, 1 = Borderless, 2 = Fullscreen, 3 = Native Fullscreen, 4 = Fullscreen Boarderless)>", CC_WindowMode);
ConVar screenwidth("screenwidth", "1280", CVAR_SAVE, "Usage: screenwidth <int>");
ConVar screenheight("screenheight", "720", CVAR_SAVE, "Usage: screenheight <int>");