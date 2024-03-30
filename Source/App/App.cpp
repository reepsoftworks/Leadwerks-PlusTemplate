#include "pch.h"
#include "App.h"

namespace App
{
	using namespace Leadwerks;

	AppMode appmode;
	std::map<std::string, std::string> Arguments;
	extern void ParseArguments(int argc, const char* argv[])
	{
		//AttachDebugHook();
		Arguments.clear();
		Arguments = OS::ParseCommandLine(argc, argv);
		appmode = MODE_NORMAL;

#ifdef DEBUG
		appmode = MODE_DEBUG;
#else
		if (CheckArgument("debug"))
		{
			appmode = MODE_DEBUG;
		}
		else if (CheckArgument("devmode"))
		{
			appmode = MODE_EDITOR;
		}
#endif // DEBUG

		// If our mode is anything above normal or if -console is used, call the CMD window.
		if (appmode > MODE_NORMAL || CheckArgument("console")) OS::CallCMDWindow();
	}

	bool CheckArgument(const std::string& argument)
	{
		return !Arguments[argument].empty();
	}

	std::string CheckArgumentString(const std::string& argument, const std::string& defaultvalue)
	{
		return Arguments[argument];
	}

	int CheckArgumentValue(const std::string& argument, const int defaultvalue)
	{
		return String::Int(Arguments[argument]);
	}

	AppMode GetAppMode()
	{
		return appmode;
	}
}

#include "GraphicsWindow.h"