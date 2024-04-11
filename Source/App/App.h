#pragma once
#include "pch.h"

namespace App
{
	// App Callback enum
	enum
	{
		// Post Render callback for drawing Gadgets.
		CALLBACK_POSTRENDER = 10,

		// Callback used when the pause state is changed.
		CALLBACK_PAUSE,

		// Fired durning the map loading hook. USE THIS FOR DRAWING!
		CALLBACK_LOADINGSCREEN,

		// Fired when the scene is ready to start taking map load requests.
		// This is always fired called when the delay timer completes regardless
		// if there's a map pending to be loaded. Check Scene::nextmaptoload 
		// Check if there's anything pending.
		CALLBACK_SCENEREADY,

		// Called right after we get told to load something.
		// Check Scene::nextmaptoload to see what's being loaded!
		CALLBACK_SCENELOAD,

		// Callback for when the scene has successfully loaded.
		// This is used for Scene Actors for calling their Start() function.
		CALLBACK_SCENECOMPLETE,

		// Callback for when the map has NOT successfully loaded.
		CALLBACK_SCENEFAILED,

		// Used for then the map has been cleared.
		CALLBACK_SCENECLEAR
	};

	// 
	enum
	{
		EVENT_LANGUAGE = 11000,

		// Input
		EVENT_DEVICE,
		EVENT_ACTIONSET,
		EVENT_DEVICESTATE
	};

	class Program//lua
	{
	public:
		static std::map<std::string, std::string> Arguments;
		static uint64_t Program::AppID;
		static std::string Program::Author;
		static std::string Program::Copyright;
		static std::string GetTitle();
		static void ParseArguments(int argc, const char* argv[]);
		static bool CheckArgument(const std::string& argument);//lua
		static std::string CheckArgumentString(const std::string& argument, const std::string& defaultvalue);//lua
		static int CheckArgumentValue(const std::string& argument, const int defaultvalue);//lua
		static const int NormalMode;//lua
		static const int EditorMode;//lua
		static const int DebugMode;//lua
		static const int GetAppMode();//lua
		static bool VendorCheck(const bool skipcheck = false);
		static bool LoadWerkFile();


		// Console Execution
		static bool ExecuteTable(table input);
		static bool ExecuteMap(std::map<std::string, std::string> input);
		static bool LoadConVars(const std::string& path);
		static bool SaveConVars(const std::string& path);
	};
}

#include "../version.h"
#include "GraphicsWindow.h"
#include "GameMenu.h"
#include "Gadget.h"
#include "Scene.h"
#include "SceneActor.h"
#include "Translator.h"
//#include "Input.h"

// Gadgets
#include "Gadgets/StatsGadget.h"
#include "Gadgets/ConsoleGadget.h"
#include "Gadgets/SettingsGadget.h"
#include "Gadgets/QuitConfirmGadget.h"
#include "Gadgets/MapListGadget.h"

// UI
#include "UI/UIGadget.h"