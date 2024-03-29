#include "pch.h"
#include "LeadwerksPlus.h"

#ifdef _WIN32
#include <windows.h>

//Hack to force NVidia discrete card when integrated graphics are present:
//http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf -  Page 3
extern "C"
{
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

#endif

namespace Leadwerks
{
	void Print(const std::wstring& s)
	{
		std::wcout << s << std::endl;

		if (logmode)
		{
			std::string ss = WString::ToString(s);
			logstream__->WriteLine(ss);
		}
	}

	std::wstring IntToWString(int i)
	{
		return std::to_wstring(i);
	}

	std::wstring WString(double f)
	{
		return std::to_wstring(f);
	}

	std::wstring WString(int i)
	{
		return std::to_wstring(i);
	}

	std::wstring WString(unsigned int i)
	{
		return std::to_wstring(i);
	}

	std::wstring WString(long l)
	{
		return std::to_wstring(l);
	}

	std::wstring WString(uint64_t i)
	{
		return std::to_wstring(i);
	}

	std::wstring WString(const std::string s)
	{
#if !defined (__cplusplus) == 201703L
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
#else
		std::wstring wstr;
		std::transform(s.begin(), s.end(), std::back_inserter(wstr), [](char c) {
			return (char)c; });

		return wstr;
#endif
	}

	std::string QuoteString(const std::string& s)
	{
		return "\"" + s + "\"";
	}

	std::string UnQuoteString(const std::string& s)
	{
		auto ret = s;
		auto len = s.length();
		if (ret[0] == '"' && ret[len - 1] == '"')
		{
			ret = String::Left(ret, (int)ret.length() - 1);
			ret = String::Right(ret, (int)ret.length() - 1);
		}
		return ret;
	}

	std::wstring QuoteWString(const std::wstring& s)
	{
		return L"\"" + s + L"\"";
	}

	std::wstring UnQuoteWString(const std::wstring& s)
	{
		auto ret = s;
		auto len = s.length();
		if (ret[0] == '"' && ret[len - 1] == '"')
		{
			ret = WString::Left(ret, (int)ret.length() - 1);
			ret = WString::Right(ret, (int)ret.length() - 1);
		}
		return ret;
	}

	void FreeObject(Object* o)
	{
		if (o)
		{
			o->Release();
			o = NULL;
		}
	}

	void FreeObject(shared_ptr<SmartObject> o)
	{
		if (o) o = NULL;
	}

	bool ExecuteCommand(const std::string& input, const bool quiet)
	{
		if (input.empty()) return false;

		Tokenizer tokenizer(input, true);
		std::string m_Name;

		if (tokenizer.m_Tokens.empty()) 
		{
			m_Name = String::Lower(input);
			m_Name = String::Trim(m_Name);
		}
		else { m_Name = String::Lower(tokenizer.m_First); }

		bool ret = false;

		if (ConCommand::Find(m_Name))
		{
			auto cmd = (*ConCommand::GetCommands())[m_Name];
			ret = cmd.FireCallback(tokenizer);
		}
		else
		{
			if (ConVar::Find(m_Name))
			{
				auto cmd = (*ConVar::GetConVars())[m_Name];
				if (cmd && !tokenizer.m_Tokens.empty())
				{
					auto value = tokenizer.m_Tokens[0];
					cmd->SetValue(value);
					if (!quiet) Print("ConVar \"" + m_Name + "\" has been set to the value of \"" + cmd->GetValue() + "\"...");
				}

				ret = true;
			}

			if (!ret && !quiet) Print("Error: Failed to locate existant convar or command \"" + m_Name + "\"...");
		}

		return ret;
	}
}