#include "pch.h"
#include "../LeadwerksPlus.h"

namespace Leadwerks
{
	Timer::Timer()
	{
		ticks = 0;
		frequency = 0;
		stopped = false;
	}

	Timer::~Timer()
	{
		RemoveCallback(this);
		ticks = 0;
		frequency = 0;
		stopped = false;
	}

	void Timer::Update()
	{
		if (!stopped)
		{
			time_t now = Time::GetCurrent();
			if (now > time_t(ticks + frequency))
			{
				EmitEvent(Event::TimerTick, this);
				ticks = Time::GetCurrent();
			}
		}
	}

	void Timer::Start()
	{
		if (!stopped) return;
		ticks = Time::GetCurrent();
		frequency = 0;
		stopped = false;
	}

	void Timer::Stop()
	{
		ticks = 0;
		frequency = 0;
		stopped = true;
	}

	void TimerCallback(Object* source, Object* extra)
	{
		auto timer = CastObject<Timer>(source);
		if (timer) timer->Update();
	}

	Timer* Timer::Create(const int frequency)
	{
		auto timer = new Timer();
		timer->frequency = frequency;
		SetCallback(CALLBACK_UPDATE, timer, TimerCallback);
		timer->Start();
		return timer;
	}
}