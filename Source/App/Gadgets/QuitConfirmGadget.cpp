#include "pch.h"
#include "../App.h"

namespace App
{
	using namespace Leadwerks;

	QuitConfirmGadget::QuitConfirmGadget()
	{
		window_opened = false;
	}

	void QuitConfirmGadget::DrawUI()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize;
		ImVec2 windowsize = ImVec2(250, 100);
		ImGui::SetNextWindowSize(windowsize, ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->GetCenter().x - (windowsize.x / 2), ImGui::GetMainViewport()->GetCenter().y - (windowsize.y / 2)), ImGuiCond_Appearing);

		if (!ImGui::Begin("Quit Game", &shown, window_flags))
		{
			ImGui::End();
			return;
		}
		else
		{
			if (window_opened == false && shown)
			{
				EmitEvent(Event::WidgetOpen, this);
				window_opened = true;
			}
		}

		// Handle closing
		if (!shown)
		{
			Hide();
			if (window_opened) EmitEvent(Event::WidgetClose, this);
			window_opened = false;
			ImGui::End();
			return;
		}

		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		if (ImGui::BeginChild("QuitConfirmRegion", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::Text("Are you sure you want to quit?");
			ImGui::EndChild();
		}
		
		ImVec2 buttonSize(80, 0);
		float widthNeeded = buttonSize.x + ImGui::GetStyle().ItemSpacing.x;
		ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x / 2) - widthNeeded);
		bool apply = ImGui::Button("Yes", buttonSize);
		if (apply)
		{
			EmitEvent(Event::WidgetAction, this, 0);
		}

		ImGui::SameLine();
		//ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x / 2) + buttonSize.x + 8);
		bool cancel = ImGui::Button("No", buttonSize);
		if (cancel)
		{
			Hide();
			if (window_opened) EmitEvent(Event::WidgetClose, this);
			window_opened = false;
		}

		ImGui::End();
	}
}
