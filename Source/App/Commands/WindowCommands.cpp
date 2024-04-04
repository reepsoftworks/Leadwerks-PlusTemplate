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