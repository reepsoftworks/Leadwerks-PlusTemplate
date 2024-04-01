#pragma once
#include "pch.h"

namespace App
{
	class AssetLoader//lua
	{
	public:
		static Leadwerks::Font* LoadFont(const std::string& path, const int size, const int style = Leadwerks::Font::Smooth, const int family = Leadwerks::Font::English, const int flags = 0, const uint64_t fileid = 0);//lua
		static Leadwerks::Font* LoadFont(Leadwerks::Stream* stream, const int size, const int style = Leadwerks::Font::Smooth, const int family = Leadwerks::Font::English, const int flags = 0);//lua
		static Leadwerks::Font* LoadFont(const std::string& path, const int size, const int style, const std::string& familychars, const int flags = 0);//lua
		static Leadwerks::Font* LoadSystemFont(const std::string& path, const int size, const int style, const int flags);//lua

		static Leadwerks::Texture* LoadTexture(const std::string& path, const int flags = 0, const uint64_t fileid = 0);//lua
		static Leadwerks::Material* LoadMaterial(const std::string& path, const int flags = 0, const uint64_t fileid = 0);//lua
		static Leadwerks::Model* LoadModel(const std::string& path, const int flags, const uint64_t fileid = 0);//lua

		static Leadwerks::Texture* DefaultTexture();
		static Leadwerks::Texture* AssetLoader::DefaultSkybox();
		static Leadwerks::Material* DefaultMaterial();

		static void HighlightMissingMaterial(Leadwerks::Entity* entity);
	};
}