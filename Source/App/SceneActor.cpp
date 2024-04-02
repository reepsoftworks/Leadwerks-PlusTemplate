#include "pch.h"
#include "App.h"

namespace App
{
	using namespace Leadwerks;

	SceneActor::SceneActor()
	{
		Actor::Actor();
		extra = NULL;
	}

	SceneActor::~SceneActor()
	{
		extra = NULL;
		Actor::~Actor();
	}

	void SceneActorCallback(Object* source, Object* extra)
	{
		auto actor = CastObject<SceneActor>(source);
		if (actor) actor->Start();
	}

	void SceneActor::Attach()
	{
		SetCallback(CALLBACK_MAPLOADCOMPLETE, this, SceneActorCallback);
	}

	void SceneActor::Detach()
	{
	}

	void SceneActor::Start()
	{
	}

	void SceneActor::SetBoolValue(const std::string& value, const bool defaultvalue)
	{
		GetEntity()->SetBool(value, defaultvalue);
	}

	bool SceneActor::GetBoolValue(const std::string& value, const bool defaultvalue)
	{
		if (GetEntity() == NULL)
			return defaultvalue;

		if (GetEntity()->GetBool(value) != defaultvalue)
		{
			return GetEntity()->GetBool(value);
		}

		return defaultvalue;
	}

	void SceneActor::SetFloatValue(const std::string& value, const float defaultvalue)
	{
		GetEntity()->SetFloat(value, defaultvalue);
	}

	float SceneActor::GetFloatValue(const std::string& value, const float defaultvalue)
	{
		if (GetEntity() == NULL)
			return defaultvalue;

		if (GetEntity()->GetFloat(value) != NULL)
		{
			return GetEntity()->GetFloat(value);
		}

		return defaultvalue;
	}

	void SceneActor::SetIntValue(const std::string& value, const int defaultvalue)
	{
		GetEntity()->SetString(value, String(defaultvalue));
	}

	int SceneActor::GetIntValue(const std::string& value, const int defaultvalue)
	{
		if (GetEntity() == NULL)
			return defaultvalue;

		if (entity->GetFloat(value) != NULL)
		{
			return static_cast<int>(entity->GetFloat(value));
		}

		return false;
	}

	void SceneActor::SetStringValue(const std::string& value, const std::string& defaultvalue)
	{
		GetEntity()->SetString(value, defaultvalue);
	}

	std::string SceneActor::GetStringValue(const std::string& value, const std::string& defaultvalue)
	{
		if (GetEntity() == NULL)
			return defaultvalue;

		if (GetEntity()->GetString(value) != "")
		{
			return GetEntity()->GetString(value);
		}

		return defaultvalue;
	}

	Leadwerks::Vec2* SceneActor::GetVec2Value(const std::string& value, Leadwerks::Vec2* defaultvalue)
	{
		if (GetEntity() == NULL)
			return defaultvalue;

		Vec2* test = dynamic_cast<Vec2*>(entity->GetObject(value));
		if (test != NULL)
		{
			return test;
		}

		return defaultvalue;
	}

	Leadwerks::Vec3* SceneActor::GetVec3Value(const std::string& value, Leadwerks::Vec3* defaultvalue)
	{
		if (GetEntity() == NULL)
			return defaultvalue;

		Vec3* test = dynamic_cast<Vec3*>(entity->GetObject(value));
		if (test != NULL)
		{
			return test;
		}

		return defaultvalue;
	}

	Leadwerks::Vec4* SceneActor::GetVec4Value(const std::string& value, Leadwerks::Vec4* defaultvalue)
	{
		if (GetEntity() == NULL)
			return defaultvalue;

		Vec4* test = dynamic_cast<Vec4*>(entity->GetObject(value));
		if (test != NULL)
		{
			return test;
		}

		return defaultvalue;
	}

	Leadwerks::Entity* SceneActor::GetEntityValue(const std::string& value, Leadwerks::Entity* defaultvalue)
	{
		if (GetEntity() == NULL)
			return defaultvalue;

		Entity* test = dynamic_cast<Entity*>(entity->GetObject(value));
		if (test != NULL)
		{
			return test;
		}

		return defaultvalue;
	}

	Leadwerks::Entity* SceneActor::GetEntity()
	{
		return this->entity;
	}

	void SceneActor::FireOutput(const std::string& eventname)
	{
		if (GetEntity() == NULL)
			return;

		if (entity->component != NULL)
		{
			entity->component->CallOutputs(eventname);
		}
	}

	bool SceneActor::EventCallback(const Event& e, Leadwerks::Object* extra)
	{
		auto elem = CastObject<SceneActor>(extra);
		if (elem == NULL) return false;
		return elem->ProcessEvent(e);
	}

	void SceneActor::Listen(const int eventid, Leadwerks::Object* source)
	{
		ListenEvent(eventid, source, SceneActor::EventCallback, this);
	}
}