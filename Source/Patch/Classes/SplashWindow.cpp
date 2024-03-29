#include "pch.h"
#include "../LeadwerksPlus.h"

#if defined (_WIN32)
#include "Shobjidl.h"
#endif

namespace Leadwerks
{
#if defined (_WIN32)
	static bool g_bStartGDI = false;
	static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	static ULONG_PTR gdiplusToken;

	const bool StartGDI()
	{
		if (!g_bStartGDI)
		{
			Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
			g_bStartGDI = true;
		}

		return g_bStartGDI;
	}

	static bool SplashWindowClassInitialized = false;
	static bool InitializeSplashWindowClass()
	{
		WNDCLASS wndclass;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = NULL;
		wndclass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.lpfnWndProc = SplashWndProc;
		wndclass.lpszMenuName = 0;
		wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;

		wndclass.lpszClassName = TEXT("SPLASH_WINDOW_CLASS");
		RegisterClass(&wndclass);

		StartGDI();

		SplashWindowClassInitialized = true;
		return SplashWindowClassInitialized;
	}

	LRESULT CALLBACK SplashWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		if (GetWindowLongA(hwnd, GWLP_USERDATA) != 0)
		{
			SplashWindow* window = (SplashWindow*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

			switch (message)
			{
			case WM_ACTIVATE:
				EventQueue::Emit(Event::WindowPaint, window);
				return 0L;
				break;
			case WM_CLOSE:
				EventQueue::Emit(Event::WindowClose, window);
				break;
			case WM_PAINT:
				return 0L;
				break;
			case WM_ERASEBKGND:
				return 1L;
				break;
			}
		}

		return DefWindowProcW(hwnd, message, wparam, lparam);
	}

#endif
	SplashWindow::~SplashWindow()
	{
		Destroy();
	}

	void SplashWindow::Build(const std::wstring& imagepath, const bool showintaskbar)
	{
#if defined (_WIN32)
		if (InitializeSplashWindowClass())
		{
			Gdiplus::Image image(imagepath.c_str());

			int w = image.GetWidth();
			int h = image.GetHeight();

			//printf("%d,%d\n", w, h);

			int wstyle = CS_HREDRAW | CS_VREDRAW | WS_POPUP;
			int xstyle = 0;

			// center 
			int px = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
			int py = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

			hwnd = CreateWindowExA(xstyle, "SPLASH_WINDOW_CLASS", "splash", wstyle, px, py, w, h, NULL, NULL, NULL, NULL);
			SetWindowLong(hwnd, GWL_STYLE, WS_EX_TRANSPARENT);
			hdc = GetDC(hwnd);

			SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)this);

			ShowWindow(hwnd, 1);

			// Activate
			SetForegroundWindow(hwnd);
			//BringWindowToTop(hwnd);
			SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			SetActiveWindow(hwnd);
			SetFocus(hwnd);

			gfx = new Gdiplus::Graphics(hdc);
			Gdiplus::Rect rect = { 0, 0, (INT)image.GetWidth(), (INT)image.GetHeight() };
			if (gfx) gfx->DrawImage(&image, rect);

			HRESULT hr;
			ITaskbarList* pTaskbarList;
			hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_ITaskbarList, (void**)&pTaskbarList);
			if (hr == S_OK)
			{
				hr = pTaskbarList->HrInit();

				if (hr == S_OK)
				{
					if (showintaskbar)
					{
						//pTaskbarList->AddTab(hwnd);
					}
					else
					{
						pTaskbarList->DeleteTab(hwnd);
					}
				}

				pTaskbarList->Release();
			}
		}
#endif
	}

	void SplashWindow::Close()
	{
#if defined (_WIN32)
		if (hwnd != NULL)
		{
			CloseWindow(hwnd);
		}
#endif
	}

	void SplashWindow::Destroy()
	{
#if defined (_WIN32)
		delete gfx;
		gfx = NULL;
		Gdiplus::GdiplusShutdown(gdiplusToken);
		g_bStartGDI = false;

		if (hwnd != NULL)
		{
			DestroyWindow(hwnd);
			hwnd = NULL;
		}
#endif
	}

	SplashWindow* SplashWindow::Create(const std::wstring& imagepath, const bool showintaskbar)
	{
		auto splashwindow = new SplashWindow();
		splashwindow->Build(imagepath, showintaskbar);
		return splashwindow;
	}
}
