#pragma once
#include "../LeadwerksPlus.h"

namespace Leadwerks
{
	class Config : public Object
	{
		table data;
		virtual void ParseStream(const std::string& filepath);
		virtual void ParseStream(const std::wstring& filepath);

	public:
		Config();
		virtual ~Config();

		//void WriteValue(std::string& key, table value);
		table GetValue(const std::string& key);


		static Config* Load(const std::string& filepath);
		static Config* Load(const std::wstring& filepath);
	};
}