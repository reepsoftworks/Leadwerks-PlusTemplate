#pragma once
#include "LeadwerksPlus.h"

namespace Leadwerks
{
    class WString
    {
    public:
        static std::size_t Find(const std::wstring& s, const std::wstring& token);
        static std::wstring Chr(const wchar_t c);
        static std::wstring Char(const wchar_t* c);
        static std::wstring Right(const std::wstring& s, const std::size_t length);
        static std::wstring Left(const std::wstring& s, const std::size_t length);
        static std::wstring Mid(const std::wstring& s, const int pos, const std::size_t length);
        static std::size_t Length(const std::wstring& s);
        static std::wstring Replace(const std::wstring& s, const std::wstring& searchstring, const std::wstring& replacestring);
        static std::wstring Lower(const std::wstring& s);
        static std::wstring Upper(const std::wstring& s);
        static std::wstring Quote(const std::wstring& s);
        static std::wstring UnQuote(const std::wstring& s);
        static std::wstring XOR(const std::wstring& s, const std::wstring& key);

        static std::wstring LTrim(std::wstring& ss);
        static std::wstring RTrim(std::wstring& ss);
        static std::wstring Trim(std::wstring& ss);
        static uint64_t UInt64(const std::wstring& s);
        static int Int(const std::wstring& s);
        static float Float(const std::wstring& s);
        static std::string ToString(const std::wstring& s);
        static std::vector<std::wstring> Split(const std::wstring& s, const std::wstring& sep);
    };
}