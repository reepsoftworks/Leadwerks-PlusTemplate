#pragma once
#include "../LeadwerksPlus.h"

namespace Leadwerks
{
	class ImGuiLayer : public Object
	{
		Context* m_context;
		bool framestarted;
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();
		void SetContext(Context* context);
		bool StartFrame();
		bool EndFrame();

		Context* GetContext();
		static ImGuiLayer* Create(Context* context = NULL);
	};
}