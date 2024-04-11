#include "pch.h"
#include "SampleApp.h"

using namespace Leadwerks;
using namespace App;

//#define TEST

#ifndef TEST
int main(int argc, const char* argv[])
{
    // Parse Arguments
    Program::ParseArguments(argc, argv);

    // Start application
    auto app = new SampleApp();
    if (!app->Start()) return 1;

    // Game Loop
    static bool running = true;
    while (running) running = app->Update();

    return 0;
}
#else
#include "App/App.h"

int main(int argc, const char* argv[])
{
    Leadwerks::Window* window = Leadwerks::Window::Create("Leadwerks", 0, 0, 1280, 720, Window::Titlebar | Window::Center);
    Leadwerks::Context* context = Leadwerks::Context::Create(window);

    auto world = World::Create();
    auto camera = Camera::Create();
    camera->SetClearColor(0.125f);
    camera->SetPosition(0, 0, -3);
    Settings::Apply(camera);

    auto light = DirectionalLight::Create();
    light->Turn(45, 35, 0);

    auto model = Model::Box();

    Material* material = AssetLoader::LoadMaterial("Materials/Developer/grid02.mat");
    model->SetMaterial(material);

    // Test Material values....
    MaterialValue myvalue;
    myvalue.name = "surfprop";
    myvalue.size = 4;
    material->values["surfprop"] = myvalue;
    for (auto& p : material->values)
    {
        auto key = p.first;
        auto value = p.second;

        Print(key + " = " + value.name);
    }
    material->Release();

    // Test getting the ambient shader.
    auto shader = Shader::Load("Shaders/Lighting/ambientlight.shader");
    if (shader)
    {
        shader->SetInt("simpleshading", 1);
    }

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

        model->Turn(0, 1.0f * Time::GetSpeed(), 0);
        UpdateTime();
        world->Update();
        world->Render();
        UpdateRender(context);
    }

    return true;
}

#endif

#if 0
int main(int argc, const char* argv[])
{
    Program::ParseArguments(argc, argv);

    if (!Program::VendorCheck())
        return 1;

    GraphicWindowSettings windowsettings;
    auto window = GraphicsWindow::Create("Leadwerks", windowsettings, SplashWindow::Create(L"splashscreen.bmp"));
    if (!window) return 1;

    //auto ui = window->GetInterface();
    //auto basepanel = Widget::Panel(0, 0, 200, 200, ui->GetBase());
    //basepanel->SetScript("Scripts/GUI/Panel.lua");
    //basepanel->CallFunction("SetColor", 0.0f, 0.0f, 0.0f, 1.0f, 0);

    //auto menu = CreateGameMenu<GameMenu>(window);

    auto stats = CreateGadget<StatsGadget>(window);
    auto console = CreateGadget<ConsoleGadget>(window);
    console->Hide();

    //auto texturegadget = CreateGadget<TextureGadget>(window);
    //auto tex = AssetLoader::LoadTexture("Materials/Developer/Textures/grid01.png");
    //texturegadget->SetImage(tex);
    //texturegadget->SetSize(iVec2(512,512));
    //texturegadget->SetOrder(1);
    //auto gadget = CreateGadget<Gadget>(window);

    // World
    //auto world = World::Create();
    //auto camera = Camera::Create();
    //camera->SetClearColor(0.125f);
    //camera->SetPosition(0, 0, -3);
    //Settings::Apply(camera);
    //auto light = DirectionalLight::Create();
    //light->Turn(45, 35, 0);
    //auto model = Model::Box();

    //auto timer = Timer::Create(2000);
    //ListenEvent(Event::TimerTick, timer, EventCallback);

    auto scene = Scene::Create();

    bool running = true;
    while (running)
    {
        while (PeekEvent())
        {
            const auto e = WaitEvent();
            if (e.id == Event::Quit)
            {
                Print("Window Close");
                running = false;
                break;
            }
            else if (e.id == EVENT_STARTRENDERER)
            {
            }
            else if (e.id == Event::KeyDown)
            {
                static bool fullscreen = false;
                if (e.data == Key::F11)
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
                else if (e.data == Key::F1)
                {
                    bool h = console->GetHidden();
                    if (!h)
                        console->Hide();
                    else
                        console->Show();
                }
            }
        }


        // World
        // model->Turn(0, 1.0f * Time::GetSpeed(), 0);
        //UpdateTime();
        //world->Update();
        //world->Render();

        scene->Update();
        window->Sync();
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

    // Try to load a bad material!
    //model->SetMaterial(Material::Load("Materials/idonotexist.mat"));

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

        model->Turn(0, 1.0f * Time::GetSpeed(), 0);
        UpdateTime();
        world->Update();
        world->Render();
        UpdateRender(currContext);
    }
    return 0;
}
#endif