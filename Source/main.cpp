#include "pch.h"

using namespace Leadwerks;

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