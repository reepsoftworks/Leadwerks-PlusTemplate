#pragma once
#include "pch.h"
#include "App/App.h"

class SampleApp
{
	Leadwerks::SplashWindow* splashwindow;
	App::GraphicsWindow* window;
	App::GraphicWindowSettings windowsettings;
	App::Scene* scene;

	App::StatsGadget* stats;
	App::ConsoleGadget* console;

	void Shutdown();

public:
	SampleApp();
	~SampleApp();

	bool Start();
	bool Update();
};

#define SafeRelease(x) if (x) { x->Release(); x = NULL; }