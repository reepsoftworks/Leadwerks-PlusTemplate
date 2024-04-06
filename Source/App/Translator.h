#pragma once
#include "App.h"

namespace App
{
	class Translator : public Leadwerks::Object
	{
	public:
		static void SetLanguage(const std::string& language);

		static void LoadTokenList(const std::string& path);
		static std::string TranslateToken(const std::string& token, const std::string& language = "english");
		static void Clear();
	};
}