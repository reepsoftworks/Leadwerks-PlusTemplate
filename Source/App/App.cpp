#include "pch.h"
#include "App.h"

namespace App
{
	using namespace Leadwerks;

	int appmode;
	std::map<std::string, std::string> Program::Arguments;
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

	std::string Title = "";
	uint64_t Program::AppID = 0;
	std::string Program::Author = "";
	std::string Program:: Copyright = "";

	std::string Program::GetTitle()
	{
		if (!Title.empty())
			return Title;

		if (Title.empty()) Title = Leadwerks::System::AppName;
		if (Title.empty()) Title = "Leadwerks";
		return Title;
	}

	bool Program::LoadWerkFile()
	{
		if (Leadwerks::System::AppName.empty()) return false;
		std::string file = Leadwerks::System::AppName + ".werk";
		if (FileSystem::GetFileType(file) == 0) return false;
		auto werkfile = Config::Load(file);
		if (!werkfile) return false;

		// Replace the name with the one in the file.
		// Title = werkfile->GetValue("Title");
		AppID = String::UInt64(werkfile->GetValue("AppID"));
		Author = werkfile->GetValue("Author");
		Copyright = werkfile->GetValue("Copyright");

		werkfile->Release();
		werkfile = NULL;

		return true;
	}

	bool Program::ExecuteTable(table input)
	{
		if (input.is_null()) return false;
		bool ret = false;
		if (input.is_object())
		{
			for (const auto p : input)
			{
				std::string key = p.first;
				key = UnQuoteString(key);
				key = String::Trim(key);

				std::string val = p.second;
				val = UnQuoteString(val);
				val = String::Trim(val);

				auto cmd = key + " " + val;
				ret = ExecuteCommand(cmd, true);
			}
		}

		return ret;
	}

	bool Program::ExecuteMap(std::map<std::string, std::string> input)
	{
		if (input.empty()) return false;
		bool ret = false;
		for (const auto p : input)
		{
			std::string key = p.first;
			key = UnQuoteString(key);
			key = String::Trim(key);

			std::string val = p.second;
			val = UnQuoteString(val);
			val = String::Trim(val);

			// Fix any paths given.
			if (FileSystem::GetFileType(val) != 0)
				val = FileSystem::FixPath(val);

			auto cmd = key + " " + val;
			ret = ExecuteCommand(cmd, true);
		}

		return ret;
	}

	bool Program::LoadConVars(const std::string& path)
	{
		// Classic cfg file
		auto stream = FileSystem::ReadFile(path);
		if (!stream) return false;

		while (!stream->EOF())
		{
			auto line = stream->ReadLine();

			// Find the key.
			std::vector<std::string> ln = String::Split(line, " ");
			if (ln.size() == 2)
			{
				std::string key = ln[0];
				key = UnQuoteString(key);
				key = String::Trim(key);

				std::string val = ln[1];
				val = UnQuoteString(val);
				val = String::Trim(val);
				
				// Fix any paths given.
				if (FileSystem::GetFileType(val) != 0)
					val = FileSystem::FixPath(val);

				SetConVar(key, val);
			}
		}

		stream = NULL;
		return true;
	}

	bool Program::SaveConVars(const std::string& path)
	{
		// Classic cfg file
		auto stream = FileSystem::WriteFile(path);
		if (!stream) return false;

		for (const auto& a : *ConVar::GetConVars())
		{
			auto name = a.first;
			auto val = a.second->GetValue();
			auto flag = a.second->GetFlag();

			if (flag == CVAR_SAVE)
			{
				auto line = name + " " + val;
				stream->WriteLine(line);
			}
		}

		stream = NULL;
		return true;
	}
}