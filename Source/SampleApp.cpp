#include "pch.h"
#include "SampleApp.h"

using namespace Leadwerks;
using namespace App;

void CC_WindowMode(std::string pArgV)
{
	if (pArgV.empty())
		return;

	auto window = GraphicsWindow::GetCurrent();
	if (window)
	{
		auto settings = window->CurrentSettings();
		auto value = (GraphicWindowStyles)String::Int(pArgV);
		settings.style = value;
		window->Resize(settings);
	}
}
static ConVar windowmode("windowmode", "0", CVAR_SAVE, "Usage: fullscreen <int (0 = Normal, 1 = Borderless, 2 = Fullscreen, 3 = Native Fullscreen, 4 = Fullscreen Boarderless)>", CC_WindowMode);
static ConVar screenwidth("screenwidth", "1280", CVAR_SAVE, "Usage: screenwidth <int>");
static ConVar screenheight("screenheight", "720", CVAR_SAVE, "Usage: screenheight <int>");

// TODO: move to user app directory...
#define USER_CONFIG "config.cfg"

SampleApp::SampleApp()
{
	splashwindow = NULL;
	window = NULL;
	windowsettings = GraphicWindowSettings();
	scene = NULL;

	stats = NULL;
	console = NULL;
}

SampleApp::~SampleApp()
{
	SafeRelease(splashwindow);
	SafeRelease(window);
	SafeRelease(scene);
	SafeRelease(stats);
	SafeRelease(console);
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
		splashwindow = SplashWindow::Create(L"splash.bmp");
	}

	// Dump Command line into convars/commands.
	Program::LoadConVars(USER_CONFIG);
	Program::ExecuteMap(Program::Arguments);

	// Create the graphics window
	windowsettings.size.x = screenwidth.GetInt();
	windowsettings.size.y = screenheight.GetInt();
	windowsettings.style = (GraphicWindowStyles)windowmode.GetInt();
	window = GraphicsWindow::Create(Program::GetTitle(), windowsettings, splashwindow);
	if (!window)
	{
		OS::MessageError("Error", "Failed to create the grapics window!");
		return false;
	}

	// Init our gadgets
	stats = CreateGadget<StatsGadget>(window);
	console = CreateGadget<ConsoleGadget>(window);

	if (!stats || !console)
	{
		OS::MessageError("Error", "Failed to create one or more gadgets!");
		return false;
	}

	// Hide the console.
	console->Hide();

	// Scene
	scene = Scene::Create();
	if (!scene)
	{
		OS::MessageError("Error", "Failed to create scene!");
		return false;
	}

	return true;
}

bool SampleApp::Update()
{
	bool running = true;
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

	// Allow the console in dev/debug modes
	auto appmode = Program::GetAppMode();
	if (appmode > Program::NormalMode)
	{
		if (window->KeyHit(Key::F1)) // TODO: Replace me with the input system.
		{
			bool h = console->GetHidden();
			if (!h)
				console->Hide();
			else
				console->Show();
		}
	}

	// Fullscreen toggle.
	if (window->KeyHit(Key::F11)) // TODO: Replace me with the input system.
	{
		static bool fullscreen = false;
		if (!fullscreen)
		{
			windowsettings.style = GRAPHICSWINDOW_FULLSCREENNATIVE;
			fullscreen = true;
		}
		else
		{
			windowsettings.style = GRAPHICSWINDOW_TITLEBAR;
			fullscreen = false;
		}
		window->Resize(windowsettings);
	}

	scene->Update();
	window->Sync();
	return running;
}

void SampleApp::Shutdown()
{
	// Save convars.
	Program::SaveConVars(USER_CONFIG);
}