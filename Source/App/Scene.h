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
		Leadwerks::Timer* delayload;
		bool cleartoload;
		bool isloading;
		float gravity;

		static bool EventCallback(const Leadwerks::Event& e, Leadwerks::Object* extra);
		void PostLoadModifications();

	public:
		Scene();
		virtual ~Scene();

		void Update();//lua
		long GetTime();//lua
		float GetSpeed();//lua
		void SetPauseState(const bool pause);//lua
		bool GetPauseState();//lua
		void SetGravity(const float value);//lua

		void Clear();//lua
		void LoadMap(std::string& path);//lua
		bool InMap();//lua
		bool IsLoading();//lua
		void WriteEntityID(Leadwerks::Entity* entity);//lua
		std::string GetEntityID(Leadwerks::Entity* entity);//lua

		void Load(const std::wstring& path);
		void Save(const std::wstring& path);
		
		Leadwerks::World* GetWorld();//lua
		Leadwerks::Camera* GetCamera();//lua

		void SetLoadingBackground(Leadwerks::Texture* texture);//lua
		Leadwerks::Texture* GetLoadingBackground();//lua
		void DrawLoadingScreen(const bool sync = true);

		static Scene* currentscene;
		static Scene* GetCurrent();//lua

		static Scene* Create();//lua

		static std::string nextmaptoload;
		virtual bool ProcessEvent(const Leadwerks::Event& e);
	};
}