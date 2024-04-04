#include "pch.h"

using namespace Leadwerks;

static const std::string CommandFlagToString(const int i)
{
	std::string str = "CVAR_UNKNOWN";

	switch (i)
	{
	case CVAR_DEFAULT:
		str = "CVAR_DEFAULT";
		break;

	case CVAR_SAVE:
		str = "CVAR_SAVE";
		break;

	case CVAR_CHEAT:
		str = "CVAR_CHEAT";
		break;

	default:
		break;
	}

	return str;
}

void CC_Help(std::vector<std::string> pArgV)
{
	if (pArgV.size() <= 0)
		return;

	if (pArgV[0] != "")
	{
		auto query = String::Trim(pArgV[0]);
		if (ConCommand::Find(query))
		{
			auto map = *ConCommand::GetCommands();
			auto command = map[query];
			auto flag = command.flag;
			auto description = command.description;

			if (!description.empty())
			{
				description = " - " + description;
			}

			Print("\"" + command.GetName() + "\" " + CommandFlagToString(flag) + description);

		}
		else if (ConVar::Find(query))
		{
			auto map = *ConVar::GetConVars();
			auto convar = map[query];
			if (convar != NULL)
			{
				auto flag = convar->GetFlag();
				auto description = convar->GetDescription();

				if (!description.empty())
				{
					description = " - " + description;
				}

				Print("\"" + query + "\"" + " = \"" + convar->GetValue() + "\" " + CommandFlagToString(flag) + description);
			}
		}
		else
		{
			Print("No such command or convar exist for \"" + query + +"\"");
		}
	}
}
static ConCommand help("help", CC_Help, CVAR_DEFAULT, "Usage: help <command/cvar name>");

void CC_CvarList(std::vector<std::string> pArgV)
{
	Print("-------------------");
	Print("----CONCOMMANDS----");
	Print("-------------------");
	size_t count = 0;
	for (const auto& a : *ConCommand::GetCommands())
	{
		auto name = a.first;
		auto description = a.second.description;
		auto flag = a.second.flag;

		if (!description.empty())
		{
			description = " - " + description;
		}

		if (flag != CVAR_HIDDEN) Print("\"" + name + "\" " + CommandFlagToString(flag) + description);
		count++;
	}

	Print("");
	Print("-------------------");
	Print("------CONVARS------");
	Print("-------------------");

	for (const auto& a : *ConVar::GetConVars())
	{
		auto name = a.first;
		auto val = a.second->GetValue();
		auto flag = a.second->GetFlag();
		auto description = a.second->GetDescription();

		if (!description.empty())
		{
			description = " - " + description;
		}

		if (flag != CVAR_HIDDEN) Print("\"" + name + "\"" + " = \"" + val + "\" " + CommandFlagToString(flag) + description);
		count++;
	}
	Print("-------------------");
	Print(String(count) + " counted ConCommands/Cvars.");
}
static ConCommand cvarlist("cvarlist", CC_CvarList, CVAR_DEFAULT, "Reports all available ConCommands and Convars.");

void CC_ConsoleLog(std::vector<std::string> pArgV)
{
	auto stream = FileSystem::WriteFile("console.log");
	stream->Seek(0);

	auto pos = logstream__->GetPos();
	logstream__->Seek(0);
	while (!logstream__->EOF())
	{
		stream->WriteLine(logstream__->ReadLine());
	}
	logstream__->Seek(pos);

	stream = NULL;
}
static ConCommand _consolelog("consolelog", CC_ConsoleLog, CVAR_HIDDEN, "");

void CC_Cheats(std::vector<std::string> pArgV)
{
	if (pArgV.size() <= 0)
		return;

	auto val = String::Int(pArgV[0]);

	if (val > 0)
	{
		if (!Cheating())
		{
			EnableCheats();
			FireCallback(CALLBACK_CHEATSENABLED, NULL, NULL);
			Print("Warning: Cheats enabled");
		}
	}
	else
	{
		//if (cheating)
		//{
		//	cheating = false;
		//	Print("Warning: Cheats disabled");
		//}
	}
}
static ConCommand cheats("cheats", CC_Cheats, CVAR_DEFAULT, "Enables Cheats");

void CC_Quit(std::vector<std::string> pArgV)
{
	EventQueue::Emit(Event::Quit, NULL);
}
static ConCommand quit("quit", CC_Quit, CVAR_DEFAULT, "Quits the application");