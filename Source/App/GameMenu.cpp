#include "pch.h"
#include "App.h"

namespace App
{
	using namespace Leadwerks;

	GameMenu::GameMenu()
	{
		window = NULL;
		ui = NULL;
	}

	GameMenu::~GameMenu()
	{
		ui = NULL;
		window = NULL;
	}

	bool GameMenu::EventCallback(const Leadwerks::Event& e, Leadwerks::Object* extra)
	{
		auto elem = CastObject<GameMenu>(extra);
		if (elem == NULL) return false;
		return elem->ProcessEvent(e);
	}

	bool GameMenu::ProcessEvent(const Leadwerks::Event& e)
	{
		if (e.id == Event::WindowSize)
		{
			auto currentui = window->GetInterface();
			if (ui == NULL || ui != currentui)
			{
				ui = window->GetInterface();
				auto root = ui->GetBase();
				if (root != NULL)
				{
					BuildUI(root);
				}
			}
		}
		return true;
	}

	void GameMenu::BuildUI(Widget* root)
	{
	}
}