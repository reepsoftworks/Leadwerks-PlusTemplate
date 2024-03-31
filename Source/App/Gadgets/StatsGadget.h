#pragma once
#include "../App.h"

namespace App
{
	class StatsGadget : public Gadget
	{
	public:
		virtual void Draw(bool* p_open);
	};
}