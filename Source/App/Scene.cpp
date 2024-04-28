#include "pch.h"
#include "App.h"
#include "SceneActor.h"
#include "../ActorSystem.h"

extern ConVar phys_steps;

namespace App
{
	using namespace Leadwerks;

	std::string Scene::nextmaptoload = "";
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

			scene->DrawLoadingScreen(true);
		}

		//AssetLoader::HighlightMissingMaterial(entity);
	}

	static bool LoadMapFile(const std::string& path, Object* extra = NULL)
	{
		bool result = Map::Load(path, MapHook, extra);
		if (!result)
		{
			Print("Error: Failed to load " + QuoteString(path) + "...");
			Scene::nextmaptoload.clear();
			FireCallback(CALLBACK_SCENEFAILED, extra, NULL);

		}
		return result;
	}

	Scene::Scene()
	{
		world = NULL;
		MainCamera = NULL;
		loadingbackground = NULL;
		delayload = NULL;
		mapdata = {};
		cleartoload = false;
		isloading = true;
		timepausestate = false;
	}

	Scene::~Scene()
	{
		mapdata.clear();
		cleartoload = false;
		isloading = false;
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

		if (delayload)
		{
			delayload->Release();
			delayload = NULL;
		}
	}

	void Scene::DrawLoadingScreen(const bool sync)
	{
		auto window = GraphicsWindow::GetCurrent();
		if (window)
		{
			auto context = window->GetFramebuffer();
			if (context)
			{
				//context->Clear();

				//if (context->GetBlendMode() != Blend::Alpha) context->SetBlendMode(Blend::Alpha);
				if (loadingbackground)
				{
					context->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
					context->DrawImage(loadingbackground, 0, 0, context->GetWidth(), context->GetHeight());
				}
				else
				{
					// If there's no texture, draw a black rect.
					context->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
					context->DrawRect(0, 0, context->GetWidth(), context->GetHeight());
				}

				// Draw anything else on top.
				FireCallback(CALLBACK_LOADINGSCREEN, this, NULL);

				//if (context->GetBlendMode() != Blend::Solid) context->SetBlendMode(Blend::Solid);
				if (!Scene::nextmaptoload.empty() && sync) window->Sync();
			}
		}
	}

	void Scene::Update()
	{
		if (!Scene::nextmaptoload.empty() && cleartoload)
		{
			// Pause
			Time::Pause();

			// Clear the scene.
			Clear();

			// Draw loading curtain
			isloading = true;
			DrawLoadingScreen(true);

			if (LoadMapFile(Scene::nextmaptoload))
			{
				// Assign the camera 
				//if (MainCamera == NULL)
				{
					int cameracount = 0;
					for (const auto& camera : world->cameras)
					{
						if (camera->GetClass() == Object::CameraClass && camera->GetRenderTarget() == NULL)
						{
							if (cameracount == 0)
							{
								MainCamera = camera;
								Settings::Apply(MainCamera);
							}
							cameracount++;
							if (cameracount > 1) Print("Warning: More than one camera exist in the scene!");
						}
					}
				}
				
				mapdata.path = Scene::nextmaptoload;
				mapdata.filetime = FileSystem::GetFileTime(Scene::nextmaptoload);
				Scene::nextmaptoload.clear();

				PostLoadModifications();

				// Show UI Gadgets
				Gadget::HideUI(false);

				isloading = false;
				FireCallback(CALLBACK_SCENECOMPLETE, this, NULL);
			}

			// Resume
			Time::Resume();
		}

		UpdateTime();
		auto steps = phys_steps.GetInt();
		if (steps <= 0) steps = 1;
		if (!PauseState()) world->Update(steps);
		world->Render();

		// If there's no map loaded, draw a black rect over the screen.
		if (mapdata.path.empty())
		{
			// Show UI Gadgets
			Gadget::HideUI(false);

			isloading = false;
			DrawLoadingScreen(false);
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
			if (world->GetSkybox() != NULL)
			{
				world->skybox->Release();
				world->skybox = NULL;
				//world->SetSkybox(AssetLoader::DefaultSkybox());
			}

			world->Clear(true);
			mapdata.clear();
			if (MainCamera) MainCamera = NULL;

			FireCallback(CALLBACK_SCENECLEAR, this, NULL);
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

		// Hide UI Gadgets
		Gadget::HideUI(true);

		Scene::nextmaptoload = test;
		FireCallback(CALLBACK_SCENELOAD, this, NULL);
	}

	bool Scene::InMap()
	{
		return !mapdata.empty();
	}

	bool Scene::IsLoading()
	{
		//if (!cleartoload) return true;
		return isloading;
	}

	void Scene::PostLoadModifications()
	{
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
			if (entity->GetScript().empty()) return;
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
		// See if the save is newer than the map.
		auto pathastring = WString::ToString(path);
		if (mapdata.filetime > FileSystem::GetFileTime(pathastring))
		{
			Print("Error: Map file is newer than saved game.");
			return;
		}

		// TODO
#if 0
		// If the map isn't currently loaded, load the map
		auto t = JSON::Load(path);
		std::string s = t["file"].get<string>();
		if (mapdata.path != s)
		{
			LoadMap(s);

			// Try it again.
			if (mapdata.path != s)
			{
				Print("Error: Failed to load save as map doesn't exist.");
				return;
			}
		}

		// TODO: 
		// Then the json and apply values to each entity.
		world->SetSize(t["world"]);
		if (t["world"]["gravity"].is_array())
		{
			world->SetGravity(t["world"]["gravity"][0], t["world"]["gravity"][1], t["world"]["gravity"][2]);
		}

		if (t["world"]["ambientlight"].is_array())
		{
			world->SetAmbientLight(t["world"]["ambientlight"][0], t["world"]["ambientlight"][1], t["world"]["ambientlight"][2], t["world"]["ambientlight"][3]);
		}

		world->mapfogmode = t["world"]["fogmode"];
		if (t["world"]["fogcolor"].is_array())
		{
			world->mapfogcolor = Vec4(t["world"]["fogcolor"][0], t["world"]["fogcolor"][1], t["world"]["fogcolor"][2], t["world"]["fogcolor"][3]);
		}

		if (t["world"]["fogrange"].is_array())
		{
			world->mapfogrange = Vec2(t["world"]["fogrange"][0], t["world"]["fogrange"][1]);
		}

		if (t["world"]["fogangle"].is_array())
		{
			world->mapfogrange = Vec2(t["world"]["fogangle"][0], t["world"]["fogangle"][1]);
		}

		world->SetWaterMode(t["world"]["watermode"]);
		world->SetWaterHeight(t["world"]["waterheight"]);
		if (t["world"]["watercolor"].is_array())
		{
			world->SetWaterColor(t["world"]["watercolor"][0], t["world"]["watercolor"][1], t["world"]["watercolor"][2], t["world"]["watercolor"][3]);
		}

		if (t["entites"].is_object())
		{
			for (const auto& p : entities)
			{
				auto uuid = String::Split(GetEntityID(p), "_");
				if (uuid.empty()) break;
				auto name = uuid[0];
				auto pos = uuid[1];
				auto hasactor = uuid[2];

				if (t["entites"][name] == name && t["entites"][name]["id"] == pos && t["entites"][name]["hasactor"] == hasactor)
				{
					auto entity = p;
					if (t["entites"][name]["position"].is_array())
					{
						entity->SetPosition(t["entites"][name]["position"][0], t["entites"][name]["position"][1], t["entites"][name]["position"][2]);
					}

					if (t["entites"][name]["rotation"].is_array())
					{
						entity->SetPosition(t["entites"][name]["rotation"][0], t["entites"][name]["rotation"][1], t["entites"][name]["rotation"][2]);
					}

					if (t["entites"][name]["scale"].is_array())
					{
						entity->SetPosition(t["entites"][name]["scale"][0], t["entites"][name]["scale"][1], t["entites"][name]["scale"][2]);
					}

					if (t["entites"][name]["color"].is_array())
					{
						entity->SetColor(t["entites"][name]["color"][0], t["entites"][name]["color"][1], t["entites"][name]["color"][2], t["entites"][name]["color"][1], t["entites"][name]["color"][3]);
					}
					entity->SetIntensity(t["entites"][name]["intensity"]);

					if (t["entites"][name]["colorspec"].is_array())
					{
						entity->SetColor(t["entites"][name]["colorspec"][0], t["entites"][name]["colorspec"][1], t["entites"][name]["colorspec"][2], t["entites"][name]["colorspec"][1], t["entites"][name]["colorspec"][3], COLOR_SPECULAR);
					}
					entity->SetIntensity(t["entites"][name]["intensityspec"], COLOR_DIFFUSE);

					if (t["entites"][name]["coloredit"].is_array())
					{
						entity->SetColor(t["entites"][name]["coloredit"][0], t["entites"][name]["coloredit"][1], t["entites"][name]["coloredit"][2], t["entites"][name]["coloredit"][1], t["entites"][name]["coloredit"][3]);
					}
					
					entity->SetViewRange(t["entites"][name]["viewrange"]);
					entity->SetShadowMode(t["entites"][name]["shadows"]);
					entity->SetOcclusionCullingMode(t["entites"][name]["occlusionculling"]);

					// Physics
					entity->SetPhysicsMode(t["entites"][name]["physicsmode"]);
					entity->SetCollisionType(t["entites"][name]["collisiontype"]);
					entity->SetMass(t["entites"][name]["mass"]);
					entity->SetCharacterControllerAngle(t["entites"][name]["characterangle"]);
					entity->SetSweptCollisionMode(t["entites"][name]["sweptcollision"]);
					entity->SetNavigationMode(t["entites"][name]["navmode"]);

					// TODO: Apply friction and such. We can't do this now as there's no refs for this in the entity's header...

					// Actor
					if (t["entites"][name]["actor"].is_object())
					{
						auto actor = entity->GetActor();
						if (actor == NULL)
						{
							Print("Error: Failed to load actor from save file..");
						}
						else
						{
							if (t["entites"][name]["actor"]["data"].is_object())
							{
								auto sceneactor = dynamic_cast<SceneActor*>(actor);
								if (sceneactor)
								{
									sceneactor->Load(t["entites"][name]["actor"]["data"]);
								}		
							}
						}
					}

				}
			}
		}
#endif
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

		t["world"]["fogmode"] = world->mapfogmode;
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

			Vec3 scale = p->GetScale();
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
			t["entites"][name]["shadows"] = p->GetShadowMode();
			t["entites"][name]["occlusionculling"] = p->GetOcclusionCullingMode();

			// Physics
			t["entites"][name]["physicsmode"] = p->GetPhysicsMode();
			t["entites"][name]["collisiontype"] = p->GetCollisionType();
			t["entites"][name]["mass"] = p->GetMass();
			t["entites"][name]["characterangle"] = p->GetCharacterControllerAngle();
			t["entites"][name]["sweptcollision"] = p->GetSweptCollisionMode();
			t["entites"][name]["navmode"] = p->GetNavigationMode();
			// TODO: get friction and such. We can't do this now as there's no refs for this in the entity's header...

			// Actors
			auto actor = p->GetActor();
			if (actor != NULL)
			{
				auto sceneactor = dynamic_cast<SceneActor*>(actor);
				if (sceneactor)
				{
					t["entites"][name]["actor"] = nlohmann::json::object();
					t["entites"][name]["actor"]["class"] = p->GetString("actor");

					// FIXME!
					t["entites"][name]["actor"]["data"] = nlohmann::json::object();
					sceneactor->Save(t["entites"][name]["actor"]["data"]);
				}
			}
		}

		JSON::Save(t, file);
		Print("Game saved...");
	}

	Leadwerks::World* Scene::GetWorld()
	{
		return world;
	}

	void Scene::SetLoadingBackground(Leadwerks::Texture* texture)
	{
		// If it's the same pointer, don't bother.
		if (loadingbackground = texture) return;

		FreeObject(loadingbackground);
		loadingbackground = texture;
	}

	Leadwerks::Texture* Scene::GetLoadingBackground()
	{
		return loadingbackground;
	}

	Scene* Scene::currentscene = NULL;
	Scene* Scene::GetCurrent()
	{
		return currentscene;
	}

	bool Scene::EventCallback(const Event& e, Leadwerks::Object* extra)
	{
		auto elem = CastObject<Scene>(extra);
		if (elem == NULL) return false;
		return elem->ProcessEvent(e);
	}

	bool Scene::ProcessEvent(const Leadwerks::Event& e)
	{
		if (e.id == EVENT_STARTRENDERER)
		{
			delayload = Timer::Create(2500);		
		}
		else if (e.id == Event::TimerTick && e.source == delayload)
		{
			delayload->Release();
			delayload = NULL;
			cleartoload = true;
			FireCallback(CALLBACK_SCENEREADY, this, NULL);
		}
		return true;
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
		//currentscene->loadingbackground = AssetLoader::DefaultTexture();
		ListenEvent(EVENT_STARTRENDERER, NULL, Scene::EventCallback, currentscene);
		ListenEvent(Event::TimerTick, currentscene->delayload, SceneActor::EventCallback, currentscene);
		return currentscene;
	}
}