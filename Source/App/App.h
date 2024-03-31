#pragma once
#include "pch.h"

namespace App
{
	// Callbacks
	enum
	{
		CALLBACK_INTERFACE = 10,

	};

	extern std::map<std::string, std::string> Arguments;
	extern void ParseArguments(int argc, const char* argv[]);
	extern bool CheckArgument(const std::string& argument);
	extern std::string CheckArgumentString(const std::string& argument, const std::string& defaultvalue);
	extern int CheckArgumentValue(const std::string& argument, const int defaultvalue);

	enum AppMode
	{
		MODE_NORMAL = 0,
		MODE_EDITOR,
		MODE_DEBUG
	};

	extern AppMode GetAppMode();
	extern bool VendorCheck(const bool skipcheck = false);
	extern bool LoadWerkFile();
}

#include "../version.h"
#include "GraphicsWindow.h"
#include "GameMenu.h"
#include "Gadget.h"

// Gadgets
#include "Gadgets/StatsGadget.h"
#include "Gadgets/ConsoleGadget.h"