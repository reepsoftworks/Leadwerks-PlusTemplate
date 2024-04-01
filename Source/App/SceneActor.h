#pragma once
#include "App.h"

namespace App
{
	class Scene;
	class SceneActor : public Leadwerks::Actor
	{
		static bool EventCallback(const Leadwerks::Event& e, Leadwerks::Object* extra);
	protected:
		Object* extra;

		void SetBoolValue(const std::string& value, const bool defaultvalue = false);
		bool GetBoolValue(const std::string& value, const bool defaultvalue = false);
		void SetFloatValue(const std::string& value, const float defaultvalue = 0);
		float GetFloatValue(const std::string& value, const float defaultvalue = 0);
		void SetIntValue(const std::string& value, const int defaultvalue = 0);
		int GetIntValue(const std::string& value, const int defaultvalue = 0);
		void SetStringValue(const std::string& value, const std::string& defaultvalue = "");
		std::string GetStringValue(const std::string& value, const std::string& defaultvalue = "");

		Leadwerks::Vec2* GetVec2Value(const std::string& value, Leadwerks::Vec2* defaultvalue = 0);
		Leadwerks::Vec3* GetVec3Value(const std::string& value, Leadwerks::Vec3* defaultvalue = 0);
		Leadwerks::Vec4* GetVec4Value(const std::string& value, Leadwerks::Vec4* defaultvalue = 0);
		Leadwerks::Entity* GetEntityValue(const std::string& value, Leadwerks::Entity* defaultvalue = NULL);

		virtual bool ProcessEvent(const Leadwerks::Event& e) { return true; };
		virtual void Listen(const int eventid, Leadwerks::Object* source);

	public:
		SceneActor();
		virtual ~SceneActor();

		virtual void Attach();
		virtual void Detach();
		virtual void Start();

		Leadwerks::Entity* GetEntity();
		virtual void FireOutput(const std::string& eventname);
		virtual bool Use(Leadwerks::Entity* source, Leadwerks::Object* extra) { return false; };

		virtual void Load(nlohmann::json properties) {};
		virtual void Save(nlohmann::json properties) {};

		friend class Scene;
	};
}