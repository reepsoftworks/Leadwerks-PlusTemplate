//========= Copyright Reep Softworks, All rights reserved. ============//
//
// Purpose: 
//
//=====================================================================//
#include "pch.h"
#include "FileStream.h"

namespace Leadwerks
{
    FileStream::FileStream()
    {
    }
    
    FileStream::~FileStream()
    {
        if (file)
        {
            file.close();
        }
    }
    
    long FileStream::GetPos()
    {
        if (!file) return 0;
        return (long)file.tellp();
    }
    
    void FileStream::Seek(const int pos)
    {
        if (file) file.seekp(pos);
    }

    uint64_t FileStream::GetSize()
    {
        if (!file) return 0;
        return file.tellg();
    }
    
    bool FileStream::EoF()
    {
        if (!file) return true;
        return file.eof();
    }

    void FileStream::Close()
    {
        if (file) file.close();
    }
    
    unsigned char FileStream::ReadUChar()
    {
        char i = 0;
        if (file) file >> i;
        return i;
    }
    
    int FileStream::ReadInt()
    {
        int i = 0;
        if (file) file >> i;
        return i;
    }
    
    short FileStream::ReadShort()
    {
        short s = 0;
        if (file) file >> s;
        return s;
    }
    
    unsigned short FileStream::ReadUShort()
    {
        unsigned short s = 0;
        if (file) file >> s;
        return s;
    }
    
    unsigned int FileStream::ReadUInt()
    {
        unsigned int i = 0;
        if (file) file >> i;
        return i;
    }
    
    unsigned long FileStream::ReadULong()
    {
        unsigned long l = 0;
        if (file) file >> l;
        return l;
    }
    
    long FileStream::ReadLong()
    {
        long l = 0;
        if (file) file >> l;
        return l;
    }
    
    float FileStream::ReadFloat()
    {
        float f = 0.0f;
        if (file) file >> f;
        return f;
    }
    
    std::string FileStream::ReadString(const int maxlength)
    {
        std::string line = "";
        if (file) std::getline(file, line);
        line.erase(maxlength, line.length() - maxlength);
        return line;
    }
    
    std::string FileStream::ReadLine()
    {
        std::string line = "";
        if (file) std::getline(file, line);
        return line;
    }
    
    void FileStream::Read(const void* b, const uint64_t size)
    {
        if (file)
        {
            file.read(reinterpret_cast<char*>(&b), size);
        }
    }
    
    void FileStream::Write(const void* b, const uint64_t size)
    {
        if (file)
        {
            file.write(reinterpret_cast<char*>(&b), size);
        }
    }
    
    void FileStream::WriteUChar(const unsigned char b)
    {
        if (file)
        {
            file << b << endl;
        }
    }
    
    void FileStream::WriteInt(const int n)
    {
        if (file)
        {
            file << n << endl;
        }
    }
    
    void FileStream::WriteFloat(const float f)
    {
        if (file)
        {
            file << f << endl;
        }
    }
    
    void FileStream::WriteString(const std::string& s)
    {
        if (file)
        {
            file << s << endl;
        }
    }
    
    void FileStream::WriteLine(const std::string& s)
    {
        if (file)
        {
            file << s << endl;
        }
    }
    
    void FileStream::WriteUShort(const unsigned short n)
    {
        if (file)
        {
            file << n << endl;
        }
    }

    void FileStream::Flush()
    {
        if (file) file.flush();
    }


    //File factory commands
    /*
    shared_ptr<FileStream> FileStream::WriteFile(const std::string & path)
    {
        auto stream = std::make_shared<FileStream>();
        stream->file.open(path, std::ios::out | std::ios::trunc);
        if (!stream->file.is_open())
        {
            stream->file.clear();
            stream->file.open(path, std::ios::out); // create file
            stream->file.close();
            stream->file.open(path);
        }

        stream->file.seekp(0);
        return stream;
    }

    shared_ptr<FileStream> FileStream::ReadFile(const std::string& path)
    {
        auto stream = std::make_shared<FileStream>();
        stream->file.open(path);
        if (!stream->file.is_open()) return NULL;
        stream->file.seekp(0);
        return stream;
    }

    shared_ptr<FileStream> FileStream::WriteFile(const std::wstring& path)
    {
        auto stream = std::make_shared<FileStream>();
        stream->file.open(path, std::ios::out | std::ios::trunc);
        if (!stream->file.is_open())
        {
            stream->file.clear();
            stream->file.open(path, std::ios::out); // create file
            stream->file.close();
            stream->file.open(path);
        }

        stream->file.seekp(0);
        return stream;
    }

    shared_ptr<FileStream> FileStream::ReadFile(const std::wstring& path)
    {
        auto stream = std::make_shared<FileStream>();
        stream->file.open(path);
        if (!stream->file.is_open()) return NULL;
        stream->file.seekp(0);
        return stream;
    }
    */

    FileStream* FileStream::WriteFile(const std::string& path)
    {
        auto stream = new FileStream();
        stream->file.open(path, std::ios::out | std::ios::trunc);
        if (!stream->file.is_open())
        {
            stream->file.clear();
            stream->file.open(path, std::ios::out); // create file
            stream->file.close();
            stream->file.open(path);
        }

        stream->file.seekp(0);
        return stream;
    }

    FileStream* FileStream::ReadFile(const std::string& path)
    {
        auto stream = new FileStream();
        stream->file.open(path);
        if (!stream->file.is_open()) return NULL;
        stream->file.seekp(0);
        return stream;
    }

    FileStream* FileStream::WriteFile(const std::wstring& path)
    {
        auto stream = new FileStream();
        stream->file.open(path, std::ios::out | std::ios::trunc);
        if (!stream->file.is_open())
        {
            stream->file.clear();
            stream->file.open(path, std::ios::out); // create file
            stream->file.close();
            stream->file.open(path);
        }

        stream->file.seekp(0);
        return stream;
    }

    FileStream* FileStream::ReadFile(const std::wstring& path)
    {
        auto stream = new FileStream();
        stream->file.open(path);
        if (!stream->file.is_open()) return NULL;
        stream->file.seekp(0);
        return stream;
    }
}
