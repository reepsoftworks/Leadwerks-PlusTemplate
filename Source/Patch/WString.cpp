#include "pch.h"
#include "LeadwerksPlus.h"

namespace Leadwerks
{
    static std::string ws2s(const std::wstring& wstr)
    {
#if !defined (__cplusplus) == 201703L
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;

        return converterX.to_bytes(wstr);
#else
        std::string str;
        std::transform(wstr.begin(), wstr.end(), std::back_inserter(str), [](wchar_t c) {
            return (char)c; });

        return str;
#endif
    }

    const static bool WStrIsNumber(const std::wstring& s)
    {
        if (s.empty()) return 0;
        return s.find_first_not_of(L"0123456789") == std::wstring::npos;
    }

    std::size_t WString::Find(const std::wstring& s, const std::wstring& token)
    {
        std::size_t found = s.rfind(token);
        if (found != std::wstring::npos)
        {
            return found;
        }

        return 0;
    }

    std::wstring WString::Chr(const wchar_t c)
    {
        std::wstringstream ss;
        std::wstring s;
        ss << c;
        ss >> s;
        return s;
    }

    std::wstring WString::Char(const wchar_t* c)
    {
        std::wstring value = L"";
        if (c != NULL) value = c;
        return value;
    }

    std::wstring WString::Right(const std::wstring& s, const std::size_t length)
    {
        if (s.empty()) return L"";
        if (s.length() < length) return L"";

        std::wstring new_str;
        new_str = s;
        new_str.erase(0, s.length() - length);
        return new_str;
    }

    std::wstring WString::Left(const std::wstring& s, const std::size_t length)
    {
        if (s.empty()) return L"";
        if (s.length() < length) return L"";

        std::wstring new_str;
        new_str = s;
        new_str.erase(length, s.length() - length);
        return new_str;
    }

    std::wstring WString::Mid(const std::wstring& s, const int pos, const std::size_t length)
    {
        std::wstring new_str = s;
        new_str.erase(0, pos);
        new_str.erase(length, new_str.length());
        return new_str;
    }

    std::size_t WString::Length(const std::wstring& s)
    {
        return s.length();
    }

    std::wstring WString::Replace(const std::wstring& s, const std::wstring& searchstring, const std::wstring& replacestring)
    {
        std::wstring newstr(s);

        /*
        if (!all)
        {
            size_t start_pos = s.find(searchstring);
            if (start_pos == std::wstring::npos)
                return searchstring;
            newstr.replace(start_pos, searchstring.length(), replacestring);
        }
        else
        {
            size_t start_pos = 0;
            while ((start_pos = newstr.find(searchstring, start_pos)) != std::wstring::npos)
            {
                newstr.replace(start_pos, searchstring.length(), replacestring);
                start_pos += replacestring.length();
            }
        }
        */

        size_t start_pos = 0;
        while ((start_pos = newstr.find(searchstring, start_pos)) != std::wstring::npos)
        {
            newstr.replace(start_pos, searchstring.length(), replacestring);
            start_pos += replacestring.length();
        }

        return newstr;
    }

    std::wstring WString::Lower(const std::wstring& s)
    {
        std::wstring new_str = s;
        std::transform(new_str.begin(), new_str.end(), new_str.begin(), ::tolower);
        return new_str;
    }

    std::wstring WString::Upper(const std::wstring& s)
    {
        std::wstring new_str = s;
        std::transform(new_str.begin(), new_str.end(), new_str.begin(), ::toupper);
        return new_str;
    }

    std::wstring WString::Quote(const std::wstring& s)
    {
        return L"\"" + s + L"\"";
    }

    std::wstring WString::UnQuote(const std::wstring& s)
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

    std::wstring WString::XOR(const std::wstring& s, const std::wstring& key)
    {
        std::wstring rtn = s;
        //const wchar_t* orignalString = rtn.c_str();
        wchar_t xorKey = *key.c_str();
        std::size_t len = wcslen(s.c_str());
        for (std::size_t i = 0; i < len; i++)
        {
            rtn[i] = rtn[i] ^ xorKey;
        }

        return rtn;
    }

    std::wstring WString::LTrim(std::wstring& ss)
    {
        std::wstring str = ss;
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int c) {return !std::isspace(c); }));
        return str;
    }

    std::wstring WString::RTrim(std::wstring& ss)
    {
        std::wstring str = ss;
        str.erase(std::find_if(str.rbegin(), str.rend(), [](int c) {return !std::isspace(c); }).base(), str.end());
        return str;
    }

    std::wstring WString::Trim(std::wstring& ss)
    {
        std::wstring str = ss;
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int c) {return !std::isspace(c); }));
        str.erase(std::find_if(str.rbegin(), str.rend(), [](int c) {return !std::isspace(c); }).base(), str.end());
        return str;
    }

    uint64_t WString::UInt64(const std::wstring& s)
    {
        if (!WStrIsNumber(s)) return 0;
        return static_cast<uint64_t>(std::stoi(s));
    }

    int WString::Int(const std::wstring& s)
    {
        if (!WStrIsNumber(s)) return 0;
        return std::stoi(s);
    }

    float WString::Float(const std::wstring& s)
    {
        return strtof(WString::ToString(s).c_str(), nullptr);
    }

    std::string WString::ToString(const std::wstring& s)
    {
        return ws2s(s);
    }

    std::vector<std::wstring> WString::Split(const std::wstring& s, const std::wstring& sep)
    {
        std::wstring str = s;
        std::vector<std::wstring> result;
        while (str.size()) {
            std::size_t index = str.find(sep);
            if (index != std::wstring::npos) {
                result.push_back(str.substr(0, index));
                str = str.substr(index + sep.size());
                if (str.size() == 0) result.push_back(str);
            }
            else {
                result.push_back(str);
                str = L"";
            }
        }
        return result;
    }
}