#include "pch.h"
#include "App.h"

namespace App
{
	using namespace Leadwerks;

	int appmode;
	uint64_t AppID;
	std::map<std::string, std::string> Arguments;

	const int Program::NormalMode = 0;
	const int Program::EditorMode = 1;
	const int Program::DebugMode = 2;

	void Program::ParseArguments(int argc, const char* argv[])
	{
		//AttachDebugHook();
		Arguments.clear();
		Arguments = OS::ParseCommandLine(argc, argv);
		appmode = NormalMode;

		auto exename = std::string(argv[0]);
		exename = Leadwerks::FileSystem::StripAll(exename);
		if (Leadwerks::String::Right(exename, 6) == ".debug") exename = Leadwerks::String::Left(exename, exename.length() - 6);
		Leadwerks::System::AppName = exename;

#ifdef DEBUG
		appmode = DebugMode;
#else
		if (CheckArgument("debug"))
		{
			appmode = DebugMode;
		}
		else if (CheckArgument("devmode"))
		{
			appmode = EditorMode;
		}
#endif // DEBUG

		// If our mode is anything above normal or if -console is used, call the CMD window.
		if (appmode > NormalMode || CheckArgument("console")) OS::CallCMDWindow();
	}

	bool Program::CheckArgument(const std::string& argument)
	{
		return !Arguments[argument].empty();
	}

	std::string Program::CheckArgumentString(const std::string& argument, const std::string& defaultvalue)
	{
		return Arguments[argument];
	}

	int Program::CheckArgumentValue(const std::string& argument, const int defaultvalue)
	{
		return String::Int(Arguments[argument]);
	}

	const int Program::GetAppMode()
	{
		return appmode;
	}

	bool Program::VendorCheck(const bool skipcheck)
	{
		auto driver = Leadwerks::GraphicsDriver::GetCurrent();
		if (driver == NULL)
		{
			OS::MessageError("Error", "Failed to detect graphics driver!");
			return false;
		}

		// Intergrated Intel graphics has issues. 
		// Unless we're told to bypass this check, return false if intel.
		if (!skipcheck)
		{
			auto vendor = driver->GetVendor();
			if (vendor == VENDOR_INTEL)
			{
				int messagebox = OS::MessageContinue("Warning", "Intel intergrated graphics isn't supported.\nContinue anyway?");
				if (messagebox == OS::MessageYes) return true;
				return false;
			}
		}

		return true;
	}

	bool Program::LoadWerkFile()
	{
		if (Leadwerks::System::AppName.empty()) return false;
		std::string file = Leadwerks::System::AppName + ".werk";
		if (FileSystem::GetFileType(file) == 0) return false;
		auto werkfile = Config::Load(file);
		if (!werkfile) return false;

		string id = werkfile->GetValue("AppID");
		AppID = String::UInt64(id);

		// TODO: Load packages listed.
		auto packagelist = werkfile->GetValue("Packages");
		if (!packagelist.is_null())
		{
		}

		return true;
	}
}

#include "GraphicsWindow.h"