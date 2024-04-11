#pragma once
#include "../App.h"

namespace App
{
	class QuitConfirmGadget : public Gadget
	{
		bool window_opened;
	public:
		QuitConfirmGadget();
		virtual void DrawUI();
	};
}