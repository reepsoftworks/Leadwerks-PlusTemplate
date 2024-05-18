#include "pch.h"
#include "LeadwerksPlus.h"

namespace Leadwerks
{
    nlohmann::json JSON::Load(const std::string& path, const int flags)
    {
        auto stream = Leadwerks::FileSystem::ReadFile(path);
        auto realpath = Leadwerks::FileSystem::RealPath(path);

        if (stream == NULL)
        {
            Debug::Error("Failed to load JSON script \"" + realpath + "\"...");
            return NULL;
        }

        //Check for starting bracket
        bool started = false;

        while (!stream->EOF())
        {
            auto c = Leadwerks::String::Chr(stream->ReadUChar());
            if (Leadwerks::String::Trim(c) == "") continue;
            if (c != "{") return false;
            started = true;
            break;
        }

        if (!started) return false;
        stream->Seek(0);

        std::vector<uint8_t> data;
        auto sz = stream->GetSize();
        data.resize(sz);
        stream->Read(data.data(), sz);

        nlohmann::json j3;
        try
        {
            j3 = nlohmann::json::parse(data);
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
            return false;
        }
        if (j3.type() != nlohmann::json::value_t::object) return NULL;

        stream->Release();
        stream = NULL;
        return j3;
    }

    nlohmann::json JSON::Load(Stream* stream)
    {
        if (stream == NULL)
        {
            Debug::Error("Failed to load JSON script from stream...");
            return NULL;
        }

        //Check for starting bracket
        bool started = false;

        while (!stream->EOF())
        {
            auto c = Leadwerks::String::Chr(stream->ReadUChar());
            if (Leadwerks::String::Trim(c) == "") continue;
            if (c != "{") return false;
            started = true;
            break;
        }

        if (!started) return false;
        stream->Seek(0);

        std::vector<uint8_t> data;
        auto sz = stream->GetSize();
        data.resize(sz);
        stream->Read(data.data(), sz);

        nlohmann::json j3;
        try
        {
            j3 = nlohmann::json::parse(data);
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
            return false;
        }
        if (j3.type() != nlohmann::json::value_t::object) return NULL;

        stream->Release();
        stream = NULL;
        return j3;
    }

    bool JSON::Save(nlohmann::json& j3, const std::string& path)
    {
        std::ofstream o(path);
        o << std::setw(4) << j3 << std::endl;

        return (bool)FileSystem::GetFileType(path);
    }

    nlohmann::json JSON::Load(const std::wstring& path, const int flags)
    {
        auto stream = FileStream::ReadFile(path);
        //auto realpath = FileSystem::RealPath(path);

        if (stream == NULL)
        {
            //Debug::Error("Failed to load JSON script \"" + realpath + "\"...");
            return NULL;
        }

        //Check for starting bracket
        bool started = false;

        while (!stream->EoF())
        {
            auto c = String::Chr(stream->ReadUChar());
            if (String::Trim(c) == "") continue;
            if (c != "{") return false;
            started = true;
            break;
        }

        if (!started) return false;
        stream->Seek(0);

        std::vector<uint8_t> data;
        auto sz = stream->GetSize();
        data.resize(sz);
        stream->Read(data.data(), sz);

        nlohmann::json j3;
        try
        {
            j3 = nlohmann::json::parse(data);
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
            return false;
        }
        if (j3.type() != nlohmann::json::value_t::object) return NULL;

        stream = NULL;
        return j3;
    }

    nlohmann::json JSON::Load(shared_ptr<FileStream> stream)
    {
        if (stream == NULL)
        {
            Debug::Error("Failed to load JSON script from stream...");
            return NULL;
        }

        //Check for starting bracket
        bool started = false;

        while (!stream->EoF())
        {
            auto c = String::Chr(stream->ReadUChar());
            if (String::Trim(c) == "") continue;
            if (c != "{") return false;
            started = true;
            break;
        }

        if (!started) return false;
        stream->Seek(0);

        std::vector<uint8_t> data;
        auto sz = stream->GetSize();
        data.resize(sz);
        stream->Read(data.data(), sz);

        nlohmann::json j3;
        try
        {
            j3 = nlohmann::json::parse(data);
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
            return false;
        }
        if (j3.type() != nlohmann::json::value_t::object) return NULL;

        stream = NULL;
        return j3;
    }

    bool JSON::Save(nlohmann::json& j3, const std::wstring& path)
    {
        std::ofstream o(path);
        o << std::setw(4) << j3 << std::endl;
        return o.good();
    }

#ifdef __TABLE_PLUS_PLUS
    table Table::Load(const std::string& path)
    {
        auto j3 = JSON::Load(path);
        table t(j3);
        return t;
    }

    table Table::Load(Stream* stream)
    {
        auto j3 = JSON::Load(stream);
        table t(j3);
        return t;
    }

    bool Table::Save(table& t, const std::string& path)
    {
        std::string s = t.to_json();
        auto stream = FileSystem::WriteFile(path);
        if (stream == NULL) return false;
        stream->WriteLine(s);
        stream->Release();
        stream = NULL;
        return true;
    }

    table Table::Load(const std::wstring& path)
    {
        auto j3 = JSON::Load(path);
        table t(j3);
        return t;
    }

    table Table::Load(shared_ptr<FileStream> stream)
    {
        auto j3 = JSON::Load(stream);
        table t(j3);
        return t;
    }

    bool Table::Save(table& t, const std::wstring& path)
    {
        std::string s = t.to_json();
        auto stream = FileStream::WriteFile(path);
        if (stream == NULL) return false;
        stream->WriteLine(s);
        stream->Release();
        stream = NULL;
        return true;
    }
#endif
}