#include "pch.h"
#include "SampleUI.h"

using namespace Leadwerks;
using namespace App;

SampleUI* SampleUI::currentui = NULL;

// UI Commands
void CC_OpenMapList(std::vector<std::string> pArgV)
{
	if (SampleUI::currentui)
	{
		SampleUI::currentui->CallMapList();
	}
}
static ConCommand maplistdialog("maplistdialog", CC_OpenMapList, CVAR_DEFAULT, "");

void CC_OpenSettings(std::vector<std::string> pArgV)
{
	if (SampleUI::currentui)
	{
		SampleUI::currentui->ToggleSettingsDialog(true);
	}
}
static ConCommand settingsdialog("settingsdialog", CC_OpenSettings, CVAR_DEFAULT, "Opens settings panel. Usage: settingsdialog");

void CC_OpenQuitConfim(std::vector<std::string> pArgV)
{
	if (SampleUI::currentui)
	{
		SampleUI::currentui->CallQuit();
	}
}
static ConCommand quitconfirm("quitconfirm", CC_OpenQuitConfim, CVAR_HIDDEN, "");

void CC_StatsMode(std::string pArgV)
{
	if (pArgV.empty())
		return;

	auto ui = SampleUI::currentui;
	if (ui)
	{
		auto value = String::Int(pArgV);
		ui->ShowStats(value);
	}
}
static ConVar stats("stats", "0", CVAR_SAVE, "Shows rendering and time statistics. Usage: stats <int>", CC_StatsMode);

// SampleUI
SampleUI::SampleUI()
{
	maplistgadget = NULL;
	settinggadget = NULL;
	quitconfirmgadget = NULL;
	statsgadget = NULL;
	console = NULL;
	loadingbackground = NULL;
	loadingicon = NULL;
	loadingspinner.fill(NULL); 
	loadtick = 0;
	loading = false;

	menushown = false;
	margin = 0;
	title = NULL;
	fonttitle = NULL;
	fontmenu = NULL;
	resourcefile = NULL;
	widgetsopened = 0;
	ingame = false;

	buttons.clear();
}

SampleUI::~SampleUI()
{
	FreeObject(console);
	FreeObject(statsgadget);
	FreeObject(maplistgadget);
	FreeObject(settinggadget);
	FreeObject(quitconfirmgadget);
	
	FreeObject(loadingbackground);
	FreeObject(loadingicon);

	FreeObject(fonttitle);
	FreeObject(fontmenu);

	for (const auto& btn : buttons)
	{
		FreeObject(btn);
	}

	loadtick = 0;
	loading = false;
	menushown = false;
	margin = 0;
	resourcefile = NULL;
	buttons.clear();
	widgetsopened = 0;
	ingame = false;

	RemoveCallback(this);
}

void SampleUI::Start()
{
	// Create our gadgets.
	maplistgadget = CreateGadget<MapListGadget>(window);
	settinggadget = CreateGadget<SettingsGadget>(window);
	quitconfirmgadget = CreateGadget<QuitConfirmGadget>(window);
	statsgadget = CreateGadget<StatsGadget>(window);
	console = CreateGadget<ConsoleGadget>(window);
	
	if (!console || !maplistgadget || !settinggadget  || !quitconfirmgadget || !statsgadget)
	{
		OS::MessageError("Error", "Failed to create one ore more gadgets!");
		Debug::Stop();
	}

	// Apply visiblity
	console->Hide();
	maplistgadget->Hide();
	settinggadget->Hide();
	quitconfirmgadget->Hide();
	ShowStats(0);

	// Load resources
	auto j3 = JSON::Load("UI/SampleUI/resource.json");
	if (j3 != NULL)
	{
		if (j3["resources"].is_object())
		{
			// Fonts
			if (j3["resources"]["fonts"].is_object())
			{
				// Title Font
				if (j3["resources"]["fonts"]["title"].is_object())
				{
					std::string file = "UI/Fonts/arial.ttf";
					int size = 10;
					if (j3["resources"]["fonts"]["title"]["file"].is_string())
					{
						file = j3["resources"]["fonts"]["title"]["file"].get<std::string>();
						
					}

					if (j3["resources"]["fonts"]["title"]["size"].is_number_unsigned())
					{
						size = j3["resources"]["fonts"]["title"]["size"].get<int>();

					}
					fonttitle = AssetLoader::LoadFont(file, size);
				}

				// Menu Font
				if (j3["resources"]["fonts"]["menu"].is_object())
				{
					std::string file = "UI/Fonts/arial.ttf";
					int size = 10;
					if (j3["resources"]["fonts"]["menu"]["file"].is_string())
					{
						file = j3["resources"]["fonts"]["menu"]["file"].get<std::string>();

					}

					if (j3["resources"]["fonts"]["menu"]["size"].is_number_unsigned())
					{
						size = j3["resources"]["fonts"]["menu"]["size"].get<int>();

					}
					fontmenu = AssetLoader::LoadFont(file, size);
				}
			}

			// Images
			if (j3["resources"]["loadingscreen"].is_string()) loadingbackground = AssetLoader::LoadTexture(j3["resources"]["loadingscreen"].get<std::string>());
			if (j3["resources"]["loadingicon"].is_string()) loadingicon = AssetLoader::LoadTexture(j3["resources"]["loadingicon"].get<std::string>());
			if (j3["resources"]["loadingspinner"].is_string())loadingspinner[0] = AssetLoader::LoadTexture(j3["resources"]["loadingspinner"].get<std::string>());

			resourcefile = j3;
		}
	}

	// Start Translator
	std::string str = "UI/gameui_";
	Directory* dir = FileSystem::LoadDir("UI");
	if (dir)
	{
		for (const auto& p : dir->files)
		{
			// Skip folders.
			auto name = FileSystem::StripAll(p);
			if (FileSystem::GetFileType(name) == 2) continue;

			// Skip files with no extention or files not json.
			auto ext = FileSystem::ExtractExt(p);
			if (ext.empty() || ext != "json") continue;

			// Prefix check.
			if (String::Left(name, 7) == "gameui_")
			{
				Translator::LoadTokenList("UI/" + name + "." + ext);
			}
		}
	}

	// Build Loading screen.
	BuildLoadingScreen();

	// Build Menu Panel
	BuildMenuPanel();
}

void SampleUI::ToggleSettingsDialog(const bool show)
{
	if (settinggadget)
	{
		if (show)
			settinggadget->Show();
		else
			settinggadget->Hide();
	}
}

void SampleUI::CallMapList()
{
	if (maplistgadget) maplistgadget->Show();
}

void SampleUI::CallQuit()
{
	if (quitconfirmgadget) quitconfirmgadget->Show();
}

void SampleUI::ShowStats(const int mode)
{
	if (statsgadget)
	{
		statsgadget->SetMode(mode);
	}
}

void SampleUI::ToggleConsole()
{
	bool h = console->GetHidden();
	if (!h)
		console->Hide();
	else
		console->Show();
}

void SampleUI::ForceHideConsole()
{
	console->Hide();
}

// Loading Screen 
void SampleUI::BuildLoadingScreen()
{
	// If the scene exists, set the default loading screen
	auto scene = Scene::GetCurrent();
	if (scene)
	{
		// If it's null/the default texture, fill all the frames with it!
		if (loadingspinner[0] == NULL)
		{
			loadingspinner[0] = AssetLoader::DefaultTexture();
			loadingspinner.fill(loadingspinner[0]);
		}
		else if (loadingspinner[0] == AssetLoader::DefaultTexture())
		{
			loadingspinner.fill(loadingspinner[0]);
		}
		else
		{
			// Fill the rest of the array with the frames with the matching name + _<number>
			string spinnerpath = loadingspinner[0]->path;
			auto name = FileSystem::StripAll(spinnerpath);
			auto path = FileSystem::ExtractDir(spinnerpath);
			auto ext = FileSystem::ExtractExt(spinnerpath);

			for (std::size_t i = 0; i < loadingspinner.size(); i++)
			{
				if (i == 0) continue;
				auto index = "_" + String((int)i);
				auto frame = path + "/" + name + index + "." + ext;
				loadingspinner[i] = AssetLoader::LoadTexture(frame);
			}
		}

		scene->SetLoadingBackground(loadingbackground);
	}
}

void SampleUI::DrawLoadingScreen()
{
	// Increase tick!
	loadtick++;

	// If the tick is higher than the framecount, reset.
	if (loadtick > (int)loadingspinner.size()-1) loadtick = 0;

	auto framebuffer = window->GetFramebuffer();
	if (framebuffer)
	{
		if (framebuffer->GetBlendMode() != Blend::Alpha) framebuffer->SetBlendMode(Blend::Alpha);
		framebuffer->SetColor(1, 1, 1, 1);
		if (loadingicon) framebuffer->DrawImage(loadingicon, framebuffer->GetWidth() - loadingicon->GetWidth(), framebuffer->GetHeight() - loadingicon->GetHeight());

		// Animate the spinner.
		if (!loadingspinner.empty())
		{
			if (loadingspinner[loadtick] != NULL)
			{
				if (loading) Time::Delay(200);
				framebuffer->DrawImage(loadingspinner[loadtick], 8, 8, 96, 96);
			}
		}
	}

	if (loadtick > 1) loading = true;
}

void LoadingScreenDrawCallback(Object* source, Object* extra)
{
	auto scene = CastObject<Scene>(source);
	if (!scene) return;

	// Only show if we're actually loading a map!
	if (!scene->IsLoading()) return;

	auto ui = CastObject<SampleUI>(extra);
	if (ui)
	{
		ui->DrawLoadingScreen();
	}
}

void SampleUI::UpdateLoadingBackgroundCallback(Object* source, Object* extra)
{
	auto scene = CastObject<Scene>(source);
	if (!scene) return;

	// Allow custom maps to have their own loading screens!
	auto ui = CastObject<SampleUI>(extra);
	if (ui)
	{
		// Hide Curtain
		ui->curtain->Hide();

		// Hide the Menu Panel
		ui->HideMenuPanel();

		// If there's no map, ensure our background screen is set!
		if (Scene::nextmaptoload.empty())
		{
			scene->SetLoadingBackground(ui->loadingbackground);
			return;
		}

		// This can't work if loadingbackground is null!
		if (ui->loadingbackground)
		{
			auto name = FileSystem::StripAll(Scene::nextmaptoload);
			auto path = FileSystem::ExtractDir(ui->loadingbackground->path);
			auto final = path + "/" + name + ".tex";

			// The file doesn't exist, use the default one present.
			if (FileSystem::GetFileType(final) != NULL)
			{
				scene->SetLoadingBackground(AssetLoader::LoadTexture(final));
			}
			else
			{
				scene->SetLoadingBackground(ui->loadingbackground);
			}
		}
	}
}

// Menu Panel
void SampleUI::BuildMenuPanel()
{
	DPrint("Building menu");

	int titleoffset = 32;

	// Load resource again to get menu panel data.
	if (resourcefile != NULL)
	{
		// Menu
		if (resourcefile["menu"].is_object())
		{
			if (resourcefile["menu"]["margin"].is_number_unsigned()) margin = resourcefile["menu"]["margin"].get<int>();
			if (resourcefile["menu"]["menuposY"].is_number()) menuposY = resourcefile["menu"]["menuposY"].get<int>();
			if (resourcefile["menu"]["titleoffset"].is_number()) titleoffset = resourcefile["menu"]["titleoffset"].get<int>();
		}
	}

	int y = menuposY;
	auto framebuffer = window->GetFramebuffer();

	// Curtain
	curtain = App::CreateGadget<UIGadget>(window);
	curtain->SetColor(0.0f, 0.0f, 0.0f, 0.5f);
	curtain->SetSize(iVec2(framebuffer->GetWidth(), framebuffer->GetHeight()));
	curtain->Hide();

	if (framebuffer) y = (framebuffer->GetHeight() / 2) + menuposY;

	menupanel = App::CreateGadget<UIGadget>(window);
	menupanel->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
	menupanel->SetPosition(iVec2(margin, y));
	menupanel->SetSize(iVec2(150, 150)); //150^2

	title = App::CreateGadget<UIGadget>(window);
	title->SetFont(fonttitle);
	title->SetText(Program::GetTitle());

	title->SetPosition(iVec2(margin - 26, y - title->GetSize().y - titleoffset));
	title->SetParent(menupanel);
	title->SetDropShadowMode(true);

	// Buttons.
	if (resourcefile != NULL)
	{
		if (resourcefile["menu"]["items"].is_object())
		{
			//for (const auto& b : resourcefile["menu"].items())
			for (std::size_t i = 0; i < resourcefile["menu"]["items"].size(); i++)
			{
				std::string index = String(i);
				if (resourcefile["menu"]["items"][index].is_object())
				{
					auto btn = App::CreateGadget<UIGadget>(window);
					btn->SetFont(fontmenu);

					// Button keyvalues
					if (resourcefile["menu"]["items"][index]["label"].is_string())
					{
						btn->keyvalues["label"] = resourcefile["menu"]["items"][index]["label"].get<std::string>();
						btn->SetText(btn->keyvalues["label"]);
					}
					if (resourcefile["menu"]["items"][index]["command"].is_string()) btn->keyvalues["command"] = resourcefile["menu"]["items"][index]["command"].get<std::string>();

					// In-game version
					if (resourcefile["menu"]["items"][index]["label_ingame"].is_string())
					{
						btn->keyvalues["label_ingame"] = resourcefile["menu"]["items"][index]["label_ingame"].get<std::string>();
						btn->SetText(btn->keyvalues["label_ingame"]);
					}
					if (resourcefile["menu"]["items"][index]["command_ingame"].is_string()) btn->keyvalues["command_ingame"] = resourcefile["menu"]["items"][index]["command_ingame"].get<std::string>();

					if (resourcefile["menu"]["color"].is_array() && resourcefile["menu"]["color"].size() == 4)
					{
						btn->SetColor(resourcefile["menu"]["color"][0], 
							resourcefile["menu"]["color"][1], 
							resourcefile["menu"]["color"][2], 
							resourcefile["menu"]["color"][3], UIGADGET_COLOR_BASE);
					}
					btn->SetDropShadowMode(true);

					/*
					if (resourcefile["menu"]["highlightcolor"].is_array() && resourcefile["menu"]["highlightcolor"].size() == 4)
					{
						btn->SetColor(resourcefile["menu"]["highlightcolor"][0], 
							resourcefile["menu"]["color"][1], 
							resourcefile["menu"]["highlightcolor"][2], 
							resourcefile["menu"]["highlightcolor"][3], UIGADGET_COLOR_HIGHLIGHT);
					}

					if (resourcefile["menu"]["sunkencolor"].is_array() && resourcefile["menu"]["sunkencolor"].size() == 4)
					{
						btn->SetColor(resourcefile["menu"]["sunkencolor"][0], 
							resourcefile["menu"]["sunkencolor"][1], 
							resourcefile["menu"]["sunkencolor"][2],
							resourcefile["menu"]["sunkencolor"][3], UIGADGET_COLOR_SUNKEN);
					}
					*/

					btn->Hide();
					buttons.push_back(btn);
				}
			}
		}
	}


	std::map<int, iVec2> buttonpos;
	buttonpos[0] = iVec2(menupanel->GetPosition().x, menupanel->GetPosition().y);

	for (std::size_t i = 0; i < buttons.size(); i++)
	{
		buttons[i]->SetPosition(buttonpos[i]);
		buttons[i]->SetParent(menupanel);
		buttons[i]->EnableListening();

		// Prepare next position.
		if (i + 1 < buttons.size()) buttonpos[i + 1] = iVec2(buttons[i]->GetPosition().x, buttons[i]->GetPosition().y + buttons[i]->GetSize().y + 16);
	}
	title->Hide();

	menupanel->nodraw = true;
	menupanel->Hide();
}

void SampleUI::ShowMenuPanel(const bool ingame)
{
	DPrint("Showing menu");

	this->ingame = ingame;

	// Refresh
	RefreshMenu(iVec2(0, 0));

	menupanel->FadeIn(1.0f, 0.05f);

	// If we're paused, show the curtain.
	if (this->ingame) curtain->Show();

	// Show stats if enabled.
	ShowStats(stats.GetInt());

	menushown = true;

	// If the menu panel is shown, enforce the global action set.
	Input::SetActionSet("");
}

void SampleUI::HideMenuPanel()
{
	DPrint("Hiding menu");

	menupanel->Hide();

	if (curtain->GetHidden() || this->ingame) curtain->Hide();

	// Close all widgets!
	if (this->ingame)
	{
		ForceHidePanels();
	}

	menushown = false;
}

void SampleUI::RefreshMenu(const iVec2& sz)
{
	DPrint("Refreshing menu");

	// Resize if valid.
	if (sz.x != 0 && sz.y != 0)
	{
		int y = menuposY;
		y = (sz.y / 2) + menuposY;
		menupanel->SetPosition(iVec2(margin, y));
		curtain->SetSize(sz);
	}

	// Check to see if we're in a map.
	if (Scene::GetCurrent())
	{
		// Rename buttons if they have a "label_ingame" keyvalue.
		for (std::size_t i = 0; i < buttons.size(); i++)
		{
			auto kv = buttons[i]->keyvalues;
			if (ingame)
			{
				if (!kv["label_ingame"].empty())
				{
					buttons[i]->SetText(kv["label_ingame"]);
				}
			}
			else
			{
				if (!kv["label"].empty())
				{
					buttons[i]->SetText(kv["label"]);
				}
			}
		}
	}

	if (widgetsopened > 0)
	{
		curtain->Show();
	}
	else if (menupanel->GetHidden())
	{
		curtain->Hide();
	}

	// Hide/Disable if any other gadgets are on the screen.
	// TODO: Dim or put a gradient effect over the buttons.
	//menupanel->enabled = (widgetsopened == 0);
}

// Event Callbacks
bool SampleUI::EventCallback(const Event& e, Leadwerks::Object* extra)
{
	auto elem = CastObject<SampleUI>(extra);
	if (elem == NULL) return false;
	return elem->ProcessEvent(e);
}

bool SampleUI::ProcessEvent(const Event& e)
{
	if (e.id == Event::WindowSize && e.extra == window)
	{
		RefreshMenu(e.size);
	}
	else if (e.id == Event::WidgetAction)
	{
		if (e.source == quitconfirmgadget)
		{
			if (e.data == 0) EmitEvent(Event::Quit);	
		}
		else
		{
			for (std::size_t i = 0; i < buttons.size(); i++)
			{
				if (e.source == buttons[i])
				{
					std::string cmd = "command";
					if (!ingame)
					{
						auto command = buttons[i]->keyvalues[cmd];
						if (!command.empty()) ExecuteCommand(command);
					}
					else
					{
						auto gamecmd = cmd + "_ingame";
						auto command = buttons[i]->keyvalues[gamecmd];
						if (!command.empty())
							ExecuteCommand(command);
						else
						{
							auto command = buttons[i]->keyvalues[cmd];
							if (!command.empty()) ExecuteCommand(command);
						}
					}
				}
			}
		}
	}
	else if (e.id == Event::WidgetOpen)
	{
		if (e.source == quitconfirmgadget)
		{
			ForceHideConsole();
			ToggleSettingsDialog(false);
			menupanel->Hide();
		}

		auto scene = Scene::GetCurrent();
		if (scene)
		{
			// This is a dumb hack to fix widgetsopened being the max int value when set to 0 for some reason.
			if (widgetsopened > 1000) widgetsopened = 0;

			if (widgetsopened == 0)
			{
				curtain->FadeIn(0.5f);
			}
			widgetsopened++;
		}
	}
	else if (e.id == Event::WidgetClose)
	{
		if (e.source == quitconfirmgadget)
		{
			if (menupanel->GetHidden()) menupanel->Show();
		}

		widgetsopened--;
		if (!timepausestate)
		{
			if (ingame)
			{
				if (widgetsopened == 0 && menupanel->GetHidden())
				{
					curtain->FadeOut();
				}
			}
			else
			{
				if (widgetsopened == 0) curtain->FadeOut();
			}
		}
	}

	return true;
}

void SampleUI::ForceHidePanels()
{
	// TODO: Maybe store a bool array to keep track of what was 
	// opened before being forced hidden??
	if (maplistgadget) maplistgadget->Hide();
	if (settinggadget) settinggadget->Hide();
	if (quitconfirmgadget) quitconfirmgadget->Hide();
	if (console) console->Hide();
	widgetsopened = 0;
}

// Scene callback
void SampleUI::SceneLoadedCallback(Object* source, Object* extra)
{
	auto scene = CastObject<Scene>(source);
	if (!scene) return;

	auto ui = CastObject<SampleUI>(extra);
	if (ui)
	{
		// Force hide the console if the map loaded.
		ui->ForceHideConsole();

		// Always hide opened panels
		ui->ForceHidePanels();

		// We're def not loading anymore.
		ui->loading = false;

		// Hide the Menu panel.
		ui->HideMenuPanel();
	}
}

void SampleUI::SceneClearCallback(Object* source, Object* extra)
{
	auto scene = CastObject<Scene>(source);
	if (!scene) return;

	auto ui = CastObject<SampleUI>(extra);
	if (ui)
	{
		// Reset the background.
		UpdateLoadingBackgroundCallback(source, extra);

		ui->ForceHidePanels();
		ui->loading = false;
		ui->ingame = false;

		// If we're not in a map nor loading anything, show the menu!
		// If we're doing something, hide it!
		if (!scene->InMap() && Scene::nextmaptoload.empty())
		{
			ui->ShowMenuPanel();
		}
		else
		{
			ui->HideMenuPanel();
		}
	}
}

SampleUI* SampleUI::Create(App::GraphicsWindow* window)
{
	currentui = new SampleUI;
	currentui->window = window;

	auto scene = Scene::GetCurrent();
	if (scene)
	{
		SetCallback(CALLBACK_SCENELOAD, scene, SampleUI::UpdateLoadingBackgroundCallback, currentui);
		SetCallback(CALLBACK_LOADINGSCREEN, scene, LoadingScreenDrawCallback, currentui);
		SetCallback(CALLBACK_SCENECOMPLETE, scene, SampleUI::SceneLoadedCallback, currentui);
		SetCallback(CALLBACK_SCENECLEAR, scene, SampleUI::SceneClearCallback, currentui);
		SetCallback(CALLBACK_SCENEREADY, scene, SampleUI::SceneClearCallback, currentui);
	}

	ListenEvent(EVENT_NONE, NULL, SampleUI::EventCallback, currentui);
	currentui->Start();
	return currentui;
}