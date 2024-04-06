#include "pch.h"
#include "LeadwerksPlus.h"

#ifdef _WIN32
#include "dwmapi.h"
#include "shellscalingapi.h"
#include <VersionHelpers.h>
#endif

namespace Leadwerks
{
	// Message IDs
	const int OS::MessageOK = 1;
	const int OS::MessageCancel = 2;
	const int OS::MessageAbort = 3;
	const int OS::MessageRetry = 4;
	const int OS::MessageIgnore = 5;
	const int OS::MessageYes = 6;
	const int OS::MessageNo = 7;

	// Themes
	const int OS::ThemeSystem = 8;
	const int OS::ThemeLight = 9;
	const int OS::ThemeDark = 10;

	void OS::CallCMDWindow()
	{
#if defined (_WIN32)
		if (!AttachConsole(ATTACH_PARENT_PROCESS))
		{
			if (AllocConsole())
			{
				freopen("conin$", "r", stdin);
				freopen("conout$", "w", stdout);
				freopen("conout$", "w", stderr);
			}
		}
		else
		{
			auto consoleHandleOut = GetStdHandle(STD_OUTPUT_HANDLE);
			auto consoleHandleIn = GetStdHandle(STD_INPUT_HANDLE);
			auto consoleHandleErr = GetStdHandle(STD_ERROR_HANDLE);
			if (consoleHandleOut != INVALID_HANDLE_VALUE) {
				freopen("conout$", "w", stdout);
				setvbuf(stdout, NULL, _IONBF, 0);
			}
			if (consoleHandleIn != INVALID_HANDLE_VALUE) {
				freopen("conin$", "r", stdin);
				setvbuf(stdin, NULL, _IONBF, 0);
			}
			if (consoleHandleErr != INVALID_HANDLE_VALUE) {
				freopen("conout$", "w", stderr);
				setvbuf(stderr, NULL, _IONBF, 0);
			}
		}
#endif
	}

	std::map<std::string, std::string> OS::ParseCommandLine(int argc, const char* argv[])
	{
		System::ParseCommandLine(argc, argv);
		return System::Arguments;
	}

	int OS::Message(const std::string& title, const std::string& message)
	{
#if defined (_WIN32)
		return (int)::MessageBoxA(NULL, message.c_str(), title.c_str(), MB_OK);
#else
		return 0;
#endif
	}

	int OS::Message(const std::wstring& title, const std::wstring& message)
	{
#if defined (_WIN32)
		return (int)::MessageBoxW(NULL, message.c_str(), title.c_str(), MB_OK);
#else
		return 0;
#endif
	}

	int OS::MessageAlert(const std::string& title, const std::string& message)
	{
#if defined (_WIN32)
		return (int)::MessageBoxA(NULL, message.c_str(), title.c_str(), MB_ICONWARNING | MB_OK);
#else
		return 0;
#endif
	}

	int OS::MessageAlert(const std::wstring& title, const std::wstring& message)
	{
#if defined (_WIN32)
		return (int)::MessageBoxW(NULL, message.c_str(), title.c_str(), MB_ICONWARNING | MB_OK);
#else
		return 0;
#endif
	}

	int OS::MessageError(const std::string& title, const std::string& message)
	{
#if defined (_WIN32)
		return (int)::MessageBoxA(NULL, message.c_str(), title.c_str(), MB_ICONERROR | MB_OK);
#else
		return 0;
#endif
	}

	int OS::MessageError(const std::wstring& title, const std::wstring& message)
	{
#if defined (_WIN32)
		return (int)::MessageBoxW(NULL, message.c_str(), title.c_str(), MB_ICONERROR | MB_OK);
#else
		return 0;
#endif
	}

	int OS::MessageContinue(const std::string& title, const std::string& message)
	{
#if defined (_WIN32)
		int msgboxID = (int)MessageBoxA(
			NULL,
			message.c_str(), title.c_str(),
			MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2
		);
		return msgboxID;
#else
		return 0;
#endif
	}

	int OS::MessageContinue(const std::wstring& title, const std::wstring& message)
	{
#if defined (_WIN32)
		int msgboxID = (int)MessageBoxW(
			NULL,
			title.c_str(), message.c_str(),
			MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2
		);
		return msgboxID;
#else
		return 0;
#endif
	}

	void PrintToIDEDebugger(const std::wstring& message)
	{
#if defined (_WIN32) && (_DEBUG)
		auto msg = message + L"\n";
		if (!msg.empty()) OutputDebugStringW(msg.c_str());
		msg.clear();
#endif
	}

	void PrintWString(const std::wstring& message, PrintColor printcolor)
	{
#if defined (_WIN32)
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// Apply a log tag
		switch (printcolor)
		{
		case PRINT_COLOR_MAGENTA:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;

		case PRINT_COLOR_GREEN:
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;

		case PRINT_COLOR_BLUE:
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;

		case PRINT_COLOR_YELLOW:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;

		case PRINT_COLOR_RED:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			break;

		default:
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			break;
		}

		std::wcout << message << std::endl;

		// Do Windows things...
		PrintToIDEDebugger(message);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
		static const std::wstring ColorRed(L"\033[0;31m");
		static const std::wstring ColorYellow(L"\033[1;33m");
		static const std::wstring ColorGreen(L"\033[1;32m");
		static const std::wstring ColorCyan(L"\033[0;36m");
		static const std::wstring ColorMagenta(L"\033[0;35m");
		static const std::wstring ColorDefault(L"\033[0m");

		switch (printcolor)
		{
		case PRINT_COLOR_MAGENTA:
			std::wcout << ColorMagenta;
			break;

		case PRINT_COLOR_GREEN:
			std::wcout << ColorGreen;
			break;

		case PRINT_COLOR_BLUE:
			std::wcout << ColorCyan;
			break;

		case PRINT_COLOR_YELLOW:
			std::wcout << ColorYellow;
			break;

		case PRINT_COLOR_RED:
			std::wcout << ColorRed;
			break;

		default:
			std::wcout << ColorDefault;
			break;
		}

		std::wcout << message << ColorDefault << std::endl;
#endif
	}

	void OS::Print(int8_t c, PrintColor printcolor)
	{
		PrintWString(std::to_wstring(c), printcolor);
	}

	void OS::Print(int16_t s, PrintColor printcolor)
	{
		PrintWString(std::to_wstring(s), printcolor);
	}

	void OS::Print(int32_t i, PrintColor printcolor)
	{
		PrintWString(std::to_wstring(i), printcolor);
	}

	void OS::Print(int64_t l, PrintColor printcolor)
	{
		PrintWString(std::to_wstring(l), printcolor);
	}

	void OS::Print(uint8_t c, PrintColor printcolor)
	{
		PrintWString(WString(c), printcolor);
	}

	void OS::Print(uint16_t s, PrintColor printcolor)
	{
		PrintWString(std::to_wstring(s), printcolor);
	}

	void OS::Print(uint32_t i, PrintColor printcolor)
	{
		PrintWString(std::to_wstring(i), printcolor);
	}

	void OS::Print(uint64_t l, PrintColor printcolor)
	{
		PrintWString(std::to_wstring(l), printcolor);
	}

	void OS::Print(float f, PrintColor printcolor)
	{
		PrintWString(std::to_wstring(f), printcolor);
	}

	void OS::Print(double d, PrintColor printcolor)
	{
		PrintWString(WString(d), printcolor);
	}

	void OS::Print(const std::string& message, PrintColor printcolor)
	{
		PrintWString(WString(message), printcolor);
	}

	void OS::Print(const std::wstring& message, PrintColor printcolor)
	{
		PrintWString(message, printcolor);
	}

	time_t OS::Millisecs()
	{
#ifdef WIN32
		return static_cast<uint64_t>(win32::timeGetTime());
#else
		return static_cast<uint64_t>(Time::Millisecs());
#endif
	}

	void OS::Sleep(time_t milliseconds)
	{
#ifndef _WIN32
		usleep((int)milliseconds);
#else
		::Sleep((DWORD)milliseconds);
#endif
	}

	std::string OS::GetSystemEnvironment(const std::string& env)
	{
#ifdef _WIN32 
		char* buf = nullptr;
		size_t sz = 0;

		// Use _dupenv_s to get the environment variable
		if (_dupenv_s(&buf, &sz, env.c_str()) == 0 && buf != nullptr)
		{
			std::string result(buf);
			free(buf); // Free the allocated buffer
			return result;
		}
#else
		auto s = std::getenv(env.c_str());
		if (s == NULL) return std::string(s);
#endif
		return ""; // Environment variable not found or error occurred
	}

	std::wstring OS::GetSystemEnvironmentW(const std::string& env)
	{
#ifdef _WIN32 
		wchar_t* buf = nullptr;
		size_t sz = 0;

		std::wstring wenv = WString(env);

		// Use _dupenv_s to get the environment variable
		if (_wdupenv_s(&buf, &sz, wenv.c_str()) == 0 && buf != nullptr)
		{
			std::wstring result(buf);
			free(buf); // Free the allocated buffer
			return result;
		}
#else
		auto s = std::getenv(env.c_str());
		if (s == NULL) return String::ToWString(s);
#endif
		return L""; // Environment variable not found or error occurred
	}

	bool OS::LightThemeEnabled()
	{
#if defined (_WIN32)
		// The value is expected to be a REG_DWORD, which is a signed 32-bit little-endian
		auto buffer = std::vector<char>(4);
		auto cbData = static_cast<DWORD>(buffer.size() * sizeof(char));
		auto res = RegGetValueW(
			HKEY_CURRENT_USER,
			L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
			L"AppsUseLightTheme",
			RRF_RT_REG_DWORD, // expected value type
			nullptr,
			buffer.data(),
			&cbData);

		if (res != ERROR_SUCCESS) {
			throw std::runtime_error("Error: error_code=" + std::to_string(res));
		}

		// convert bytes written to our buffer to an int, assuming little-endian
		auto i = int(buffer[3] << 24 |
			buffer[2] << 16 |
			buffer[1] << 8 |
			buffer[0]);

		return i == 1;
#else
		return false;
#endif
	}

	// Convert UINT to float in the range [0, 1]
	static float ConvertToNormalizedFloat(UINT value, UINT maxValue)
	{
		// Ensure maxValue is not zero to avoid division by zero
		if (maxValue == 0) {
			return 0.0f;
		}

		// Normalize the value to [0, 1]
		float normalizedValue = static_cast<float>(value) / static_cast<float>(maxValue);

		// Scale to the desired range (e.g., [0, 1])
		return normalizedValue;
	}

	iVec2 OS::GetDisplaySize()
	{
		return System::GetGraphicsMode(System::CountGraphicsModes() - 1);
	}

	float OS::GetDisplayScale()
	{
		float scale = System::DPIScaling;
		HMONITOR hMonitor = MonitorFromPoint({ 0, 0 }, MONITOR_DEFAULTTOPRIMARY);
		UINT dpiX, dpiY;
		HRESULT hr = GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
		if (SUCCEEDED(hr)) 
		{
			scale = ConvertToNormalizedFloat(dpiX, 96);
		}
		return scale;
		//return System::DPIScaling;
	}

	std::vector<iVec2> OS::GetDisplayModes()
	{
		System::CountGraphicsModes();
		return System::graphicsmodes;
	}

	static std::vector<std::string> resolution_strings;
	std::vector<std::string> OS::GetDisplayOptions()
	{
		auto gfx = OS::GetDisplayModes();
		for (const auto& p : gfx)
		{
			std::string n = String(p.x) + "x" + String(p.y);
			resolution_strings.push_back(n);
		}

		return resolution_strings;
	}

	int OS::FindDisplayOption(const std::string& SizeXxSizeY)
	{
		int ret = 0;
		if (resolution_strings.empty()) resolution_strings = OS::GetDisplayOptions();
		for (std::size_t i = 0; i < resolution_strings.size(); i ++)
		{
			auto t = resolution_strings[i];
			auto test_vec2 = String::Split(SizeXxSizeY, "x");
			auto vec2 = String::Split(t, "x");
			if (vec2[0] == test_vec2[0] && vec2[1] == test_vec2[1])
			{
				ret = (int)i;
				break;
			}
		}

		return ret;
	}

	std::string OS::GetDisplayString(const int i)
	{
		if (resolution_strings.empty()) resolution_strings = OS::GetDisplayOptions();
		return resolution_strings[i];
	}

	bool OS::SetWindowTitlebarTheme(Leadwerks::Window* source, const int theme)
	{
#if defined (_WIN32)
		BOOL darkmode = false;
		if (theme == OS::ThemeDark) darkmode = true;
		else if (theme == OS::ThemeSystem) darkmode = !LightThemeEnabled();

		BOOL SET_IMMERSIVE_DARK_MODE_SUCCESS = SUCCEEDED(DwmSetWindowAttribute(
			source->GetHandle(), DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE, // 20
			&darkmode, sizeof(darkmode)));

		return SET_IMMERSIVE_DARK_MODE_SUCCESS == darkmode;
#else
		return false;
#endif
	}

	bool OS::SetWindowIcon(Leadwerks::Window* source, const int id)
	{
		if (!source)
			return false;

#if defined (_WIN32)
		// Get device context
		HWND hwnd = source->GetHandle();
		HDC hdc = GetDC(hwnd);

		if (hdc == NULL) return false;

		// Load the icon for window titlebar and taskbar
		HICON icon = LoadIconA(GetModuleHandle(NULL), (LPCSTR)id);
		SendMessage(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon));
#endif
		return true;
	}

	std::string OS::GetPlatformName()
	{
		std::string platform = "Unknown";
#ifdef _WIN32 
		platform = "Windows";
		if (IsWindows10OrGreater()) 
		{
			platform = platform + " 10+";
		}
		if (IsWindows8Point1OrGreater()) 
		{
			platform = platform + " 8.1";
		}
		else if (IsWindows8OrGreater()) 
		{
			platform = platform + " 8";
		}
		else if (IsWindows7OrGreater()) 
		{
			platform = platform + " 7";
		}
		else if (IsWindowsVistaOrGreater()) 
		{
			platform = platform + " Vista";
		}
		else if (IsWindowsXPOrGreater()) 
		{
			platform = platform + " XP";
		}

#elif __APPLE__
		platform = "Apple";
#elif __linux__
		platform = "Linux";
#endif
		return platform;
	}
}