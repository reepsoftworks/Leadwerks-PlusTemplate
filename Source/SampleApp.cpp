#include "pch.h"
#include "SampleApp.h"

using namespace Leadwerks;
using namespace App;

extern ConVar windowmode;
extern ConVar screenwidth;
extern ConVar screenheight;

// TODO: move to user app directory...
#define USER_CONFIG "Config/config.cfg"
#define USER_CONFIGEDITOR "Config/config_editor.cfg"
#define PATH_SPLASH L"UI/splash.bmp"

SampleApp::SampleApp()
{
	splashwindow = NULL;
	window = NULL;
	scene = NULL;
	ui = NULL;
}

SampleApp::~SampleApp()
{
	FreeObject(splashwindow);
	FreeObject(window);
	FreeObject(scene);
	FreeObject(ui);
}

bool SampleApp::Start()
{
	// Check the hardware
	if (!Program::VendorCheck())
		return false;

	// Read the .werk file.
	if (!Program::LoadWerkFile())
	{
		OS::MessageError("Error", "Failed to load .werk file!");
		return false;
	}
		
	// Create splash window
	if (!Program::CheckArgument("nosplash"))
	{
		splashwindow = SplashWindow::Create(PATH_SPLASH);
	}

	// Dump Command line into convars/commands.
	Program::LoadConVars(USER_CONFIG);
	Program::ExecuteMap(Program::Arguments);

	// Gather the launching window settings.
	GraphicWindowSettings windowsettings = GraphicWindowSettings();
	const bool editormode = (bool)(Program::GetAppMode() > Program::NormalMode);
	if (!editormode)
	{
		windowsettings.size.x = screenwidth.GetInt();
		windowsettings.size.y = screenheight.GetInt();
		windowsettings.style = (GraphicWindowStyles)windowmode.GetInt();
	}
	else
	{
		// Load an editor config if one exists
		auto config = Config::Load(USER_CONFIGEDITOR);
		if (config != NULL)
		{
			windowsettings.size.x = String::Int(config->GetValue("screenwidth"));
			if (windowsettings.size.x <= 0) windowsettings.size.x = 1280;
			windowsettings.size.y = String::Int(config->GetValue("screenheight"));
			if (windowsettings.size.y <= 0) windowsettings.size.y = 720;
			windowsettings.style = (GraphicWindowStyles)String::Int(config->GetValue("windowmode"));
			config->Release();
			config = NULL;
		}
	}

	// Create the window.
	window = GraphicsWindow::Create(Program::GetTitle(), windowsettings, splashwindow);
	if (!window)
	{
		OS::MessageError("Error", "Failed to create the grapics window!");
		return false;
	}

	// Scene
	scene = Scene::Create();
	if (!scene)
	{
		OS::MessageError("Error", "Failed to create scene!");
		return false;
	}

	// Apply a global gravity to all maps.
	scene->SetGravity(18);

	// Create our UI.
	ui = SampleUI::Create(window);

	Input::Initalize();

	return true;
}

bool SampleApp::Update()
{
	bool running = true;

	// Process events.
	while (PeekEvent())
	{
		const auto e = WaitEvent();
		if (e.id == Event::Quit)
		{
			Shutdown();
			running = false;
			break;
		}
	}

	// Allow special features for debug/devmode.
	auto appmode = Program::GetAppMode();
	if (appmode > Program::NormalMode)
	{
		if (Input::ActionHit("Console"))
		{
			if (ui) ui->ToggleConsole();
		}
	
		// Fullscreen toggle.
		if (Input::ActionHit("Fullscreen"))
		{
			bool fullscreen = false;
			GraphicWindowSettings windowsettings = window->CurrentSettings();
			if (windowsettings.style == GRAPHICSWINDOW_FULLSCREENNATIVE) fullscreen = true;
			if (!fullscreen)
			{
				windowsettings.style = GRAPHICSWINDOW_FULLSCREENNATIVE;
				//fullscreen = true;
			}
			else
			{
				windowsettings.style = GRAPHICSWINDOW_TITLEBAR;
				//fullscreen = false;
			}
			window->Resize(windowsettings);
		}

		// Terminate
		if (Input::ActionHit("Terminate"))
		{
			EmitEvent(Event::Quit);
		}
	}

	// Update + sync!
	scene->Update();
	window->Sync();
	return running;
}

void SampleApp::Shutdown()
{
	// Push window settings to cvars so they are saved.
	// Skip saving these settings if we're in editor mode.
	if (window && (Program::GetAppMode() == Program::NormalMode))
	{
		auto currentwindowsettings = window->CurrentSettings();
		SetConVar("screenwidth", String(currentwindowsettings.size.x));
		SetConVar("screenheight", String(currentwindowsettings.size.y));
		SetConVar("windowmode", String(currentwindowsettings.style));

		// Save convars.
		Program::SaveConVars(USER_CONFIG);
	}
}