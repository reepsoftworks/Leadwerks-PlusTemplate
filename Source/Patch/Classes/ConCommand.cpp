#pragma once
#include "pch.h"
#include "../LeadwerksPlus.h"

namespace Leadwerks
{
	bool cheating = false;
	void EnableCheats()
	{
		cheating = true;
	}

	bool Cheating()
	{
		return cheating;
	}

	std::map<std::string, ConCommand>* concommands = nullptr;
	ConCommand::ConCommand(const std::string& commandname, const std::function<void(std::vector<std::string>)>& callback, const int flag, const std::string& description)
	{
		m_Callback = callback;
		name = String::Lower(commandname);
		this->description = description;
		this->flag = flag;

		GetCommands()->insert({ name, *this });
	}

	bool ConCommand::FireCallback(Tokenizer& pTokenizer)
	{
		if (!cheating && flag == CVAR_CHEAT)
		{
			Print("ConCommand \"" + name + "\" is a registered cheat command. Please enable cheats first.");
			return false;
		}

		m_Callback(pTokenizer.m_Tokens);
		return true;
	}

	bool ConCommand::Find(const std::string& name)
	{
		return ConCommand::GetCommands()->find(String::Lower(name)) != ConCommand::GetCommands()->end();
	}

	std::map<std::string, ConCommand>* ConCommand::GetCommands()
	{
		if (concommands == nullptr) {
			concommands = new std::map<std::string, ConCommand>();
		}
		return concommands;
	}
}