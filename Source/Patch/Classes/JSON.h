#pragma once
#include "LeadwerksPlus.h"

namespace Leadwerks
{
	class FileStream;

	class JSON
	{
	public:
		static nlohmann::json Load(const std::string& path, const int flags = 0);
		static nlohmann::json Load(Stream* stream);
		static bool Save(nlohmann::json& j3, const std::string& path);

		static nlohmann::json Load(const std::wstring& path, const int flags = 0);
		static nlohmann::json Load(shared_ptr<FileStream> stream);
		static bool Save(nlohmann::json& j3, const std::wstring& path);
	};

#ifdef __TABLE_PLUS_PLUS
	class Table
	{
	public:
		static table Load(const std::string& path);
		static table Load(Stream* stream);
		static bool Save(table& t, std::string& path);

		static table Load(const std::wstring& path);
		static table Load(shared_ptr<FileStream> stream);
		static bool Save(table& t, std::wstring& path);
	};
#endif
}