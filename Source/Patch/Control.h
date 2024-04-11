#pragma once
#include "LeadwerksPlus.h"

namespace Leadwerks
{
	// Time
	extern bool PauseState();
	extern float GetAppSpeed();
	extern void UpdateTime(const int framePerSecond = 60);

	// Events
	extern bool PeekEvent();
	extern Event WaitEvent();
	extern void EmitEvent(const int id, Object* source = NULL, const int data = 0, const int x = 0, const int y = 0, const int width = 0, const int height = 0, Object* extra = NULL);
	extern void ListenEvent(const int id, Object* source, std::function<bool(Event, Object*)> func, Object* extra = NULL);

	// Framebuffer
	extern void UpdateRender(Framebuffer* framebuffer, const bool sync = false, const float framerate = 0.0f);
}