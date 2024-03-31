#pragma once
#include "App.h"

namespace App
{
	class GameMenu : public Leadwerks::Object
	{
		Leadwerks::Interface* ui;
	protected:
		virtual bool ProcessEvent(const Leadwerks::Event& e);
	public:
		GraphicsWindow* window;

		GameMenu();
		~GameMenu();

		virtual void BuildUI(Leadwerks::Widget* root);
		static bool EventCallback(const Leadwerks::Event& e, Leadwerks::Object* extra);
	};

	template <class T>
	T* CreateGameMenu(GraphicsWindow* window)
	{
		auto menu = new T();
		menu->window = window;
		ListenEvent(EVENT_NONE, NULL, GameMenu::EventCallback, menu);
		return menu;
	};
}