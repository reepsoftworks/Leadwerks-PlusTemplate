#include "pch.h"

using namespace Leadwerks;

bool EventCallback(const Event& e, Object* extra)
{
    if (e.id == Event::WindowMove)
    {
        Print("Event::WindowMove");
    }

    return true;
}

int main(int argc, const char argv[])
{
    Leadwerks::Window* mainWindow = Leadwerks::Window::Create("Leadwerks", 0, 0, 1280, 720, Window::Titlebar | Window::Center);
    OS::SetWindowTitlebarTheme(mainWindow, OS::ThemeDark);
    OS::SetWindowIcon(mainWindow);
    Leadwerks::Context* currContext = Leadwerks::Context::Create(mainWindow);
    Leadwerks::World* world = Leadwerks::World::Create();
    Leadwerks::Camera* camera = Leadwerks::Camera::Create();

    ListenEvent(Event::WindowMove, mainWindow, EventCallback);

    bool running = true;
    while (running)
    {
        while (PeekEvent())
        {
            const auto e = WaitEvent();
            //Print(e.id);
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

        if (mainWindow->KeyHit(Key::Escape)) mainWindow->Close();

        UpdateTime();
        world->Update();
        world->Render();
        UpdateRender(currContext);
    }
    return 0;
}