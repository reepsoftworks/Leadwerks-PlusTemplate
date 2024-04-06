#pragma once
#include "App.h"

namespace App
{
	class Gadget : public Leadwerks::Object
	{
	protected:
		bool shown;
		int renderlayer;

		static bool EventCallback(const Leadwerks::Event& e, Leadwerks::Object* extra);
		virtual bool ProcessEvent(const Leadwerks::Event& e);
		void Listen(const int eventid, Leadwerks::Object* source);

	public:
		Gadget();
		virtual ~Gadget();
		virtual void DrawUI();
		virtual void PostRender(Leadwerks::Framebuffer* context);

		virtual void Show();
		virtual void Hide();
		bool GetHidden();

		void SetOrder(const int layer);
		int GetOrder();

		static std::vector<Gadget*> gadgets;
		static void ReleaseGadgets();
		static void HideGadgets(const bool b);
		static void HideUI(const bool b);
		static void GadgetCallback(Object* source, Object* extra);
	};

	template <class T>
	T* CreateGadget(GraphicsWindow* window)
	{
		auto c = new T();
		auto gadget = dynamic_cast<T*>(c);
		if (gadget)
		{
			if (Gadget::gadgets.empty())
			{
				Leadwerks::SetCallback(CALLBACK_POSTRENDER, window, Gadget::GadgetCallback);
			}
			Gadget::gadgets.push_back(gadget);
		}
		return gadget;
	};
}