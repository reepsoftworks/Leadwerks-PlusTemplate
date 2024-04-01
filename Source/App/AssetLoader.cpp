#include "pch.h"
#include "App.h"

namespace App
{
    using namespace Leadwerks;

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
            Print("Creating fallback material!");
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

            // Color for the first square
            int r1 = 0;
            int g1 = 0;
            int b1 = 0;
            int a1 = 0;

            for (int w = 0; w < fallbackskybox->GetWidth(); w++)
            {
                for (int h = 0; h < fallbackskybox->GetHeight(); h++)
                {
                    fallbackskybox->WritePixel(w, h, r1, g1, b1, a1, 0, 0);
                }
            }
        }

        return fallbackskybox;
    }

    static Material* fallbackmaterial = NULL;
	Leadwerks::Material* AssetLoader::DefaultMaterial()
	{
        if (!fallbackmaterial)
        {
            fallbackmaterial = Material::Create();

            auto texture = DefaultTexture();
            if (texture) fallbackmaterial->SetTexture(texture);

            auto shader = Shader::Load("Shaders/Model/Flat/default.shader");
            if (!shader)
            {
                Debug::Error("Error: Failed to load shader " + QuoteString("Shaders/Model/Flat/default.shader") + "....");
            }

            fallbackmaterial->SetShader(shader);
        }

        return fallbackmaterial;
	}

    static Model* fallbackmodel = NULL;
    static Leadwerks::Model* DefaultModel()
    {
        if (!fallbackmodel)
        {
            fallbackmodel = Model::Box();
            auto mat = AssetLoader::DefaultMaterial();
            if (mat) fallbackmodel->SetMaterial(mat);    
        }

        return fallbackmodel;
    }

    Font* AssetLoader::LoadFont(const std::string& path, const int size, const int style, const int family, const int flags, const uint64_t fileid)
    {
        // Load the font normally.
        auto font = Font::Load(path, size, style, family, flags, fileid);

        // If the pointer is vaild, return here.
        if (font)
        {
            FireCallback(CALLBACK_ASSETLOADED, NULL, font);
            return font;
        }

        // Ok, we didn't find the font. Maybe it's in the system folder?
        auto file = FileSystem::StripDir(path);
        font = LoadSystemFont(file, size, style, flags);
        if (font)
        {
            FireCallback(CALLBACK_ASSETLOADED, NULL, font);
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
            FireCallback(CALLBACK_ASSETLOADED, NULL, font);
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
            FireCallback(CALLBACK_ASSETLOADED, NULL, font);
            return font;
        }

        // Ok, we didn't find the font. Maybe it's in the system folder?
        auto file = FileSystem::StripDir(path);
        font = LoadSystemFont(file, size, style, flags);
        if (font)
        {
            FireCallback(CALLBACK_ASSETLOADED, NULL, font);
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
            FireCallback(CALLBACK_ASSETLOADED, NULL, texture);
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
        if (texture) return texture;

        // Return the fallback instead!
        return DefaultMaterial();
	}

	Leadwerks::Model* AssetLoader::LoadModel(const std::string& path, const int flags, const uint64_t fileid)
	{
        // Load the model normally.
        auto mdl = Model::Load(path, flags, fileid);

        // If the pointer is vaild, return here.
        if (mdl) return mdl;

        return DefaultModel();
	}

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
}