#include "pch.h"
#include "App.h"

namespace App
{
	using namespace Leadwerks;

	std::list<int> renderlayers;
	std::vector<Gadget*> Gadget::gadgets;
	void Gadget::ReleaseGadgets()
	{
		for (auto p : gadgets)
		{
			p->Release();
			p = NULL;
		}

		gadgets.clear();
	}

	static bool g_hideallgadgets = false;
	static bool g_hideUI = false;
	void Gadget::HideGadgets(const bool b)
	{
		g_hideallgadgets = b;
	}

	void Gadget::HideUI(const bool b)
	{
		if (g_hideUI == b) return;
		bool oldstate = g_hideUI;
		g_hideUI = b;

		if (oldstate == true && g_hideUI == false)
		{
			auto window = GraphicsWindow::GetCurrent();
			if (window) window->GetImGUI()->EndFrame();
		}
		
	}

	void Gadget::SetOrder(const int layer)
	{
		renderlayer = layer;

		// Push back a new render layer if one isn't registered!
		auto it = std::find(renderlayers.begin(), renderlayers.end(), renderlayer);
		if (it == renderlayers.end())
		{
			renderlayers.push_back(layer);
			renderlayers.sort();
		}
	}

	int Gadget::GetOrder()
	{
		return renderlayer;
	}

	void Gadget::GadgetCallback(Object* source, Object* extra)
	{
		if (!g_hideallgadgets)
		{
			auto window = CastObject<GraphicsWindow>(source);//GraphicsWindow::GetCurrent();
			if (window)
			{
				if (window->StartFrame())
				{
					for (const auto& gadget : gadgets)
					{
						bool b = gadget->shown;
						if (b)
						{
							for (int n : renderlayers)
							{
								if (gadget->renderlayer == n)
								{
									auto framebuffer = window->GetFramebuffer();
									if (framebuffer)
									{
										if (b) gadget->PostRender(framebuffer);
									}
									if (!g_hideUI) gadget->DrawUI();
								}
							}
						}
					}
				}
			}
		}
	}

	bool Gadget::EventCallback(const Event& e, Leadwerks::Object* extra)
	{
		auto elem = CastObject<Gadget>(extra);
		if (elem == NULL) return false;
		return elem->ProcessEvent(e);
	}

	bool Gadget::ProcessEvent(const Event& e)
	{
		return true;
	}

	Gadget::Gadget()
	{
		shown = true;
		SetOrder(0);
	}

	Gadget::~Gadget()
	{
		shown = false;
	}

	void Gadget::DrawUI()
	{
		ImGui::ShowDemoWindow(&shown);
	}

	void Gadget::PostRender(Leadwerks::Framebuffer* context)
	{
	}

	void Gadget::Show()
	{
		shown = true;
	}

	void Gadget::Hide()
	{
		shown = false;
	}

	bool Gadget::GetHidden()
	{
		return !shown;
	}

	void Gadget::Listen(const int eventid, Leadwerks::Object* source)
	{
		ListenEvent(eventid, source, Gadget::EventCallback, this);
	}
}