#pragma once
#include "../LeadwerksPlus.h"

namespace Leadwerks
{
	class ConCommand : public Object
	{
	protected:
		std::string name;
	public:
		int flag;
		std::string description;
		std::function<void(std::vector<std::string>)> m_Callback = nullptr;

		ConCommand() {};
		ConCommand(const std::string& commandname, const std::function<void(std::vector<std::string>)>& callback, const int flag = CVAR_DEFAULT, const std::string& description = "");

		virtual std::string GetName() { return name; };
		virtual std::string GetDescription() { return description; };
		virtual bool FireCallback(Tokenizer& pTokenizer);

		static bool Find(const std::string& name);
		static std::map<std::string, ConCommand>* GetCommands();
	};
}