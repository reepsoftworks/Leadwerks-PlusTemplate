#include "pch.h"
#include "App.h"

#if defined (_WIN32)
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif
#endif

namespace App
{
	using namespace Leadwerks;

    bool GraphicsWindow::EventCallback(const Event& e, Leadwerks::Object* extra)
    {
        auto elem = CastObject<GraphicsWindow>(extra);
        return elem->ProcessEvent(e);
    }

    bool GraphicsWindow::ProcessEvent(const Event& e)
    {
        if (e.id == EVENT_STARTRENDERER)
        {
            if (splash != NULL)
            {
                delaytimer = Timer::Create(2000);
            }
            else
            {
                if (showonready)
                {
                    Activate();
                }
            }
        }
        else if (e.id == Event::TimerTick && e.source == delaytimer)
        {
            delaytimer->Release();
            delaytimer = NULL;
            if (splash)
            {
                splash->Close();
                splash->Release();
                splash = NULL;
            }
            Activate();
        }
        else if (e.id == Event::WindowClose && e.source == window_ptr)
        {
            window_ptr->Hide();
            EmitEvent(Event::WindowClose, this);
        }

        return true;
    }

	GraphicsWindow::GraphicsWindow()
	{
        title.clear();
        window_ptr = NULL;
        framebuffer = NULL;
        ui = NULL;
        splash = NULL;
	}

	GraphicsWindow::~GraphicsWindow()
	{
        if (framebuffer != NULL)
        {
            framebuffer->Release();
            framebuffer = NULL;
        }

        if (window_ptr != NULL)
        {
            window_ptr->Release();
            window_ptr = NULL;
        }

        title.clear();
	}

    bool GraphicsWindow::Initialize(const GraphicWindowSettings& settings)
    {
        // Release existing pointers so we can rebuild!
        static bool firstwindow = true;

        // Set the cursor to default before we delete the window.
        if (window_ptr)
        {
            window_ptr->ShowMouse();
        }

        if (framebuffer != NULL)
        {
            framebuffer->Release();
            framebuffer = NULL;
        }

        if (window_ptr != NULL)
        {
            window_ptr->Release();
            window_ptr = NULL;
        }

        currentsettings.size.x = settings.size.x;
        currentsettings.size.y = settings.size.y;
        currentsettings.style = settings.style;

        int window_style = Leadwerks::Window::Titlebar | Leadwerks::Window::Center | Leadwerks::Window::Hidden;
        iVec2 final_windowsize = currentsettings.size;
        switch (settings.style)
        {
        case GRAPHICSWINDOW_TITLEBAR:
            window_style = Leadwerks::Window::Titlebar | Leadwerks::Window::Center | Leadwerks::Window::Hidden;
            break;

        case GRAPHICSWINDOW_BORDERLESS:
            window_style = Leadwerks::Window::Center | Leadwerks::Window::Hidden;
            break;

        case GRAPHICSWINDOW_FULLSCREEN:
            window_style = Leadwerks::Window::FullScreen | Leadwerks::Window::Center | Leadwerks::Window::Hidden;
            break;

        case GRAPHICSWINDOW_FULLSCREENNATIVE:
            final_windowsize.x = OS::GetDisplaySize().x;
            final_windowsize.y = OS::GetDisplaySize().y;
            window_style = Leadwerks::Window::FullScreen | Leadwerks::Window::Center | Leadwerks::Window::Hidden;
            break;

        default:
            break;
        }

        // TODO: Set the title depending on app mode.
        title = title;

        window_ptr = Window::Create(title, 0, 0, final_windowsize.x, final_windowsize.y, window_style);
        window_ptr->Hide();
        Debug::Assert(window_ptr != NULL, "Failed to create window!");
        OS::SetWindowIcon(window_ptr);
        OS::SetWindowTitlebarTheme(window_ptr, OS::ThemeSystem);
        
        framebuffer = Framebuffer::Create(window_ptr);
        Debug::Assert(framebuffer != NULL, "Failed to create framebuffer!");

        //if (ui == NULL) ui = Interface::Create(framebuffer);
        //Debug::Assert(ui != NULL, "Failed to create Interface!");
        //ui->window = window_ptr;
        //ui->context = framebuffer;

        if (firstwindow)
        {
            firstwindow = false;
        }
        else
        {
            showonready = true;
            window_ptr->Show();
        }

        window_ptr->Activate();

#if defined (_WIN32)
        RAWINPUTDEVICE Rid[1];
        Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
        Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
        Rid[0].dwFlags = RIDEV_INPUTSINK;
        Rid[0].hwndTarget = current->window_ptr->GetHandle();

        if (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) == FALSE)
        {
            Print("Error: Failed to register raw input devices.");
        }

        // Replace the engine's proc with ours.
        WNDPROC OldProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(current->window_ptr->GetHandle(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(GfxWndProc)));
#endif

        // Send this event with the size of the framebuffer.
        EmitEvent(Event::WindowSize, window_ptr, 0, 0, 0, framebuffer->GetWidth(), framebuffer->GetHeight(), this);

        return true;
    }

    void GraphicsWindow::Activate()
    {
        if (window_ptr)
        {
            window_ptr->Activate();
            window_ptr->Update();

            // Call EVENT_WINDOWPAINT only if we're being shown for the first time.
            if (window_ptr->GetHidden())
            {
                window_ptr->Show();
                EmitEvent(Event::WindowPaint, this, 0, 0, 0, framebuffer->GetWidth(), framebuffer->GetHeight(), this);
            }
        }
    }

    void GraphicsWindow::Resize(const GraphicWindowSettings& settings)
    {
        // Only resize if any of the values differ...
        if (currentsettings.size != settings.size ||
            currentsettings.style != settings.style)
        {
            if (!Initialize(settings))
            {
                OS::MessageError("Failed to rebuild window.", "Error");
            }
        }
    }

    void GraphicsWindow::Close()
    {
        if (window_ptr)
        {
            window_ptr->Close();
        }
    }

    bool GraphicsWindow::Closed()
    {
        if (window_ptr) return window_ptr->Closed();
        return true;
    }

    bool GraphicsWindow::MouseDown(const int button)
    {
        return window_ptr->MouseDown(button);
    }

    bool GraphicsWindow::MouseHit(const int button)
    {
        return window_ptr->MouseHit(button);
    }

    bool GraphicsWindow::KeyHit(const int keycode)
    {
        return window_ptr->KeyHit(keycode);
    }

    bool GraphicsWindow::KeyDown(const int keycode)
    {
        return window_ptr->KeyDown(keycode);
    }

    static Vec2 rawmouseaxis = Vec2(0);
    Vec2 GraphicsWindow::GetMouseAxis(const float dpi = 1000)
    {
        Vec2 t = Vec2(0);
        t.x = rawmouseaxis.x / dpi;
        t.y = rawmouseaxis.y / dpi;
        return t;
    }

#if defined (_WIN32)

    LRESULT CALLBACK GfxWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
    {
        int button = 0;
        auto window = GraphicsWindow::GetCurrent();
        switch (message)
        {
        case WM_INPUT:
        {
            UINT dwSize = sizeof(RAWINPUT);
            static BYTE lpb[sizeof(RAWINPUT)];

            GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

            RAWINPUT* raw = (RAWINPUT*)lpb;

            if (raw->header.dwType == RIM_TYPEMOUSE)
            {
                rawmouseaxis.x += float(raw->data.mouse.lLastX);
                rawmouseaxis.y += float(raw->data.mouse.lLastY);
            }
            break;
        }
        case WM_CHAR:
        case WM_SYSCHAR:
            EventQueue::Emit(Event::KeyChar, window, wparam);
            break;

        case WM_MBUTTONDBLCLK:
            EventQueue::Emit(Event::MouseDown, window, Mouse::Middle);
            EventQueue::Emit(Event::DoubleClick, window, Mouse::Middle);
            break;

        case WM_RBUTTONDBLCLK:
            EventQueue::Emit(Event::MouseDown, window, Mouse::Right);
            EventQueue::Emit(Event::DoubleClick, window, Mouse::Right);
            break;

        case WM_LBUTTONDBLCLK:
            EventQueue::Emit(Event::MouseDown, window, Mouse::Left);
            EventQueue::Emit(Event::DoubleClick, window, Mouse::Left);
            break;

        case WM_XBUTTONDBLCLK:
        {
            switch (HIWORD(wparam))
            {
            case XBUTTON1:
                button = Mouse::Forward;
                break;

            case XBUTTON2:
                button = Mouse::Back;
                break;
            }

            EventQueue::Emit(Event::MouseDown, window, button, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            EventQueue::Emit(Event::DoubleClick, window, button, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            break;
        }

        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_XBUTTONDOWN:
        {
            switch (message)
            {
            case WM_LBUTTONDOWN:
                button = Mouse::Left;
                break;
            case WM_RBUTTONDOWN:
                button = Mouse::Right;
                break;
            case WM_MBUTTONDOWN:
                button = Mouse::Middle;
                break;
            case WM_XBUTTONDOWN:
                switch (HIWORD(wparam))
                {
                case XBUTTON1:
                    button = Mouse::Forward;
                    break;
                case XBUTTON2:
                    button = Mouse::Back;
                    break;
                }
                break;
            }
            EventQueue::Emit(Event::MouseDown, window, button, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            break;
        }
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_XBUTTONUP:
        {
            switch (message)
            {
            case WM_LBUTTONUP:
                button = Mouse::Left;
                break;
            case WM_RBUTTONUP:
                button = Mouse::Right;
                break;
            case WM_MBUTTONUP:
                button = Mouse::Middle;
                break;
            case WM_XBUTTONUP:
                switch (HIWORD(wparam))
                {
                case XBUTTON1:
                    button = Mouse::Forward;
                    break;
                case XBUTTON2:
                    button = Mouse::Back;
                    break;
                }
                break;
            }
            EventQueue::Emit(Event::MouseUp, window, button, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            break;
        }
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            EventQueue::Emit(Event::KeyDown, window, wparam);
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            EventQueue::Emit(Event::KeyUp, window, wparam);
            break;

        case WM_ACTIVATE:
            //EventQueue::Emit(Event::WindowPaint, window);
            //return 0L;
            break;
        case WM_CLOSE:
            EventQueue::Emit(Event::WindowClose, window);
            break;

        default:
            break;
        }

        return Leadwerks::WndProc(hwnd, message, wparam, lparam);
    }
#endif

    GraphicsWindow* GraphicsWindow::current = NULL;
    GraphicsWindow* GraphicsWindow::GetCurrent()
    {
        return current;
    }

    Leadwerks::Framebuffer* GraphicsWindow::GetFramebuffer()
    {
        window_ptr->Update();
        return framebuffer;
    }

    GraphicsWindow* GraphicsWindow::Create(const std::string& title, const GraphicWindowSettings& settings, const bool showonready)
    {
        if (!current)
        {
            current = new GraphicsWindow();
            current->title = title;
            current->showonready = showonready;

            if (current->Initialize(settings))
            {
                ListenEvent(EVENT_NONE, NULL, GraphicsWindow::EventCallback, current);
            }
            else
            {
                current = NULL;
            }
        }

        return current;
    }
}