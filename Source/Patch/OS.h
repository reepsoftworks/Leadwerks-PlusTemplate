#pragma once
#include "LeadwerksPlus.h"

namespace Leadwerks
{
	class Display;

	class OS
	{
	public:
		static void CallCMDWindow();
		static std::map<std::string, std::string> ParseCommandLine(int argc, const char* argv[]);
		static int Message(const std::string& title, const std::string& message);
		static int Message(const std::wstring& title, const std::wstring& message);
		static int MessageAlert(const std::string& title, const std::string& message);
		static int MessageAlert(const std::wstring& title, const std::wstring& message);
		static int MessageError(const std::string& title, const std::string& message);
		static int MessageError(const std::wstring& title, const std::wstring& message);
		static int MessageContinue(const std::string& title, const std::string& message);
		static int MessageContinue(const std::wstring& title, const std::wstring& message);
		static const int MessageOK;
		static const int MessageCancel;
		static const int MessageAbort;
		static const int MessageRetry;
		static const int MessageIgnore;
		static const int MessageYes;
		static const int MessageNo;
		static const int ThemeSystem;
		static const int ThemeLight;
		static const int ThemeDark;
		static void Print(int8_t c, PrintColor printcolor = PRINT_COLOR_NORM);
		static void Print(int16_t s, PrintColor printcolor = PRINT_COLOR_NORM);
		static void Print(int32_t i, PrintColor printcolor = PRINT_COLOR_NORM);
		static void Print(int64_t l, PrintColor printcolor = PRINT_COLOR_NORM);
		static void Print(uint8_t c, PrintColor printcolor = PRINT_COLOR_NORM);
		static void Print(uint16_t s, PrintColor printcolor = PRINT_COLOR_NORM);
		static void Print(uint32_t i, PrintColor printcolor = PRINT_COLOR_NORM);
		static void Print(uint64_t l, PrintColor printcolor = PRINT_COLOR_NORM);
		static void Print(float f, PrintColor printcolor = PRINT_COLOR_NORM);
		static void Print(double d, PrintColor printcolor = PRINT_COLOR_NORM);
		static void Print(const std::string& message, PrintColor printcolor = PRINT_COLOR_NORM);
		static void Print(const std::wstring& message, PrintColor printcolor = PRINT_COLOR_NORM);
		static time_t Millisecs();
		static void Sleep(time_t milliseconds);
		static std::string GetSystemEnvironment(const std::string& env);
		static std::wstring GetSystemEnvironmentW(const std::string& env);
		static bool LightThemeEnabled();
		static iVec2 GetDisplaySize();
		static float GetDisplayScale();
		static std::vector<iVec2> GetDisplayModes();
		static std::vector<std::string> GetDisplayOptions();
		static std::string GetDisplayString(const int i);
		static int FindDisplayOption(const std::string& SizeXxSizeY);
		static bool SetWindowTitlebarTheme(Leadwerks::Window* source, const int theme);
		static bool SetWindowIcon(Leadwerks::Window* source, const int id = 101);

		static std::string GetPlatformName();
	};
}