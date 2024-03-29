#pragma once
#include "pch.h"
#include "../LeadwerksPlus.h"

namespace Leadwerks
{
	extern bool cheating;
	std::map<std::string, ConVar*>* convars = nullptr;
	ConVar::ConVar(const std::string& name, const std::string& value, const int flag, const std::string& description, std::function<void(std::string)> callback)
	{
		this->name = name;
		this->value = value;
		this->flag = flag;
		this->description = description;
		this->m_Callback = callback;
		GetConVars()->insert({ name, this });
	}

	void ConVar::SetValue(const std::string& value)
	{
		if (!cheating && flag == CVAR_CHEAT)
		{
			Print("ConVar \"" + name + "\" is a registered cheat convar. Please enable cheats first.");
		}
		else
		{
			this->value = value;
			m_Callback(this->value);
		}
	}

	bool ConVar::GetBool()
	{
		return (bool)String::Int(value);
	}

	uint64_t ConVar::GetInt()
	{
		return String::Int(value);
	}

	float ConVar::GetFloat()
	{
		return (float)String::Float(value);
	}

	std::string ConVar::GetValue()
	{
		return value;
	}

	bool ConVar::Find(const std::string& name)
	{
		return ConVar::GetConVars()->find(String::Lower(name)) != ConVar::GetConVars()->end();
	}

	std::map<std::string, ConVar*>* ConVar::GetConVars()
	{
		if (convars == nullptr) {
			convars = new std::map<std::string, ConVar*>();
		}
		return convars;
	}

	bool ConVar::Load(std::string path)
	{
		auto ext = FileSystem::ExtractExt(path);

		if (ext != "json")
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
					auto key = String::Trim(UnQuoteString(ln[0]));
					auto val = String::Trim(UnQuoteString(ln[1]));
					SetConVar(key, val);
				}
			}

			stream = NULL;
		}
		else
		{
			// JSON file
			auto t = Table::Load(path);
			if (t.is_null()) return false;
			if (t["convars"].is_object())
			{
				for (const auto p : t["convars"])
				{
					auto key = String::Trim(UnQuoteString(p.first));
					auto val = String::Trim(UnQuoteString(p.second));
					SetConVar(key, val);
				}
			}

			t = NULL;
		}

		return true;
	}

	bool ConVar::Save(std::string path)
	{
		auto ext = FileSystem::ExtractExt(path);
		bool ret = false;

		if (ext != "json")
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
			ret = true;
		}
		else
		{
			// JSON file
			table t;
			t["convars"] = {};
			for (const auto& a : *ConVar::GetConVars())
			{
				auto name = a.first;
				auto val = a.second->GetValue();
				auto flag = a.second->GetFlag();

				if (flag == CVAR_SAVE)
				{
					t["convars"][name] = val;
				}

				ret = Table::Save(t, path);
			}
		}

		return ret;
	}
}