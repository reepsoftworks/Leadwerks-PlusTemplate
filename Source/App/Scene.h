#pragma once
#include "App.h"

namespace App
{
	struct MapData
	{
		std::string path;
		uint64_t filetime;

		void clear()
		{
			path.clear();
			filetime = 0;
		}

		bool empty()
		{
			return path.empty();
		}
	};

	class Scene : public Leadwerks::Object//lua
	{
		Leadwerks::World* world;
		//std::string currentmappath;
		MapData mapdata;
		std::vector<Leadwerks::Entity*> entities;
		Leadwerks::Texture* loadingbackground;

		void DrawLoadingScreen();
	public:
		Scene();
		virtual ~Scene();

		void Update();
		long GetTime();
		float GetSpeed();
		void SetPauseState(const bool pause);
		bool GetPauseState();

		void Clear();
		void LoadMap(std::string& path);
		bool InMap();
		void WriteEntityID(Leadwerks::Entity* entity);
		std::string GetEntityID(Leadwerks::Entity* entity);

		void Load(const std::wstring& path);
		void Save(const std::wstring& path);

		Leadwerks::World* GetWorld();//lua
		Leadwerks::Camera* GetCamera();

		static Scene* currentscene;
		static Scene* GetCurrent();//lua

		static Scene* Create();
	};
}