#include "pch.h"
#include "App.h"
#include "Input/ButtonCode.h"

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
        if (elem == NULL) return false;
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
            if (delaytimer)
            {
                delaytimer->Release();
                delaytimer = NULL;
            }

            if (splash)
            {
                splash->Close();
                splash->Release();
                splash = NULL;
            }
            Activate();
        }
        else if (e.id == Event::WindowSize && e.source == this)
        {
            iVec2 pos = e.position;
            GraphicWindowSettings newsettings = CurrentSettings();
            newsettings.size = e.size;
            newsettings.style = (GraphicWindowStyles)e.data;

            // Only resize if any of the values differ...
            if (currentsettings.size != newsettings.size ||
                currentsettings.style != newsettings.style)
            {
                if (!Initialize(newsettings))
                {
                    OS::MessageError("Failed to rebuild window.", "Error");
                }
            }
        }
        else if (e.id == Event::WindowClose && e.source == this)
        {
            EmitEvent(Event::Quit);
        }

        return true;
    }

	GraphicsWindow::GraphicsWindow()
	{
        title.clear();
        window_ptr = NULL;
        framebuffer = NULL;
        ui = NULL;
        imgui = NULL;
        splash = NULL;
        uibasecolor = NULL;
        rawmouseaxis = Vec2(0);
	}

	GraphicsWindow::~GraphicsWindow()
	{
        if (uibasecolor != NULL)
        {
            uibasecolor->Release();
            uibasecolor = NULL;
        }

        if (imgui != NULL)
        {
            imgui->Release();
            imgui = NULL;
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

        if (ui != NULL)
        {
            ui->Release();
            ui = NULL;
        }

        if (imgui != NULL) imgui->SetContext(NULL);

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

        case GRAPHICSWINDOW_FULLSCREENBORDERLESS:
            final_windowsize.x = OS::GetDisplaySize().x;
            final_windowsize.y = OS::GetDisplaySize().y;
            window_style = Leadwerks::Window::Center | Leadwerks::Window::Hidden;
            break;

        default:
            break;
        }

        // Apply DPI scaling.
        float scale = OS::GetDisplayScale();
        if (scale > 1.0f)
        {
            final_windowsize.x *= static_cast<int>(scale);
            final_windowsize.y *= static_cast<int>(scale);
        }

        if (Program::GetAppMode() == Program::DebugMode)
        {
            std::string debugtag = " - [Debug]"; // 10;
            if (String::Right(title, (int)debugtag.length()) != debugtag)
            {
                title = title + debugtag;
            }
            else
            {
                title = title;
            }
        }
        else
        {
            title = title;
        }

        window_ptr = Window::Create(title, 0, 0, final_windowsize.x, final_windowsize.y, window_style);
        window_ptr->Hide();
        Debug::Assert(window_ptr != NULL, "Failed to create window!");
        OS::SetWindowIcon(window_ptr);
        OS::SetWindowTitlebarTheme(window_ptr, OS::ThemeSystem);
        
        framebuffer = Framebuffer::Create(window_ptr);
        Debug::Assert(framebuffer != NULL, "Failed to create framebuffer!");

        if (imgui == NULL) imgui = ImGuiLayer::Create();
        Debug::Assert(imgui != NULL, "Failed to create imgui!");
        imgui->SetContext(framebuffer);

        if (ui == NULL) ui = Interface::Create(framebuffer);
        Debug::Assert(ui != NULL, "Failed to create Interface!");
        ui->SetScale(OS::GetDisplayScale());
        ui->GetBase()->SetScript("Scripts/GUI/Panel.lua");

        if (uibasecolor == NULL) uibasecolor = new Vec4(0, 0, 0, 0);
        ui->GetBase()->SetObject("backgroundcolor", uibasecolor);

        // Make this invisable.
        //ui->GetBase()->CallFunction("SetColor", 1.0f, 0.0f, 0.0f, 1.0f, 1);

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
#if 0
        // Only resize if any of the values differ...
        if (currentsettings.size != settings.size ||
            currentsettings.style != settings.style)
        {
            if (!Initialize(settings))
            {
                OS::MessageError("Failed to rebuild window.", "Error");
            }
        }
#endif

        EmitEvent(Event::WindowSize, this, (int)settings.style, 0, 0, settings.size.x, settings.size.y);
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

    Leadwerks::iVec2 GraphicsWindow::GetSize()
    {
        if (window_ptr) return iVec2(window_ptr->GetWidth(), window_ptr->GetHeight());
        return currentsettings.size;
    }

    bool GraphicsWindow::ButtonHit(const int code)
    {
        const bool is_button_down = buttonhitstate[code];
        buttonhitstate[code] = false;
        return is_button_down;
    }

    bool GraphicsWindow::ButtonDown(const int code)
    {
        return buttondownstate[code];
    }

    bool GraphicsWindow::ButtonReleased(const int code)
    {
        const bool is_button_down = buttondownstate[code];
        const bool is_button_up = buttonreleasedstate[code];

        if (!is_button_down && !is_button_up)
        {
            buttonreleasedstate[code] = true;
            return true;
        }

        return false;
    }

    bool GraphicsWindow::ButtonAnyDown()
    {
        return std::any_of(buttondownstate.begin(), buttondownstate.end(),
            [](const auto& p) { return p.second; });
    }

    bool GraphicsWindow::ButtonAnyHit()
    {
        const bool is_button_down = anybuttonhit;
        anybuttonhit = false;
        return is_button_down;
    }

    int GraphicsWindow::LastButtonPressed()
    {
        return lastbutton;
    }

    void GraphicsWindow::SetMousePosition(const int x, const int y)
    {
        window_ptr->SetMousePosition(x, y);
    }

    Leadwerks::Vec2 GraphicsWindow::GetMousePosition()
    {
        return window_ptr->GetMousePosition();
    }

    //static Vec2 rawmouseaxis = Vec2(0);
    Vec2 GraphicsWindow::GetMouseAxis(const float dpi)
    {
        Vec2 t = Vec2(0);
        t.x = rawmouseaxis.x / dpi;
        t.y = rawmouseaxis.y / dpi;
        return t;
    }

    void GraphicsWindow::SetCursor(bool cursor)
    {
        if (cursor)
            window_ptr->ShowMouse();
        else
            window_ptr->HideMouse();
    }

    void GraphicsWindow::Flush()
    {
        buttondownstate.clear();
        buttonhitstate.clear();
        anybuttonhit = false;

        if (window_ptr != NULL)
        {
            window_ptr->FlushKeys();
            window_ptr->FlushMouse();
        }
    }

    void GraphicsWindow::PumpButtonCodeDown(const int code)
    {
        if (window_ptr == NULL) return;

        bool actually_down = false;
        if (code < InputSystem::BUTTON_FIRST_KEY)
            actually_down = window_ptr->MouseDown(code);
        else
            actually_down = window_ptr->KeyDown(code);

        buttondownstate[code] = actually_down;
        buttonhitstate[code] = actually_down;
        buttonreleasedstate[code] = !actually_down;
        anybuttonhit = actually_down;
        if (lastbutton != code) lastbutton = code;
    }

    void GraphicsWindow::PumpButtonCodeUp(const int code)
    {
        if (window_ptr == NULL) return;

        bool actually_up = false;
        if (code < InputSystem::BUTTON_FIRST_KEY)
            actually_up = window_ptr->MouseDown(code);
        else
            actually_up = window_ptr->KeyDown(code);

        buttondownstate[code] = actually_up;
        buttonhitstate[code] = actually_up;
        anybuttonhit = actually_up;
    }

    void GraphicsWindow::PumpMouseWheelDir(const int dir)
    {
        // Reset the mousewheel.
        if (buttondownstate[InputSystem::BUTTON_MOUSE_WHEELDOWN])
        {
            buttondownstate[InputSystem::BUTTON_MOUSE_WHEELDOWN] = false;
            buttonhitstate[InputSystem::BUTTON_MOUSE_WHEELDOWN] = false;
            anybuttonhit = false;
        }

        if (buttondownstate[InputSystem::BUTTON_MOUSE_WHEELUP])
        {
            buttondownstate[InputSystem::BUTTON_MOUSE_WHEELUP] = false;
            buttonhitstate[InputSystem::BUTTON_MOUSE_WHEELUP] = false;
            anybuttonhit = false;
        }

        float fixed_dir = Math::Sgn(static_cast<float>(dir));
        if (static_cast<float>(fixed_dir) != 0)
        {
            if (fixed_dir > 0)
            {
                if (!buttondownstate[InputSystem::BUTTON_MOUSE_WHEELDOWN])
                {
                    buttondownstate[InputSystem::BUTTON_MOUSE_WHEELDOWN] = true;
                    buttonhitstate[InputSystem::BUTTON_MOUSE_WHEELDOWN] = true;
                    anybuttonhit = true;
                    if (lastbutton != InputSystem::BUTTON_MOUSE_WHEELDOWN) lastbutton = InputSystem::BUTTON_MOUSE_WHEELDOWN;
                }
            }
            else if (fixed_dir < 0)
            {
                if (!buttondownstate[InputSystem::BUTTON_MOUSE_WHEELUP])
                {
                    buttondownstate[InputSystem::BUTTON_MOUSE_WHEELUP] = true;
                    buttonhitstate[InputSystem::BUTTON_MOUSE_WHEELUP] = true;
                    anybuttonhit = true;
                    if (lastbutton != InputSystem::BUTTON_MOUSE_WHEELUP) lastbutton = InputSystem::BUTTON_MOUSE_WHEELUP;
                }
            }
        }
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
                window->rawmouseaxis.x += float(raw->data.mouse.lLastX);
                window->rawmouseaxis.y += float(raw->data.mouse.lLastY);
            }
            break;
        }
        case WM_CHAR:
        case WM_SYSCHAR:
            EventQueue::Emit(Event::KeyChar, window, wparam);
            break;

        case WM_MBUTTONDBLCLK:
            window->PumpButtonCodeDown(Mouse::Middle);
            EventQueue::Emit(Event::MouseDown, window, Mouse::Middle);
            EventQueue::Emit(Event::DoubleClick, window, Mouse::Middle);
            break;

        case WM_RBUTTONDBLCLK:
            window->PumpButtonCodeDown(Mouse::Right);
            EventQueue::Emit(Event::MouseDown, window, Mouse::Right);
            EventQueue::Emit(Event::DoubleClick, window, Mouse::Right);
            break;

        case WM_LBUTTONDBLCLK:
            window->PumpButtonCodeDown(Mouse::Left);
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

            window->PumpButtonCodeDown(button);
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
            window->PumpButtonCodeDown(button);
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

            window->PumpButtonCodeUp(button);
            EventQueue::Emit(Event::MouseUp, window, button, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            break;
        }
        case WM_MOUSELEAVE:
            EventQueue::Emit(Event::MouseLeave, window, 0, LOWORD(lparam), HIWORD(lparam));
            break;

        case WM_MOUSEMOVE:
            EventQueue::Emit(Event::MouseMove, window, 0, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            break;

        case WM_MOUSEWHEEL:
            window->PumpMouseWheelDir(-GET_WHEEL_DELTA_WPARAM(wparam) / 120);
            EventQueue::Emit(Event::MouseWheel, window, -GET_WHEEL_DELTA_WPARAM(wparam) / 120, 0, 0, 0, 0);
            break;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            window->PumpButtonCodeDown(wparam);
            EventQueue::Emit(Event::KeyDown, window, wparam);
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            window->PumpButtonCodeUp(wparam);
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

#ifdef IMGUI_API
        ImGui_ImplWin32_WndProcHandler(hwnd, message, wparam, lparam);
#endif
        return Leadwerks::WndProc(hwnd, message, wparam, lparam);
    }
#endif

    bool GraphicsWindow::StartFrame()
    {
        bool ret = false;
        if (imgui) ret = imgui->StartFrame();
        if (framebuffer->GetBlendMode() != Blend::Alpha) framebuffer->SetBlendMode(Blend::Alpha);
        return ret;
    }

    void GraphicsWindow::Sync(const bool sync, const float framerate)
    {
        window_ptr->Update();
        FireCallback(CALLBACK_POSTRENDER, window_ptr, NULL);
        //framebuffer->SetColor(1,1,1,1);
        if (framebuffer->GetBlendMode() != Blend::Solid) framebuffer->SetBlendMode(Blend::Solid);
        if (imgui) imgui->EndFrame();
        UpdateRender(framebuffer, sync, framerate);
    }

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

    Leadwerks::Interface* GraphicsWindow::GetInterface()
    {
        return ui;
    }

    Leadwerks::ImGuiLayer* GraphicsWindow::GetImGUI()
    {
        return imgui;
    }

    GraphicsWindow* GraphicsWindow::Create(const std::string& title, const GraphicWindowSettings& settings, Leadwerks::SplashWindow* splashwindow)
    {
        if (!current)
        {
            current = new GraphicsWindow();
            current->title = title;

            current->splash = splashwindow;
            current->showonready = (bool)current->splash == NULL;

            if (current->Initialize(settings))
            {
                ListenEvent(EVENT_NONE, NULL, GraphicsWindow::EventCallback, current);
            }
            else
            {
                current->Release();
                current = NULL;
            }

            auto BUTTON_LAST = InputSystem::BUTTON_MOUSE_WHEELRIGHT;
            for (int i = 0; i < BUTTON_LAST + 1; i++)
            {
                auto code = (InputSystem::ButtonCode)i;
                current->buttonreleasedstate[code] = true;
            }
        }

        return current;
    }
}