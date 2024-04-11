#pragma once
#include "LeadwerksPlus.h"

#undef FILETYPE_FILE
#undef FILETYPE_DIR

namespace Leadwerks
{
	enum PrintColor
	{
		PRINT_COLOR_NORM = 0,
		PRINT_COLOR_MAGENTA,
		PRINT_COLOR_GREEN,
		PRINT_COLOR_BLUE,
		PRINT_COLOR_YELLOW,
		PRINT_COLOR_RED
	};

	enum FileType
	{
		FILETYPE_NONE = 0,
		FILETYPE_FILE,
		FILETYPE_DIR,
	};

	enum CVarType
	{
		CVAR_DEFAULT,
		CVAR_SAVE,
		CVAR_HIDDEN,
		CVAR_CHEAT
	};

	enum Setting
	{
		//SETTING_DISABLED = 0,
		SETTING_LOW,
		SETTING_MEDIUM,
		SETTING_HIGH,
		//SETTING_ULTRA
	};

	// LeadwerksPlus Callback
	enum
	{
		CALLBACK_UPDATE,
		CALLBACK_CHEATSENABLED,
	};

	// LeadwerksPlus EventIDs
	enum
	{
		EVENT_NONE = 0,
		EVENT_STARTRENDERER = 10000,
		EVENT_PAUSESTATE
	};
}