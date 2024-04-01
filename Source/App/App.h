#pragma once
#include "pch.h"

namespace App
{
	enum
	{
		CALLBACK_POSTRENDER = 10,
		CALLBACK_ASSETLOADED,
		CALLBACK_PAUSE,
		CALLBACK_MAPLOAD,
		CALLBACK_MAPLOADCOMPLETE,
		CALLBACK_MAPLOADFAILED,
		CALLBACK_MAPCLEAR
	};

	class Program//lua
	{
	public:
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
	};


	/*
	extern std::map<std::string, std::string> Arguments;
	extern void ParseArguments(int argc, const char* argv[]);
	extern bool CheckArgument(const std::string& argument);
	extern std::string CheckArgumentString(const std::string& argument, const std::string& defaultvalue);
	extern int CheckArgumentValue(const std::string& argument, const int defaultvalue);

	enum AppMode
	{
		MODE_NORMAL = 0,
		MODE_EDITOR,
		MODE_DEBUG
	};

	extern AppMode GetAppMode();
	extern bool VendorCheck(const bool skipcheck = false);
	extern bool LoadWerkFile();
	*/
}

#include "../version.h"
#include "GraphicsWindow.h"
#include "AssetLoader.h"
#include "GameMenu.h"
#include "Gadget.h"
#include "Scene.h"
#include "SceneActor.h"


// Gadgets
#include "Gadgets/StatsGadget.h"
#include "Gadgets/ConsoleGadget.h"
#include "Gadgets/TextureGadget.h"