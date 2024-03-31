#include "pch.h"
#include "../LeadwerksPlus.h"

namespace Leadwerks
{
	Config::Config()
	{
		data = table();
		data = {};
	}

	Config::~Config()
	{
		data.clear();
		data = NULL;
	}

	void Config::ParseStream(const std::string& filepath)
	{
		auto stream = FileStream::ReadFile(filepath);
		if (stream)
		{
			while (!stream->EoF())
			{
				auto line = stream->ReadLine();
				if (line[0] == '/' && line[1] == '/') continue;

				// Get the key.
				auto kv = String::Split(line, "=");
				if (kv.empty()) continue;

				auto key = kv[0];
				if (!key.empty())
				{
					auto value = kv[1];

					// Check to see is the value isn't a list of somesorts.
					auto list = String::Split(value, ",");
					if (list.empty())
					{
						data[key] = {};
						for (const auto& p : list)
						{
							data[key].push_back(p);
						}
					}
					else
					{
						data[key] = value;
					}
				}
			}
		}
	}

	void Config::ParseStream(const std::wstring& filepath)
	{
		auto stream = FileStream::ReadFile(filepath);
		if (stream)
		{
			data = {};
			while (!stream->EoF())
			{
				auto line = stream->ReadLine();
				if (line[0] == '/' && line[1] == '/') continue;

				// Get the key.
				auto kv = String::Split(line, "=");
				if (kv.empty()) continue;

				auto key = kv[0];
				if (!key.empty())
				{
					auto value = kv[1];

					// Check to see is the value isn't a list of somesorts.
					auto list = String::Split(value, ",");
					if (list.empty())
					{
						data[key] = {};
						for (const auto& p : list)
						{
							data[key].push_back(p);
						}
					}
					else
					{
						data[key] = value;
					}
				}
			}
		}
	}

	/*
	void Config::WriteValue(std::string& key, table value)
	{
		data[key] = value;
	}
	*/

	table Config::GetValue(const std::string& key)
	{
		return data[key];
	}

	Config* Config::Load(const std::string& filepath)
	{
		auto cfg = new Config();
		cfg->ParseStream(filepath);
		return cfg;
	}

	Config* Config::Load(const std::wstring& filepath)
	{
		auto cfg = new Config();
		cfg->ParseStream(filepath);
		return cfg;
	}
}