#pragma once
#include "../App.h"

namespace App
{
	class MapListGadget : public Gadget
	{
		bool window_opened;
		std::vector<std::string> list;

	public:
		MapListGadget();
		virtual void DrawUI();
	};
}