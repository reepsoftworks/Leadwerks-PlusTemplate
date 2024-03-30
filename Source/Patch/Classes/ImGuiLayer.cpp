#pragma once
#include "pch.h"
#include "../LeadwerksPlus.h"

namespace Leadwerks
{
	ImGuiLayer::ImGuiLayer()
	{
		m_context = NULL;
		framestarted = false;
	}

	ImGuiLayer::~ImGuiLayer()
	{
#ifdef IMGUI_API
		framestarted = false;
		ImGui::DestroyContext();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
#endif
		m_context = NULL;
	}

	void ImGuiLayer::SetContext(Context* context)
	{
		if (m_context == NULL && context == NULL) return;
		if (m_context != context)
		{
			if (context != NULL)
			{
				auto win = context->window;
				ImGui::SetCurrentContext(ImGui::CreateContext());
				ImGui_ImplWin32_Init(win->GetHandle());
				ImGui_ImplOpenGL3_Init("#version 400");
			}
			else
			{
				framestarted = false;
				ImGui::DestroyContext();
			}

			m_context = context;
		}
	}

	bool ImGuiLayer::StartFrame()
	{
		if (!m_context) return false;
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		framestarted = true;
		return true;
	}

	bool ImGuiLayer::EndFrame()
	{
		if (!m_context) return false;
		if (!framestarted) return false;
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		return true;
	}

	Context* ImGuiLayer::GetContext()
	{
		return m_context;
	}

	ImGuiLayer* ImGuiLayer::Create(Context* context)
	{
		auto imguilayer = new ImGuiLayer();
		imguilayer->SetContext(context);
		return imguilayer;
	}
}