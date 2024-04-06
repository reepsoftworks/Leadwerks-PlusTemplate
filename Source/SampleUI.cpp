#include "pch.h"
#include "SampleUI.h"

using namespace Leadwerks;
using namespace App;

SampleUI* SampleUI::currentui = NULL;

// UI Commands
void CC_OpenSettings(std::vector<std::string> pArgV)
{
	if (SampleUI::currentui)
	{
		SampleUI::currentui->ToggleSettingsDialog(true);
	}
}
static ConCommand settingsdialog("settingsdialog", CC_OpenSettings, CVAR_DEFAULT, "Opens settings panel. Usage: settingsdialog");

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
	statsgadget = NULL;
	settinggadget = NULL;
	console = NULL;
	loadingbackground = NULL;
	loadingicon = NULL;
	loadingspinner.fill(NULL); 
	loadtick = 0;

	menushown = false;
	margin = 0;
	title = NULL;
	fonttitle = NULL;
	fontmenu = NULL;
	resourcefile = NULL;
	widgetsopened = 0;

	buttons.clear();
}

SampleUI::~SampleUI()
{
	FreeObject(console);
	FreeObject(statsgadget);
	FreeObject(settinggadget);
	
	FreeObject(loadingbackground);
	FreeObject(loadingicon);

	FreeObject(fonttitle);
	FreeObject(fontmenu);

	for (const auto& btn : buttons)
	{
		FreeObject(btn);
	}

	loadtick = 0;
	menushown = false;
	margin = 0;
	resourcefile = NULL;
	buttons.clear();
	widgetsopened = 0;

	RemoveCallback(this);
}

void SampleUI::Start()
{
	// Create our gadgets.
	settinggadget = CreateGadget<SettingsGadget>(window);
	statsgadget = CreateGadget<StatsGadget>(window);
	console = CreateGadget<ConsoleGadget>(window);
	
	if (!console || !settinggadget || !statsgadget)
	{
		OS::MessageError("Error", "Failed to create one ore more gadgets!");
		Debug::Stop();
	}

	// Apply visiblity
	console->Hide();
	settinggadget->Hide();
	ShowStats(stats.GetInt());

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
	Translator::LoadTokenList("UI/gameui_english.json");

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
				Time::Delay(200);
				framebuffer->DrawImage(loadingspinner[loadtick], 8, 8, 128, 128);
			}
		}
	}
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

		// Wait.
		//Time::Delay(10);

		// Draw the loading screen.
		ui->DrawLoadingScreen();
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
	menupanel->SetSize(iVec2(150, 150));

	title = App::CreateGadget<UIGadget>(window);
	title->SetFont(fonttitle);
	title->SetText(Program::GetTitle());


	title->SetPosition(iVec2(margin - 26, y - title->GetSize().y - titleoffset));
	title->SetParent(menupanel);

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

	// Refresh
	RefreshMenu(iVec2(0, 0));

	menupanel->FadeIn(1.0f, 0.10f);

	// If we're paused, show the curtain.
	//if (timepausestate) curtain->Show();

	menushown = true;
}

void SampleUI::HideMenuPanel()
{
	DPrint("Hiding menu");

	menupanel->Hide();
	//curtain->Hide();

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
			if (Scene::GetCurrent()->InMap())
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
		for (std::size_t i = 0; i < buttons.size(); i++)
		{
			if (e.source == buttons[i])
			{
				auto scene = Scene::GetCurrent();
				if (scene)
				{
					std::string cmd = "command";
					bool ingame = Scene::GetCurrent()->InMap();
					if (ingame) cmd += "_ingame";
					auto command = buttons[i]->keyvalues[cmd];
					if (!command.empty()) ExecuteCommand(command);
				}
			}
		}
	}
	else if (e.id == Event::WidgetOpen)
	{
		auto scene = Scene::GetCurrent();
		if (scene)
		{
			if (Scene::GetCurrent()->InMap())
			{
			}

			if (widgetsopened == 0)
			{
				curtain->FadeIn(0.5f);
			}
			widgetsopened++;
		}
	}
	else if (e.id == Event::WidgetClose)
	{
		auto scene = Scene::GetCurrent();
		if (scene)
		{
			if (Scene::GetCurrent()->InMap())
			{
			}	
		}

		widgetsopened--;
		if (!timepausestate)
		{
			if (widgetsopened == 0) curtain->FadeOut();
			
		}
	}

	return true;
}

// Scene callback
void SampleUI::SceneLoadedCallback(Object* source, Object* extra)
{
	auto scene = CastObject<Scene>(source);
	if (!scene) return;

	auto ui = CastObject<SampleUI>(extra);
	if (ui)
	{
		// TODO:
		// When we have an input system, check to see if the 
		// Action Set is currently not the default. If it's not
		// Forcefully hide the console!
		/*
		if (!Input::ActionState().empty())
		{
			ui->ToggleConsole(false);
		}
		*/

		// Always hide opened panels
		ui->ToggleSettingsDialog(false);
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
		SetCallback(CALLBACK_LOADINGSCREEN, scene, LoadingScreenDrawCallback, currentui);
		SetCallback(CALLBACK_SCENELOAD, scene, SampleUI::UpdateLoadingBackgroundCallback, currentui);
		SetCallback(CALLBACK_SCENECOMPLETE, scene, SampleUI::SceneLoadedCallback, currentui);
		SetCallback(CALLBACK_SCENECLEAR, scene, SampleUI::SceneClearCallback, currentui);
		SetCallback(CALLBACK_SCENEREADY, scene, SampleUI::SceneClearCallback, currentui);
	}

	ListenEvent(EVENT_NONE, NULL, SampleUI::EventCallback, currentui);
	currentui->Start();
	return currentui;
}