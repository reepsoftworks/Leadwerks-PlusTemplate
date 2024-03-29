#pragma once
#include "../LeadwerksPlus.h"

namespace Leadwerks
{
	class SplashWindow : public Object
	{	
#if defined (_WIN32)
		HWND hwnd;
		HDC hdc;
		Gdiplus::Graphics* gfx;
#endif
		virtual void Destroy();
		virtual void Build(const std::wstring& imagepath, const bool showintaskbar = false);
	public:
		virtual ~SplashWindow();
		virtual void Close();

		static SplashWindow* Create(const std::wstring& imagepath, const bool showintaskbar = false);
	};

	LRESULT CALLBACK SplashWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
}
