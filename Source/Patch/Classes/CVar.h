#pragma once
#include "../LeadwerksPlus.h"

namespace Leadwerks
{
	class ConVar : public Object
	{
		std::string name;
		std::string value;
		std::string description;
		int flag;
	public:
		std::function<void(std::string)> m_Callback = nullptr;

		ConVar() {};
		ConVar(const std::string& name, const std::string& value, const int flag = CVAR_DEFAULT, const std::string& description = "", std::function<void(std::string)> callback = nullptr);

		virtual void SetValue(const std::string& value);

		bool GetBool();
		uint64_t GetInt();
		float GetFloat();
		std::string GetValue();

		const std::string GetName() { return name; };
		const int GetFlag() { return flag; };
		const std::string GetDescription() { return description; };

		static bool Find(const std::string& name);
		static std::map<std::string, ConVar*>* GetConVars();
		static bool Load(std::string path);
		static bool Save(std::string path);
	};

	template <typename T> void SetConVar(const std::string& name, const T value)
	{
		if (ConVar::Find(name))
		{
			auto cmd = (*ConVar::GetConVars())[name];
			if (cmd) cmd->SetValue(value);
		}
	}
}