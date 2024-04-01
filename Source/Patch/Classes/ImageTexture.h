#pragma once
#include "../LeadwerksPlus.h"

namespace Leadwerks
{
    class ImageTexture
    {
    public:
        static bool LoadTextureFromFile(const char* path, GLuint* out_texture, int* out_width, int* out_height);
        static Texture* Load(const std::string& path, const int flags = 0);
    };
}