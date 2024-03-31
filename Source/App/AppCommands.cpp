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