#pragma once
#include "../App.h"
#include "ButtonCode.h"

namespace App
{
	namespace InputSystem
	{
		extern bool LoadGlyphs(const std::string& filepath);
		extern Leadwerks::Texture* GetButtonGlyph(const ButtonCode code);
		extern Leadwerks::Texture* GetAxisGlyph(const AxisCode code);
	}
}