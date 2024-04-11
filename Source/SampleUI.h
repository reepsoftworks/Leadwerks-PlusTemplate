#pragma once
#include "pch.h"
#include "App/App.h"

class SampleApp;
class SampleUI : public Leadwerks::Object
{
	App::GraphicsWindow* window;
	App::StatsGadget* statsgadget;
	App::SettingsGadget* settinggadget;
	App::MapListGadget* maplistgadget;
	App::QuitConfirmGadget* quitconfirmgadget;
	App::ConsoleGadget* console;
	nlohmann::json resourcefile;

	// Console toggling
	void ToggleConsole();

	// Loading Screen
	int loadtick;
	bool loading;
	Leadwerks::Texture* loadingbackground;
	Leadwerks::Texture* loadingicon;
	std::array<Leadwerks::Texture*, 8> loadingspinner; // 8 dots.
	void BuildLoadingScreen();
	static void UpdateLoadingBackgroundCallback(Object* source, Object* extra);

	// Menu Panel
	bool menushown;
	Leadwerks::Font* fonttitle;
	Leadwerks::Font* fontmenu;
	int margin;
	int menuposY; // This is the y position from the center!
	App::UIGadget* title;
	App::UIGadget* menupanel;
	App::UIGadget* curtain;
	std::vector<App::UIGadget*> buttons;
	unsigned int widgetsopened;
	bool ingame;

	void BuildMenuPanel();
	void RefreshMenu(const Leadwerks::iVec2& sz);

	// Callbacks/Events
	static void SceneLoadedCallback(Object* source, Object* extra);
	static void SceneClearCallback(Object* source, Object* extra);
	static bool EventCallback(const Leadwerks::Event& e, Leadwerks::Object* extra);
	virtual bool ProcessEvent(const Leadwerks::Event& e);

public:
	SampleUI();
	~SampleUI();

	void Start();
	void ToggleSettingsDialog(const bool show);
	void CallMapList();
	void CallQuit();
	void ForceHideConsole();
	void ForceHidePanels();
	void ShowStats(const int mode);

	// Loading Screen
	void DrawLoadingScreen();

	// Menu Panel
	void ShowMenuPanel(const bool ingame = false);
	void HideMenuPanel();

	static SampleUI* currentui;
	static SampleUI* Create(App::GraphicsWindow* window);

	friend class SampleApp;
};