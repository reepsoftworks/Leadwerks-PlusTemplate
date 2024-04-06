#include "pch.h"
#include "../App.h"

namespace App
{
    using namespace Leadwerks;

    ConsoleGadget::ConsoleGadget()
    {
        shown = true;
        memset(InputBuf, 0, sizeof(InputBuf));
        historypos = -1;
        scrolltobottom = true;
        autoscroll = true;

        window_opened = false;
    }

    ConsoleGadget::~ConsoleGadget()
    {
        ClearLog();
        for (int i = 0; i < history.Size; i++)
            ImGui::MemFree(history[i]);
    }

    void ConsoleGadget::ClearLog()
    {
        for (int i = 0; i < items.Size; i++)
            ImGui::MemFree(items[i]);
        items.clear();
    }


    static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
    {
        ConsoleGadget* console = (ConsoleGadget*)data->UserData;
        return console->TextEditCallback(data);
    }

    static int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
    static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
    static char* Strdup(const char* s) { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = ImGui::MemAlloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
    static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

    int ConsoleGadget::TextEditCallback(ImGuiInputTextCallbackData* data)
    {
        //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
        switch (data->EventFlag)
        {
        case ImGuiInputTextFlags_CallbackCompletion:
        {
            // Example of TEXT COMPLETION

            // Locate beginning of current word
            const char* word_end = data->Buf + data->CursorPos;
            const char* word_start = word_end;
            while (word_start > data->Buf)
            {
                const char c = word_start[-1];
                if (c == ' ' || c == '\t' || c == ',' || c == ';')
                    break;
                word_start--;
            }

            // Build a list of candidates
            ImVector<const char*> candidates;
            for (const auto& command : *ConCommand::GetCommands())
            {
                auto cmd = command.first;
                if (Strnicmp(cmd.c_str(), word_start, (int)(word_end - word_start)) == 0)
                {
                    candidates.push_back(cmd.c_str());
                }
            }

            if (candidates.Size == 0)
            {
                // No match
                AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
            }
            else if (candidates.Size == 1)
            {
                // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                data->InsertChars(data->CursorPos, candidates[0]);
                data->InsertChars(data->CursorPos, " ");
            }
            else
            {
                // Multiple matches. Complete as much as we can..
                // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
                int match_len = (int)(word_end - word_start);
                for (;;)
                {
                    int c = 0;
                    bool all_candidates_matches = true;
                    for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                        if (i == 0)
                            c = toupper(candidates[i][match_len]);
                        else if (c == 0 || c != toupper(candidates[i][match_len]))
                            all_candidates_matches = false;
                    if (!all_candidates_matches)
                        break;
                    match_len++;
                }

                if (match_len > 0)
                {
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                    data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                }

                // List matches
                AddLog("Possible matches:\n");
                for (int i = 0; i < candidates.Size; i++)
                    AddLog("- %s\n", candidates[i]);
            }

            break;
        }
        case ImGuiInputTextFlags_CallbackHistory:
        {
            // Example of HISTORY
            const int prev_history_pos = historypos;
            if (data->EventKey == ImGuiKey_UpArrow)
            {
                if (historypos == -1)
                    historypos = history.Size - 1;
                else if (historypos > 0)
                    historypos--;
            }
            else if (data->EventKey == ImGuiKey_DownArrow)
            {
                if (historypos != -1)
                    if (++historypos >= history.Size)
                        historypos = -1;
            }

            // A better implementation would preserve the data on the current input line along with cursor position.
            if (prev_history_pos != historypos)
            {
                const char* history_str = (historypos >= 0) ? history[historypos] : "";
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, history_str);
            }
        }
        }
        return 0;
    }

    void ConsoleGadget::AddLog(const char* fmt, ...) //IM_FMTARGS(2)
    {
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf) - 1] = 0;
        va_end(args);
        items.push_back(Strdup(buf));
    }

    void ConsoleGadget::Hide()
    {
        shown = false;
        if (window_opened) EmitEvent(Event::WidgetClose, this);
        window_opened = false;
    }

    void ConsoleGadget::ExecCommand(const char* command_line)
    {
        AddLog("] %s\n", command_line);

        // Insert into history. First find match and delete it so it can be pushed to the back.
        // This isn't trying to be smart or optimal.
        historypos = -1;
        for (int i = history.Size - 1; i >= 0; i--)
            if (Stricmp(history[i], command_line) == 0)
            {
                ImGui::MemFree(history[i]);
                history.erase(history.begin() + i);
                break;
            }
        history.push_back(Strdup(command_line));

        auto str = std::string(command_line);
        if (str == "clear" || str == "cls") ClearLog();
        else ExecuteCommand(std::string(command_line));
        scrolltobottom = true;
    }

    void ConsoleGadget::DrawUI()
    {
        static long size = 0;
        if (logstream__ != NULL)
        {
            if (size != logstream__->GetSize())
            {
                int oldpos = logstream__->GetPos();
                logstream__->Seek(size);
                while (!logstream__->EOF())
                {
                    AddLog(logstream__->ReadLine().c_str());
                }
                logstream__->Seek(oldpos);
                size = logstream__->GetSize();
            }
        }

        // Info corner
        static int location = 1;
        if (location >= 0)
        {
            const float PAD = 0.0f;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;

            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        }

        if (ImGui::Begin("InfoGadget", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize
            | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground))
        {
            string build = " (Build " + String(VERSION_BUILD) + ")";
            string engine = "Leadwerks Game Engine " + String(System::GetVersion()) + build;
            ImGui::Text(engine.c_str());
        }
        ImGui::End();

        if (location >= 0)
        {
            const float PAD = 25.0f;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;

            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Appearing, window_pos_pivot);
            ImGui::SetNextWindowSize(ImVec2(work_size.x / 2 , work_size.y / 1.25f), ImGuiCond_Appearing);
        }

        if (!ImGui::Begin("Console", &shown, ImGuiWindowFlags_NoCollapse))
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
            ImGui::End();
            return;
        }


        // Reserve enough left-over height for 1 separator + 1 input text
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_HorizontalScrollbar))
        {
            for (const char* item : items)
            {
                // Normally you would store more information in your item than just a string.
                // (e.g. make Items[] an array of structure, store color/type etc.)
                ImVec4 color;
                bool has_color = false;
                if (strstr(item, "Error:")) { color = ImVec4(0.85f, 0.25f, 0.25f, 1.0f); has_color = true; }
                else if (strstr(item, "Warning:")) { color = ImVec4(1.0f, 0.56f, 0.0f, 1.0f); has_color = true; }
                else if (strncmp(item, "] ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
                if (has_color)
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(item);
                if (has_color)
                    ImGui::PopStyleColor();
            }

            if (scrolltobottom || (autoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                ImGui::SetScrollHereY(1.0f);
            scrolltobottom = false;
        }
        ImGui::EndChild();

        // Textfield
        float currposx = ImGui::GetCursorPosX();
        ImVec2 buttonSize(80, 0);
        float widthNeeded = buttonSize.x + ImGui::GetStyle().ItemSpacing.x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
        bool send = ImGui::Button("Send", buttonSize);
        ImGui::SameLine();
        ImGui::SetCursorPosX(currposx);
        bool reclaim_focus = false;
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory;

        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x - widthNeeded);
        if (ImGui::InputText(" ", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub, (void*)this) || send)
        {
            char* s = InputBuf;
            Strtrim(s);
            if (s[0])
                ExecCommand(s);
            strcpy(s, "");
            reclaim_focus = true;
        }
        ImGui::PopItemWidth();

        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

		ImGui::End();
    }
}