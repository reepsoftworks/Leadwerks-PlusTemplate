#pragma once
#include "../App.h"

namespace App
{
	class ConsoleGadget : public Gadget
	{
		char	InputBuf[256];
		ImVector<char*>	items;
		ImVector<const char*> commands;
		ImVector<char*>	history;
		int	historypos;
		bool scrolltobottom;
		bool autoscroll;

		void ClearLog();
		void ExecCommand(const char* command_line);
		bool window_opened;
	public:
		ConsoleGadget();
		~ConsoleGadget();

		virtual void Hide();

		void AddLog(const char* fmt, ...);
		virtual void DrawUI();
		int TextEditCallback(ImGuiInputTextCallbackData* data);
	};
}