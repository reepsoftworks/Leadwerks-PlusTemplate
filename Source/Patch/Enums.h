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

	// LeadwerksPlus EventIDs
	enum
	{
		EVENT_STARTRENDERER = 10001
	};
}