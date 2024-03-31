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
	public:
		ConsoleGadget();
		~ConsoleGadget();

		void ExecCommand(const char* command_line);
		void AddLog(const char* fmt, ...);
		virtual void Draw(bool* p_open);
		int TextEditCallback(ImGuiInputTextCallbackData* data);
	};
}