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
		g_hideUI = b;
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
										gadget->PostRender(framebuffer);
									}

									if (!g_hideUI) gadget->DrawUI(&b);
								}
							}
						}
					}
				}
			}
		}
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

	void Gadget::DrawUI(bool* open)
	{
		ImGui::ShowDemoWindow(open);
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
}