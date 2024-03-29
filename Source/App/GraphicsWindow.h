#pragma once
#include "App.h"

namespace App
{
	enum GraphicWindowStyles
	{
		GRAPHICSWINDOW_TITLEBAR,
		GRAPHICSWINDOW_BORDERLESS,
		GRAPHICSWINDOW_FULLSCREEN,
		GRAPHICSWINDOW_FULLSCREENNATIVE,
	};

	struct GraphicWindowSettings
	{
		Leadwerks::iVec2 size{ Leadwerks::iVec2(1280, 720) };
		GraphicWindowStyles style{ GRAPHICSWINDOW_TITLEBAR };
	};

	class GraphicsWindow : public Leadwerks::Object
	{
		static GraphicsWindow* current;
	protected:
		std::string title;
		Leadwerks::Window* window_ptr;
		Leadwerks::Framebuffer* framebuffer;
		Leadwerks::Interface* ui;
		Leadwerks::Timer* delaytimer;
		GraphicWindowSettings currentsettings;
		bool showonready;

		bool Initialize(const GraphicWindowSettings& settings);
		static bool EventCallback(const Leadwerks::Event& e, Leadwerks::Object* extra);
		virtual bool ProcessEvent(const Leadwerks::Event& e);
	public:
		Leadwerks::SplashWindow* splash;

		GraphicsWindow();
		virtual ~GraphicsWindow();

		virtual void Activate();
		virtual void Resize(const GraphicWindowSettings& settings);
		virtual void Close();
		virtual bool Closed();

		bool KeyHit(const int keycode);
		bool KeyDown(const int keycode);
		bool MouseHit(const int button);
		bool MouseDown(const int button);
		Leadwerks::Vec2 GetMouseAxis(const float dpi = 1000);

		static GraphicsWindow* Create(const std::string& title, const GraphicWindowSettings& settings, const bool showonready = true);
		static GraphicsWindow* GetCurrent();
		Leadwerks::Framebuffer* GetFramebuffer();
		GraphicWindowSettings CurrentSettings() { return currentsettings; };
	};

#if defined (_WIN32)
	LRESULT CALLBACK GfxWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
#endif
}