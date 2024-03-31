#pragma once
#include "App.h"

namespace App
{
	class Gadget : public Leadwerks::Object
	{
	protected:
		bool shown;
	public:
		Gadget();
		virtual ~Gadget();
		virtual void Draw(bool* p_open);

		void Show();
		void Hide();
		bool GetHidden();

		static void GadgetCallback(Object* source, Object* extra);
	};

	extern std::vector<Gadget*> gadgets;
	extern void ReleaseGadgets();
	extern void HideGadgets(const bool b);

	template <class T>
	T* CreateGadget(GraphicsWindow* window)
	{
		auto gadget = new T();
		if (gadgets.empty())
		{
			Leadwerks::SetCallback(Leadwerks::CALLBACK_UPDATE, NULL, Gadget::GadgetCallback);
		}
		gadgets.push_back(gadget);
		return gadget;
	};
}