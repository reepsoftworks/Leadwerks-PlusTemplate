#include "pch.h"
#include "../App.h"

namespace App
{
	using namespace Leadwerks;

	MapListGadget::MapListGadget()
	{
		window_opened = false;

		auto stream = FileSystem::ReadFile("maplist.txt");
		if (stream)
		{
			stream->Seek(0);
			while (!stream->EOF())
			{
				auto ln = stream->ReadLine();
				ln = FileSystem::StripAll(ln);
				list.push_back(ln);
			}
		}
	}

	void MapListGadget::DrawUI()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize;
		ImVec2 windowsize = ImVec2(350, 100);
		ImGui::SetNextWindowSize(windowsize, ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->GetCenter().x - (windowsize.x / 2), ImGui::GetMainViewport()->GetCenter().y - (windowsize.y / 2)), ImGuiCond_Appearing);

		if (!ImGui::Begin("Load Map", &shown, window_flags))
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

		static int item_current_mode = 0;
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		if (ImGui::BeginChild("MapListRegion", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_NoSavedSettings))
		{
			if (!list.empty())
			{
				std::string combo_preview_modevalue = list[item_current_mode];
				if (ImGui::BeginCombo("Map Selection", combo_preview_modevalue.c_str()))
				{
					for (std::size_t n = 0; n < list.size(); n++)
					{
						const bool is_selected = (item_current_mode == n);
						if (ImGui::Selectable(list[n].c_str(), is_selected))
							item_current_mode = n;

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}

			ImGui::EndChild();
		}
		
		ImVec2 buttonSize(80, 0);
		float widthNeeded = buttonSize.x + ImGui::GetStyle().ItemSpacing.x;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
		bool apply = ImGui::Button("Play", buttonSize);
		if (apply)
		{
			ExecuteCommand("map " + list[item_current_mode]);
		}

		ImGui::End();
	}
}
