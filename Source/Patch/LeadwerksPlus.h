#pragma once

#ifdef _WIN32
#pragma comment (lib, "shcore.lib")
#pragma comment (lib, "Dwmapi.lib")
#endif

#include <queue> // std:queue
#include <iomanip> // std::setw
#include <fstream>

// Default Engine
#include "Leadwerks.h"

#define EOF -1
#include "Libraries/nlohmann_json/single_include/nlohmann/json.hpp"
#undef EOF

#include "Libraries/tableplusplus/tableplusplus.h"
using namespace tableplusplus;

// imgui
#include "Libraries/imgui/include/imconfig.h"
#include "Libraries/imgui/include/imgui.h"
#include "Libraries/imgui/include/imgui_impl_opengl3.h"
//#include "Libraries/imgui/include/imgui_impl_opengl3_loader.h"
#include "Libraries/imgui/include/imgui_impl_win32.h"

namespace Leadwerks
{
	class SmartObject;
	typedef GUI Interface;
	typedef Source Speaker;
	typedef Context Framebuffer;

	extern void Print(const std::wstring& s);

	extern std::wstring IntToWString(int i);
	extern std::wstring WString(double f);
	extern std::wstring WString(int i);
	extern std::wstring WString(unsigned int i);
	extern std::wstring WString(long l);
	extern std::wstring WString(uint64_t i);
	extern std::wstring WString(const std::string s);

	extern std::string QuoteString(const std::string& s);
	extern std::string UnQuoteString(const std::string& s);
	extern std::wstring QuoteWString(const std::wstring& s);
	extern std::wstring UnQuoteWString(const std::wstring& s);

	void FreeObject(Object* o);
	void FreeObject(shared_ptr<SmartObject> o);

	template <class T>
	T* CastObject(Object* o) { return dynamic_cast<T*>(o); }

	extern bool ExecuteCommand(const std::string& input, const bool quiet = false);
}

//Includes
#include "Enums.h"
#include "WString.h"
#include "OS.h"
#include "Control.h"
#include "Callback.h"

//Classes
#include "Classes/SmartObject.h"
#include "Classes/FileStream.h"
#include "Classes/Config.h"
#include "Classes/JSON.h"
//#include "Classes/GamePad.h"
#include "Classes/Tokenizer.h"
#include "Classes/ConCommand.h"
#include "Classes/CVar.h"
#include "Classes/Settings.h"
#include "Classes/Timer.h"
#include "Classes/SplashWindow.h"
#include "Classes/ImGuiLayer.h"
#include "Classes/ImageTexture.h"
#include "Classes/Input.h"

// Actor defs
typedef int exposed_int;
typedef float exposed_float;
typedef bool exposed_bool;
typedef string exposed_string;
typedef Leadwerks::Vec2 exposed_vec2;
typedef Leadwerks::Vec3 exposed_vec3;
typedef Leadwerks::Vec4 exposed_vec4;
typedef Leadwerks::Entity exposed_entity;
typedef void exposed_input;
typedef void exposed_output;
#define ATTACH_ENTITY_TO_ACTOR(_entity_, _actor_) if (_entity_->GetString("actor") == #_actor_) _entity_->SetActor(new _actor_())