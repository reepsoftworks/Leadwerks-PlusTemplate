#include "pch.h"
#include "../App.h"

using namespace Leadwerks;
using namespace App;

void CC_Map(std::vector<std::string> pArgV)
{
	if (pArgV.size() <= 0)
		return;

	if (pArgV[0].empty())
		return;

	auto scene = Scene::GetCurrent();
	if (scene)
	{
		scene->LoadMap(pArgV[0]);
	}
	else
	{
		auto test = pArgV[0];
		if (FileSystem::GetFileType(test) == 0)
		{
			// If invaild, try to correct the the path.
			test = "Maps/" + test + ".map";

			if (FileSystem::GetFileType(test) == 0)
			{
				Print("Error: Failed to load " + QuoteString(pArgV[0]) + "...");
				return;
			}
		}

		Scene::nextmaptoload = test;
	}
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