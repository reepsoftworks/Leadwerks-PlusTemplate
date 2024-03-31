//========= Copyright Reep Softworks, All rights reserved. ============//
//
// Purpose: 
//
//=====================================================================//
#pragma once
#include "../LeadwerksPlus.h"

namespace Leadwerks
{   
    class FileStream : public Object
    {
    public:
        std::fstream file;
        
        FileStream();
        virtual ~FileStream();
        
        virtual long GetPos();
        virtual void Seek(const int pos);
        virtual uint64_t GetSize();
        virtual bool EoF();
        virtual void Close();

        virtual unsigned char ReadUChar();
        virtual int ReadInt();
        virtual short ReadShort();
        virtual unsigned short ReadUShort();
        virtual unsigned int ReadUInt();
        virtual unsigned long ReadULong();
        virtual long ReadLong();
        virtual float ReadFloat();
        virtual std::string ReadString(const int maxlength=0);
        virtual std::string ReadLine();
        virtual void Read(const void* b, const uint64_t size);
        virtual void Write(const void* b, const uint64_t size);
        virtual void WriteUChar(const unsigned char b);
        virtual void WriteInt(const int n);
        virtual void WriteFloat(const float f);
        virtual void WriteString(const std::string& s);
        virtual void WriteLine(const std::string& s);
        virtual void WriteUShort(const unsigned short n);
        virtual void Flush();

        //File factory commands
        //static shared_ptr<FileStream> WriteFile(const std::string& path);
        //static shared_ptr<FileStream> ReadFile(const std::string& path);
        //static shared_ptr<FileStream> WriteFile(const std::wstring& path);
        //static shared_ptr<FileStream> ReadFile(const std::wstring& path);

        static FileStream* WriteFile(const std::string& path);
        static FileStream* ReadFile(const std::string& path);
        static FileStream* WriteFile(const std::wstring& path);
        static FileStream* ReadFile(const std::wstring& path);
    };

#if 0
    // JSON 
    extern nlohmann::json LoadJSON(const std::string& path, const int flags = 0);
    extern nlohmann::json LoadJSON(Stream* stream);
    extern bool SaveJSON(nlohmann::json& j3, const std::string& path);

    static nlohmann::json LoadJSON(const std::wstring& path, const int flags = 0);
    static nlohmann::json LoadJSON(shared_ptr<Stream> stream);
    static bool SaveJSON(nlohmann::json& j3, const std::wstring& path);

    // Table
    extern table LoadTable(const std::string& path);
    extern table LoadTable(Stream* stream);
    extern bool SaveTable(table& t, std::string& path);

    //extern table LoadTable(const WString& path);
    //extern table LoadTable(shared_ptr<Stream> stream);
    //extern bool SaveTable(table& t, const WString& path);
#endif
}
