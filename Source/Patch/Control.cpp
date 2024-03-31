#include "pch.h"
#include "LeadwerksPlus.h"

namespace Leadwerks
{
	// Time
	extern void InitTime();
	bool PauseState()
	{
		return timepausestate;
	}

	extern long starttime__;
	extern bool timeinitialized;
	void UpdateTime(const int framePerSecond)
	{
		double speedsmoothing = 0.5;
		double frametime = 1000.0 / ((double)framePerSecond);
		stepmode__ = 0;
		if (!timepausestate)
		{
			long newtime = Time::Millisecs();
			if (!timeinitialized)
			{
				InitTime();
			}
			else
			{
				newtime -= timeoffset__;
				long elapsed = newtime - systemtime__;
				if (elapsed < 1) elapsed = 1;
				time__ = newtime - starttime__;//+= elapsed;
				speed__ = float(speed__ * speedsmoothing) + ((1.0 - speedsmoothing) * (elapsed / frametime));//lerp to smooth speed value
				systemtime__ = newtime;

				// Fire callback
				FireCallback(CALLBACK_UPDATE, NULL, NULL);
			}
		}
	}

	// Events
	//std::multimap<const int, std::function<void(Event, Object*)>> eventcallbacks;
	struct EventListener
	{
		int id = EVENT_NONE;
		Object* source = NULL;
		Object* extra = NULL;
		std::function<bool(Event, Object*)> callback;
	};
	std::vector<EventListener> eventlisteners;

	static void FireCustomEvents(const Event& e)
	{
		//if (!eventcallbacks.empty())
		//{
		//	for (const auto& i : eventcallbacks)
		//	{
		//		const int id = i.first;
		//		if (id == e.id)
		//		{
		//			i.second(e.id, e.extra);
		//		}
		//	}
		//}

		if (!eventlisteners.empty())
		{
			for (const auto& l : eventlisteners)
			{
				auto currevent = e;
				if (l.id == EVENT_NONE)
				{
					// None allows live listening to all events.
					l.callback(currevent, l.extra);
				}
				else
				{
					// Do an ID check
					if (currevent.id == e.id)
					{
						if (!l.callback(currevent, l.extra))
						{
							//Print("Failed to callback " + String(currevent.id));
						}
					}
				}
			}
		}
	}

	bool PeekEvent()
	{
		return EventQueue::Peek();
	}

	Event WaitEvent()
	{
		const auto e = EventQueue::Wait();
		FireCustomEvents(e);
		return e;
	}

	void EmitEvent(const int id, Object* source, const int data, const int x, const int y, const int width, const int height, Object* extra)
	{
		EventQueue::Emit(id, source, data, x, y, width, height, extra);
	}

	void ListenEvent(const int id, Object* source, std::function<bool(Event, Object*)> func, Object* extra)
	{
		auto listener = EventListener();
		listener.id = id;
		listener.source = source;
		listener.extra = extra;
		listener.callback = func;
		eventlisteners.push_back(listener);

		//eventcallbacks.insert(pair<const int, std::function<void(Event, Object*)>>(id, func));
	}

	// Framebuffer
	bool startrendering = false;
	void UpdateRender(Framebuffer* framebuffer, const bool sync, const float framerate)
	{
		framebuffer->Sync(sync, framerate);
		if (!startrendering)
		{
			int vendor = -1;
			auto driver = Leadwerks::GraphicsDriver::GetCurrent();
			if (driver != NULL) vendor = driver->GetVendor();
			EmitEvent(EVENT_STARTRENDERER, NULL, vendor);
			startrendering = true;
		}
	}
}