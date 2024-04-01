#include "pch.h"
#include "../LeadwerksPlus.h"

#define EOF -1
#define STB_IMAGE_IMPLEMENTATION
#include "Libraries/stb/stb_image.h"
#undef EOF

namespace Leadwerks
{
    //https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#example-for-opengl-users
	bool ImageTexture::LoadTextureFromFile(const char* path, GLuint* out_texture, int* out_width, int* out_height)
	{
        Print("Loading texture " + QuoteString(FileSystem::RealPath(path)) + "...");

        // Load from file
        int image_width = 0;
        int image_height = 0;
        unsigned char* image_data = stbi_load(path, &image_width, &image_height, NULL, 4);
        if (image_data == NULL)
        {
            Print("Error: Failed to loading texture " + QuoteString(FileSystem::RealPath(path)) + "...");
            return false;
        }

        // Create a OpenGL texture identifier
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

        // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        stbi_image_free(image_data);

        *out_texture = image_texture;
        *out_width = image_width;
        *out_height = image_height;

        return true;
	}

	Texture* ImageTexture::Load(const std::string& path, const int flags)
	{
		Print("Loading texture " + QuoteString(FileSystem::RealPath(path)) + "...");

		int image_width = 0;
		int image_height = 0;
		unsigned char* image_data = stbi_load(path.c_str(), &image_width, &image_height, NULL, 4);
        if (image_data == NULL)
        {
            Print("Error: Failed to loading texture " + QuoteString(FileSystem::RealPath(path)) + "...");
            return false;
        }

        if (path == FileSystem::ExtractExt("hdr"))
        {
            // Unsuppored.
            // It's a lot of work Just use this tool.
            //https://matheowis.github.io/HDRI-to-CubeMap/
            //
            Print("Error: Unable to load " + QuoteString(FileSystem::RealPath(path)) + " as hdr files aren't supported...");
            return false;
        }
 
        Texture* texture = Texture::Create(image_width, image_height);
        texture->SetPixels(reinterpret_cast<const char*>(image_data));	
        texture->path = path;
		stbi_image_free(image_data);
		return texture;
	}
}