#pragma once
#include "../App.h"

namespace App
{
	class StatsGadget : public Gadget
	{
		int mode;
	public:
		void SetMode(const int statmode);
		virtual void DrawUI();
	};
}