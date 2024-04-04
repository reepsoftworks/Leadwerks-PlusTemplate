#include "pch.h"
#include "../App.h"

namespace App
{
	using namespace Leadwerks;

	void StatsGadget::DrawUI()
	{
		static int location = 2;
		ImGuiIO& io = ImGui::GetIO();
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize 
			| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

        if (location >= 0)
        {
            const float PAD = 10.0f;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            window_flags |= ImGuiWindowFlags_NoMove;
        }
        else if (location == -2)
        {
            // Center window
            ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            window_flags |= ImGuiWindowFlags_NoMove;
        }
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("StatsPanel", 0, window_flags))
        {
            //IMGUI_DEMO_MARKER("Examples/Simple Overlay");
            ImGui::Text("Stats");
            ImGui::Separator();
 
            auto world = World::GetCurrent();
            if (world)
            {
                float framerate = Time::UPS();
                string str = std::string("FPS: " + String(framerate));
                if (framerate < 30)
                    ImGui::TextColored(ImVec4(0.85f, 0.25f, 0.25f, 1.0f), str.c_str());
                else if (framerate < 55 && framerate > 30)
                    ImGui::TextColored(ImVec4(1.0f, 0.56f, 0.0f, 1.0f), str.c_str());
                else
                    ImGui::TextColored(ImVec4(0.07f, 0.55f, 0.0f, 1.0f), str.c_str());

                str = std::string("Update Time: " + String(world->stats_updatetime));
                ImGui::Text(str.c_str());

                str = std::string("Render Time: " + String(world->stats_rendertime));
                ImGui::Text(str.c_str());

                str = std::string("Poly Count: " + String(world->stats_polysdrawn));
                ImGui::Text(str.c_str());

                str = std::string("Batches Drawn: " + String(world->stats_batchesdrawn));
                ImGui::Text(str.c_str());

                str = std::string("Draw Calls: " + String(world->stats_drawcalls));
                ImGui::Text(str.c_str());

                str = std::string("Shadow Polygons: " + String(world->stats_shadowpolysdrawn));
                ImGui::Text(str.c_str());

                str = std::string("Lights Drawn: " + String(world->stats_lightsdrawn));
                ImGui::Text(str.c_str());

                // Add more later....
            }

            // Allow the end user to put this whereever they want.
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Custom", NULL, location == -1)) location = -1;
                if (ImGui::MenuItem("Center", NULL, location == -2)) location = -2;
                if (ImGui::MenuItem("Top-left", NULL, location == 0)) location = 0;
                if (ImGui::MenuItem("Top-right", NULL, location == 1)) location = 1;
                if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
                if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
                ImGui::EndPopup();
            }
        }
        ImGui::End();
	}
}