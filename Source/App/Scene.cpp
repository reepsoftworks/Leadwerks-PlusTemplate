#include "pch.h"
#include "App.h"
#include "SceneActor.h"
#include "../ActorSystem.h"

namespace App
{
	using namespace Leadwerks;

	static std::string currentmap = "";
	static void MapHook(Entity* entity, Object* extra = NULL)
	{
		auto scene = Scene::GetCurrent();
		if (scene)
		{
			RegisterActors(entity);
			auto actor = dynamic_cast<SceneActor*>(entity->actor);
			if (actor)
			{
				actor->Load();
			}

			scene->WriteEntityID(entity);
		}

		AssetLoader::HighlightMissingMaterial(entity);
	}

	static bool LoadMapFile(const std::string& path, Object* extra = NULL)
	{
		bool result = Map::Load(path, MapHook, extra);
		if (!result)
		{
			Print("Error: Failed to load " + QuoteString(path) + "...");
			currentmap.clear();
			FireCallback(CALLBACK_MAPLOADFAILED, extra, NULL);

		}
		return result;
	}

	void CC_Map(std::vector<std::string> pArgV)
	{
		if (pArgV.size() <= 0)
			return;

		if (pArgV[0].empty())
			return;

		auto scene = Scene::GetCurrent();
		if (scene) scene->LoadMap(pArgV[0]);
	}
	static ConCommand map("map", CC_Map, CVAR_DEFAULT, "Usage: map <mapname/mappath>");

	void CC_Disconnect(std::vector<std::string> pArgV)
	{
		auto scene = Scene::GetCurrent();
		if (scene) scene->Clear();
	}
	static ConCommand disconnect("disconnect", CC_Disconnect, CVAR_DEFAULT);

	void CC_Save(std::vector<std::string> pArgV)
	{
		if (pArgV.size() <= 0)
			return;

		if (pArgV[0].empty())
			return;

		auto scene = Scene::GetCurrent();
		if (scene)
		{
			auto file = pArgV[0];
			if (FileSystem::ExtractExt(file) != "sav")
			{
				file = FileSystem::StripExt(file);
				file = file + ".sav";
				std::wstring f = WString(file);
				scene->Save(f);
			}
		}
	}
	static ConCommand save("save", CC_Save, CVAR_DEFAULT, "Usage: save <filepath>");

	void CC_Load(std::vector<std::string> pArgV)
	{
		if (pArgV.size() <= 0)
			return;

		if (pArgV[0].empty())
			return;

		auto scene = Scene::GetCurrent();
		if (scene)
		{
			auto file = pArgV[0];
			if (FileSystem::ExtractExt(file) != "sav")
			{
				file = FileSystem::StripExt(file);
				file = file + ".sav";
				std::wstring f = WString(file);
				scene->Load(f);
			}
		}
	}
	static ConCommand load("load", CC_Load, CVAR_DEFAULT, "Usage: load <filepath>");

	void CC_Fov(std::vector<std::string> pArgV)
	{
		if (pArgV.size() <= 0)
			return;

		if (pArgV[0].empty())
			return;

		Print("Setting fov to " + pArgV[0]);
		Settings::SetFov(String::Float(pArgV[0]));
	}
	static ConCommand fov("fov", CC_Fov, CVAR_DEFAULT, "Usage: fov <float>");

	Scene::Scene()
	{
		world = NULL;
		MainCamera = NULL;
		loadingbackground = NULL;
		mapdata = {};
	}

	Scene::~Scene()
	{
		mapdata.clear();
		if (loadingbackground)
		{
			loadingbackground->Release();
			loadingbackground = NULL;
		}

		if (world)
		{
			world->Release();
			world = NULL;
		}
	}

	void Scene::DrawLoadingScreen()
	{
		auto window = GraphicsWindow::GetCurrent();
		if (window)
		{
			auto context = window->GetFramebuffer();
			if (context)
			{
				//if (context->GetBlendMode() != Blend::Alpha) context->SetBlendMode(Blend::Alpha);
				if (loadingbackground)
				{
					context->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
					context->DrawImage(loadingbackground, 0, 0, context->GetWidth(), context->GetHeight());
				}
				else
				{
					context->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
					context->DrawRect(0, 0, context->GetWidth(), context->GetHeight());
				}
				//if (context->GetBlendMode() != Blend::Solid) context->SetBlendMode(Blend::Solid);
				if (!currentmap.empty()) window->Sync();
			}
		}
	}

	void Scene::Update()
	{
		if (!currentmap.empty())
		{
			// Pause
			Time::Pause();

			// Hide UI Gadgets
			Gadget::HideUI(true);

			// Draw loading curtain
			DrawLoadingScreen();

			if (LoadMapFile(currentmap))
			{
				// Assign the camera 
				if (MainCamera == NULL) MainCamera = world->cameras.front();
				Settings::Apply(MainCamera);

				mapdata.path = currentmap;
				mapdata.filetime = FileSystem::GetFileTime(currentmap);
				currentmap.clear();

				FireCallback(CALLBACK_MAPLOADCOMPLETE, this, NULL);
			}

			// Show UI Gadgets
			Gadget::HideUI(false);

			// Resume
			Time::Resume();
		}

		UpdateTime();
		if (!PauseState()) world->Update();		
		world->Render();

		// If there's no map loaded, draw a black rect over the screen.
		if (mapdata.path.empty())
		{
			DrawLoadingScreen();
		}
	}

	long Scene::GetTime()
	{
		return Time::GetCurrent();
	}

	float Scene::GetSpeed()
	{
		return Time::GetSpeed();
	}

	void Scene::SetPauseState(const bool pause)
	{
		if (pause != PauseState())
		{
			if (pause)
			{
				Time::Pause();
			}
			else
			{
				Time::Resume();
			}
		}

		FireCallback(CALLBACK_PAUSE, this, NULL);
	}

	bool Scene::GetPauseState()
	{
		return PauseState();
	}

	void Scene::Clear()
	{
		if (!mapdata.path.empty())
		{
			FireCallback(CALLBACK_MAPCLEAR, this, NULL);

			if (world->GetSkybox() != NULL)
			{
				world->skybox->Release();
				world->skybox = NULL;
				//world->SetSkybox(AssetLoader::DefaultSkybox());
			}

			world->Clear();
			mapdata.clear();
		}
	}

	void Scene::LoadMap(std::string& path)
	{
		auto test = path;
		if (FileSystem::GetFileType(test) == 0)
		{
			// If invaild, try to correct the the path.
			test = "Maps/" + test + ".map";

			if (FileSystem::GetFileType(test) == 0)
			{
				Print("Error: Failed to load " + QuoteString(path) + "...");
				return;
			}
		}

		currentmap = test;
		FireCallback(CALLBACK_MAPLOAD, this, NULL);
	}

	bool Scene::InMap()
	{
		return !mapdata.empty();
	}

	bool pointee_is_equal(const Leadwerks::Entity* s, const Leadwerks::Entity* p) {
		return s == p;
	}

	void Scene::WriteEntityID(Leadwerks::Entity* entity)
	{
		// Skip brushes, they aren't going anywhere!
		if (entity->GetClass() == Object::ModelClass)
		{
			auto mdl = dynamic_cast<Model*>(entity);
			if (mdl->collapsedfrombrushes) return;
		}
		else if (entity->GetClass() == Object::BrushClass)
		{
			return;
		}

		// ID is <EntityName>_<PositionInTree>_<(Bool)HasActor>
		if (entity)
		{
			std::string name = entity->GetKeyValue("name");
			std::size_t posinlist = entities.size();
			int hasactor = 0;

#if 0
			std::list<Entity*>::iterator matching_iter = std::find_if(
				world->entities.begin(), world->entities.end(),
				std::bind1st(pointee_is_equal, entity)
			);

			if (matching_iter != world->entities.end()) {
				pos = std::distance(world->entities.begin(), matching_iter);
			}

			

			//std::list<Entity*>::iterator del_new = std::find(world->entities.begin(), world->entities.end(), entity);
			//if (del_new != world->entities.end())
			//{
			//	pos = del_new.p
			//}
#endif
			hasactor = entity->GetActor() != NULL;

			// Write the id to a keyvalue
			std::string uuid = name + "_" + to_string(posinlist) + "_" + String(hasactor);
			entity->SetKeyValue("uuid", uuid);
			//Print(uuid);
			entities.push_back(entity);
		}
	}

	std::string Scene::GetEntityID(Leadwerks::Entity* entity)
	{
		return entity->GetKeyValue("uuid");
	}

	void Scene::Load(const std::wstring& path)
	{
		// TODO: 
		// See if the save is newer than the map.
		// If the map isn't currently loaded, load the map
		// Then the json and apply values to each entity.
		table j3 = Table::Load(path);
	}

	void Scene::Save(const std::wstring& path)
	{
		if (!InMap()) return;

		std::wstring file = path;
		nlohmann::json t;
;
		t["file"] = mapdata.path;

		// World
		t["world"] = nlohmann::json::object();
		t["world"]["size"] = world->GetSize();
		t["world"]["gravity"] = nlohmann::json::array();
		t["world"]["gravity"][0] = world->GetGravity().x;
		t["world"]["gravity"][1] = world->GetGravity().y;
		t["world"]["gravity"][2] = world->GetGravity().z;

		t["world"]["ambientlight"] = nlohmann::json::array();
		t["world"]["ambientlight"][0] = world->GetAmbientLight().r;
		t["world"]["ambientlight"][1] = world->GetAmbientLight().g;
		t["world"]["ambientlight"][2] = world->GetAmbientLight().b;
		t["world"]["ambientlight"][3] = world->GetAmbientLight().a;

		t["world"]["fogmode"] = true;
		t["world"]["fogcolor"] = nlohmann::json::array();
		t["world"]["fogcolor"][0] = world->mapfogcolor.r;
		t["world"]["fogcolor"][1] = world->mapfogcolor.g;
		t["world"]["fogcolor"][2] = world->mapfogcolor.b;
		t["world"]["fogcolor"][3] = world->mapfogcolor.a;

		t["world"]["fogrange"] = nlohmann::json::array();
		t["world"]["fogrange"][0] = world->mapfogrange.x;
		t["world"]["fogrange"][1] = world->mapfogrange.y;

		t["world"]["fogangle"] = nlohmann::json::array();
		t["world"]["fogangle"][0] = world->mapfogangle.x;
		t["world"]["fogangle"][1] = world->mapfogangle.y;

		t["world"]["watermode"] = world->GetWaterMode();
		t["world"]["waterheight"] = world->GetWaterHeight();
		t["world"]["watercolor"] = nlohmann::json::array();
		t["world"]["watercolor"][0] = world->GetWaterColor().r;
		t["world"]["watercolor"][1] = world->GetWaterColor().g;
		t["world"]["watercolor"][2] = world->GetWaterColor().b;
		t["world"]["watercolor"][3] = world->GetWaterColor().a;

		// Entities
		t["entites"] = nlohmann::json::object();
		for (const auto& p : entities)
		{
			auto uuid = String::Split(GetEntityID(p), "_");
			if (uuid.empty()) break;

			auto name = uuid[0];
			auto pos = uuid[1];
			auto hasactor = uuid[2];

			t["entites"][name] = nlohmann::json::object();
			t["entites"][name]["id"] = pos;
			t["entites"][name]["hasactor"] = hasactor;

			// General
			Vec3 position = p->GetPosition();
			t["entites"][name]["position"] = nlohmann::json::array();
			t["entites"][name]["position"][0] = position.x;
			t["entites"][name]["position"][1] = position.y;
			t["entites"][name]["position"][2] = position.z;

			Vec3 rotation = p->GetRotation();
			t["entites"][name]["rotation"] = nlohmann::json::array();
			t["entites"][name]["rotation"][0] = rotation.x;
			t["entites"][name]["rotation"][1] = rotation.y;
			t["entites"][name]["rotation"][2] = rotation.z;

			Vec3 scale = p->GetRotation();
			t["entites"][name]["scale"] = nlohmann::json::array();
			t["entites"][name]["scale"][0] = scale.x;
			t["entites"][name]["scale"][1] = scale.y;
			t["entites"][name]["scale"][2] = scale.z;

			// Apperence
			Vec4 color = p->GetColor(COLOR_DIFFUSE);
			t["entites"][name]["color"] = nlohmann::json::array();
			t["entites"][name]["color"][0] = color.r;
			t["entites"][name]["color"][1] = color.g;
			t["entites"][name]["color"][2] = color.b;
			t["entites"][name]["color"][3] = color.a;
			t["entites"][name]["intensity"] = p->GetIntensity(COLOR_DIFFUSE);

			Vec4 colorspec = p->GetColor(COLOR_SPECULAR);
			t["entites"][name]["colorspec"] = nlohmann::json::array();
			t["entites"][name]["colorspec"][0] = colorspec.r;
			t["entites"][name]["colorspec"][1] = colorspec.g;
			t["entites"][name]["colorspec"][2] = colorspec.b;
			t["entites"][name]["colorspec"][3] = colorspec.a;
			t["entites"][name]["intensityspec"] = p->GetIntensity(COLOR_SPECULAR);

			Vec4 coloredit = p->GetColor(COLOR_EDIT);
			t["entites"][name]["coloredit"] = nlohmann::json::array();
			t["entites"][name]["coloredit"][0] = coloredit.r;
			t["entites"][name]["coloredit"][1] = coloredit.g;
			t["entites"][name]["coloredit"][2] = coloredit.b;
			t["entites"][name]["coloredit"][3] = coloredit.a;

			t["entites"][name]["viewrange"] = p->GetViewRange();
			t["entites"][name]["shaodows"] = p->GetShadowMode();
			t["entites"][name]["occlusionculling"] = p->GetOcclusionCullingMode();

			// Physics
			t["entites"][name]["physicsmode"] = p->GetPhysicsMode();
			t["entites"][name]["collisiontype"] = p->GetCollisionType();
			t["entites"][name]["mass"] = p->GetMass();
			t["entites"][name]["characterangle"] = p->GetCharacterControllerAngle();
			t["entites"][name]["sweptcollision"] = p->GetSweptCollisionMode();
			t["entites"][name]["navmode"] = p->GetNavigationMode();
			// TODO: get friction and such...

			// Actors
			auto actor = p->GetActor();
			if (actor != NULL)
			{
				auto sceneactor = dynamic_cast<SceneActor*>(actor);
				if (sceneactor)
				{
					t["entites"][name]["actor"] = nlohmann::json::object();
					sceneactor->Save(t["entites"][name]["actor"]);
				}
			}
		}

		JSON::Save(t, file);
	}

	Leadwerks::World* Scene::GetWorld()
	{
		return world;
	}

	Scene* Scene::currentscene = NULL;
	Scene* Scene::GetCurrent()
	{
		return currentscene;
	}

	Scene* Scene::Create()
	{
		if (currentscene)
		{
			currentscene->Release();
			currentscene = NULL;
		}

		currentscene = new Scene();
		currentscene->world = World::Create();
		currentscene->loadingbackground = AssetLoader::DefaultTexture();
		return currentscene;
	}
}