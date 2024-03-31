#include "pch.h"
#include "App.h"

namespace App
{
	using namespace Leadwerks;

	std::vector<Gadget*> gadgets;
	void ReleaseGadgets()
	{
		for (auto p : gadgets)
		{
			p->Release();
			p = NULL;
		}

		gadgets.clear();
	}

	static bool g_hideallgadgets = false;
	void HideGadgets(const bool b)
	{
		g_hideallgadgets = b;
	}

	void Gadget::GadgetCallback(Object* source, Object* extra)
	{
		if (!g_hideallgadgets)
		{
			auto window = GraphicsWindow::GetCurrent();
			if (window)
			{
				if (window->GetImGui()->StartFrame())
				{
					for (const auto& g : gadgets)
					{
						bool b = g->shown;
						if (b)
						{
							g->Draw(&b);
						}
					}
				}
			}
		}
	}

	Gadget::Gadget()
	{
		shown = true;
	}

	Gadget::~Gadget()
	{
		shown = false;
	}

	void Gadget::Draw(bool* p_open)
	{
		ImGui::ShowDemoWindow(p_open);
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