#include "pch.h"
#include "../LeadwerksPlus.h"

#define EOF -1
#define STB_IMAGE_IMPLEMENTATION
#include "Libraries/stb/stb_image.h"
#undef EOF

namespace Leadwerks
{
    using namespace Leadwerks;

    class ImageTexture
    {
    public:
        static bool LoadTextureFromFile(const char* path, GLuint* out_texture, int* out_width, int* out_height);
        static Texture* Load(const std::string& path, const int flags = 0);
    };

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


    std::string GetSystemFontDir()
    {
        std::string path = "";
#ifdef _WIN32
        std::string syspath = getenv("WINDIR");
        path = FileSystem::RealPath(syspath);
        path = path + "/Fonts";
#else
        // Not sure what to do here for non-windows atm....
#endif
        return path;
    }

    Font* AssetLoader::LoadSystemFont(const std::string& path, const int size, const int style, const int flags)
    {
        auto fontpath = FileSystem::StripDir(path);
        fontpath = GetSystemFontDir() + "/" + fontpath;
        auto font = Font::Load(path, size, style, Font::English, flags);
        return font;
    }

	static Texture* fallbacktexture = NULL;
	Texture* AssetLoader::DefaultTexture()
	{
		if (fallbacktexture == NULL)
		{
            //Print("Creating fallback texture!");
            fallbacktexture = Texture::Create(8, 8);

            // Color for the first square
            int r1 = 0;
            int g1 = 0;
            int b1 = 0;
            int a1 = 255;

            // Color for the second square
            int r2 = 255;
            int g2 = 0;
            int b2 = 255;
            int a2 = 255;

            for (int w = 0; w < fallbacktexture->GetWidth(); w++)
            {
                for (int h = 0; h < fallbacktexture->GetHeight(); h++)
                {
                    // Alternate between the two colors based on position
                    if ((w + h) % 2 == 0)
                    {
                        fallbacktexture->WritePixel(w, h, r1, g1, b1, a1, 0, 0);
                    }
                    else
                    {
                        fallbacktexture->WritePixel(w, h, r2, g2, b2, a2, 0, 0);
                    }
                }
            }
		}

        return fallbacktexture;
	}

    static Texture* fallbackskybox = NULL;
    Texture* AssetLoader::DefaultSkybox()
    {
        if (fallbackskybox == NULL)
        {
            fallbackskybox = Texture::Create(8, 8);
            for (int w = 0; w < fallbackskybox->GetWidth(); w++)
            {
                for (int h = 0; h < fallbackskybox->GetHeight(); h++)
                {
                    fallbackskybox->WritePixel(w, h, 0, 0, 0, 1, 0, 0);
                }
            }
        }

        return fallbackskybox;
    }

   
	Leadwerks::Material* AssetLoader::DefaultMaterial()
	{
        static Material* fallbackmaterial = Material::Create();
        return fallbackmaterial;
	}

    Font* AssetLoader::LoadFont(const std::string& path, const int size, const int style, const int family, const int flags, const uint64_t fileid)
    {
        // Load the font normally.
        auto font = Font::Load(path, size, style, family, flags, fileid);

        // If the pointer is vaild, return here.
        if (font)
        {
            //FireCallback(CALLBACK_ASSETLOADED, NULL, font);
            return font;
        }

        // Ok, we didn't find the font. Maybe it's in the system folder?
        auto file = FileSystem::StripDir(path);
        font = LoadSystemFont(file, size, style, flags);
        if (font)
        {
            //FireCallback(CALLBACK_ASSETLOADED, NULL, font);
            return font;
        }

        // If we made it here, we're out of options. Just return NULL.
        return NULL;
    }

    Font* AssetLoader::LoadFont(Stream* stream, const int size, const int style, const int family, const int flags)
    {
        // Load the font normally.
        auto font = Font::Load(stream, size, style, family, flags);

        // If the pointer is vaild, return here.
        if (font)
        {
            //FireCallback(CALLBACK_ASSETLOADED, NULL, font);
            return font;
        }

        // If we made it here, we're out of options. Just return NULL.
        return NULL;
    }

    Font* AssetLoader::LoadFont(const std::string& path, const int size, const int style, const std::string& familychars, const int flags)
    {
        // Load the font normally.
        auto font = Font::Load(path, size, style, flags);

        // If the pointer is vaild, return here.
        if (font)
        {
            //FireCallback(CALLBACK_ASSETLOADED, NULL, font);
            return font;
        }

        // Ok, we didn't find the font. Maybe it's in the system folder?
        auto file = FileSystem::StripDir(path);
        font = LoadSystemFont(file, size, style, flags);
        if (font)
        {
            //FireCallback(CALLBACK_ASSETLOADED, NULL, font);
            return font;
        }

        // If we made it here, we're out of options. Just return NULL.
        return NULL;
    }


	Leadwerks::Texture* AssetLoader::LoadTexture(const std::string& path, const int flags, const uint64_t fileid)
	{
        // Load the texture normally.
        Leadwerks::Texture* texture;
        auto ext = FileSystem::ExtractExt(path);
        if (ext == "tex")
            texture = Texture::Load(path);
        else
            texture = ImageTexture::Load(path);

        // If the pointer is vaild, return here.
        if (texture)
        {
            //FireCallback(CALLBACK_ASSETLOADED, NULL, texture);
            return texture;
        }

        // Return the fallback instead!
        return DefaultTexture();
	}

	Leadwerks::Material* AssetLoader::LoadMaterial(const std::string& path, const int flags, const uint64_t fileid)
	{
        // Load the material normally.
        auto texture = Material::Load(path, flags, fileid);

        // If the pointer is vaild, return here.
        //if (texture) return texture;

        // Return the fallback instead!
        return texture;
	}

    Leadwerks::Model* AssetLoader::DefaultModel = NULL;
	Leadwerks::Model* AssetLoader::LoadModel(const std::string& path, const int flags, const uint64_t fileid)
	{
        // Load the model normally.
        auto mdl = Model::Load(path, flags, fileid);

        // If the pointer is vaild, return here.
        if (mdl) return mdl;

        if (!DefaultModel)
        {
            Print("Warning: No fallback asset for models is defined!");
            DefaultModel = Model::Box();
        }
        return DefaultModel;
	}

    Leadwerks::Sound* AssetLoader::DefaultSound;
    Leadwerks::Sound* AssetLoader::LoadSound(const std::string& path, const int flags, const uint64_t fileid)
    {
        // Load the model normally.
        auto snd = Sound::Load(path, flags, fileid);

        // If the pointer is vaild, return here.
        if (snd) return snd;

        if (!DefaultSound) Debug::Error("Error: No fallback asset for sounds is defined!");
        return DefaultSound;
    }

    /*
    void AssetLoader::HighlightMissingMaterial(Entity* entity)
    {
        if (!entity) return;

        auto mat = DefaultMaterial();
        if (!mat) return;

        if (entity->GetClass() == Object::ModelClass)
        {
            auto mdl = dynamic_cast<Model*>(entity);
            if (mdl)
            {
                for (int i = 0; i < mdl->CountSurfaces(); i++)
                {
                    Leadwerks::Surface* surface = mdl->GetSurface(i);
                    if (surface != NULL)
                    {
                        auto mat_on_surf = surface->GetMaterial();
                        if (mat_on_surf == NULL) surface->SetMaterial(mat);
                    }
                }
            }
        }
        else if (entity->GetClass() == Object::BrushClass)
        {
            auto brush = dynamic_cast<Brush*>(entity);
            if (brush)
            {
                for (int i = 0; i < brush->CountFaces(); i++)
                {
                    Leadwerks::Face* surface = brush->GetFace(i);
                    if (surface != NULL)
                    {
                        auto mat_on_surf = surface->GetMaterial();
                        if (mat_on_surf == NULL) surface->SetMaterial(mat);
                    }
                }
            }
        }
        else if (entity->GetClass() == Object::SpriteClass)
        {
            auto sprite = dynamic_cast<Sprite*>(entity);
            if (sprite)
            {
                sprite->SetMaterial(mat);
            }
        }
    }
    */
}