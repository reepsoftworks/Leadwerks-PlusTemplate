#pragma once
#include "pch.h"
#include "App/App.h"
#include "SampleUI.h"

class SampleApp
{
	Leadwerks::SplashWindow* splashwindow;
	App::GraphicsWindow* window;
	App::Scene* scene;
	SampleUI* ui;

	void Shutdown();

public:
	SampleApp();
	~SampleApp();

	bool Start();
	bool Update();
};

