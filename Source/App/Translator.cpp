#include "pch.h"
#include "App.h"

namespace App
{
	using namespace Leadwerks;

	static std::string currentlanguage = "english";
	static std::multimap<std::string, std::map<string,string>> tokenlists;

	void Translator::SetLanguage(const std::string& language)
	{
		currentlanguage = String::Lower(language);
		EmitEvent(EVENT_LANGUAGE, NULL);
	}

	void Translator::LoadTokenList(const std::string& path)
	{
		auto j3 = JSON::Load(path);
		if (j3.is_object())
		{
			auto lang = currentlanguage;
			if (!j3["language"].is_string())
				return;

			lang = j3["language"];
			std::map<string, string> list;

			if (j3["tokens"].is_object())
			{
				for (const auto& p : j3["tokens"].items())
				{
					std::string key = p.key();
					std::string value = p.value();
					list[key] = value;
				}
			}

			tokenlists.insert(pair<std::string, std::map<string, string>>(String::Lower(lang), list));
		}
	}

	std::string Translator::TranslateToken(const std::string& token, const std::string& language)
	{
		string ret = "";
		for (const auto& i : tokenlists)
		{
			if (language == currentlanguage)
			{
				std::map<string, string> map = i.second;
				ret = map[token];
			}
		}
		if (ret.empty()) return token;
		return ret;
	}

	void Translator::Clear()
	{
		tokenlists.clear();
	}
}