#pragma once
#include "LeadwerksPlus.h"

namespace Leadwerks
{
	class Timer : public Object
	{
		bool stopped;
		uint64_t ticks;
		void Update();

	public:
		int frequency;

		Timer();
		virtual ~Timer();
		virtual void Start();
		virtual void Stop();
		friend void TimerCallback(Object* source, Object* extra);
		static Timer* Create(const int frequency);
	};
}