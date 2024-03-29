#include "pch.h"
#include "App/App.h"

using namespace Leadwerks;
using namespace App;
bool EventCallback(const Event& e, Object* extra)
{
    if (e.id == Event::WindowMove)
    {
        Print("Event::WindowMove");
    }
    else if (e.id == Event::TimerTick)
    {
        Print("Tick");
    }

    return true;
}

int main(int argc, const char argv[])
{
    GraphicWindowSettings windowsettings;
    auto window = GraphicsWindow::Create("Leadwerks", windowsettings, false);
    window->splash = SplashWindow::Create(L"splashscreen.bmp");

    // World
    auto world = World::Create();
    auto camera = Camera::Create();
    camera->SetClearColor(0.125f);
    camera->SetPosition(0, 0, -3);
    Settings::Apply(camera);
    auto light = DirectionalLight::Create();
    light->Turn(45, 35, 0);
    auto model = Model::Box();

    //auto timer = Timer::Create(2000);
    //ListenEvent(Event::TimerTick, timer, EventCallback);

    bool running = true;
    while (running)
    {
        while (PeekEvent())
        {
            const auto e = WaitEvent();
            if (e.id == Event::WindowClose)
            {
                Print("Window Close");
                running = false;
                break;
            }
            else if (e.id == EVENT_STARTRENDERER)
            {
                Print("EVENT_STARTRENDERER");
            }
            else if (e.id == Event::KeyDown)
            {
                static bool fullscreen = false;
                if (e.data == Key::Space)
                {
                    GraphicWindowSettings resize;

                    if (!fullscreen)
                    {
                        resize.style = GRAPHICSWINDOW_FULLSCREENNATIVE;
                        fullscreen = true;
                    }
                    else
                    {
                        resize.style = GRAPHICSWINDOW_TITLEBAR;
                        fullscreen = false;
                    }

                    window->Resize(resize);
                }
            }
        }

        //if (window->KeyHit(Key::Escape)) window->Close();

        model->Turn(0, 1.0f * Time::GetSpeed(), 0);

        // World
        UpdateTime();
        world->Update();
        world->Render();
        UpdateRender(window->GetFramebuffer());
    }

    return true;
}

#if 0
int main(int argc, const char argv[])
{
    for (auto& gfx : OS::GetDisplayModes())
    {
        Print(gfx.ToString());
    }

    auto splash = App::SplashWindow::Create(L"splashscreen.bmp");

    Leadwerks::Window* window = Leadwerks::Window::Create("Leadwerks", 0, 0, 1280, 720, Window::Titlebar | Window::Center);
    OS::SetWindowTitlebarTheme(window, OS::ThemeDark);
    OS::SetWindowIcon(window);

    Leadwerks::Context* currContext = Leadwerks::Context::Create(window);
    auto gui = Leadwerks::GUI::Create(currContext);

    auto world = World::Create();
    auto camera = Camera::Create();
    camera->SetClearColor(0.125f);
    camera->SetPosition(0, 0, -3);
    Settings::Apply(camera);

    auto light = DirectionalLight::Create();
    light->Turn(45, 35, 0);

    auto model = Model::Box();

    auto timer = Timer::Create(2000);
    ListenEvent(Event::TimerTick, timer, EventCallback);

    bool running = true;
    while (running)
    {
        while (PeekEvent())
        {
            const auto e = WaitEvent();
            if (e.id == Event::WindowClose)
            {
                Print("Window Close");
                running = false;
                break;
            }
            else if (e.id == EVENT_STARTRENDERER)
            {
                Print("EVENT_STARTRENDERER");
            }
        }

        if (window->KeyHit(Key::Escape)) window->Close();

        static bool fullscreen = false;
        if (window->KeyHit(Key::Space))
        {
            currContext->Release();
            window->Release();

            if (!fullscreen)
            {
                window = Leadwerks::Window::Create("Leadwerks", 0, 0, 1920, 1080, Window::FullScreen);
                currContext = Leadwerks::Context::Create(window);
            }
            else
            {
                window = Leadwerks::Window::Create("Leadwerks", 0, 0, 1280, 720, Window::Titlebar | Window::Center);
                currContext = Leadwerks::Context::Create(window);
            }

            gui->window = window;
            gui->context = currContext;

            fullscreen = !fullscreen;
        }

        UpdateTime();
        world->Update();
        world->Render();
        UpdateRender(currContext);
    }
    return 0;
}
#endif