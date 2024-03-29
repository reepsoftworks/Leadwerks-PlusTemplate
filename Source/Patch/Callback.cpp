#include "pch.h"
#include "LeadwerksPlus.h"

namespace Leadwerks
{
	static std::multimap<const int, std::function<void(Object*, Object*)>> rawcallbacks;
	static std::multimap<const int, std::function<void(std::shared_ptr<SmartObject>, std::shared_ptr<SmartObject>)>> callbacks;

	void SetCallback(const int id, std::function<void(Object*, Object*)> func)
	{
		rawcallbacks.insert(pair<const int, std::function<void(Object*, Object*)>>(id, func));
	}

	void FireCallback(const int id, Object* source, Object* extra)
	{
		for (const auto& i : rawcallbacks)
		{
			auto key = i.first;
			if (id == key) i.second(source, extra);
		}
	}

	void SetCallback(const int id, std::function<void(std::shared_ptr<SmartObject>, std::shared_ptr<SmartObject>)> func)
	{
		callbacks.insert(pair<const int, std::function<void(std::shared_ptr<SmartObject>, std::shared_ptr<SmartObject>)>>(id, func));
	}

	void FireCallback(const int id, std::shared_ptr<SmartObject> source, std::shared_ptr<SmartObject> extra)
	{
		for (const auto& i : callbacks)
		{
			auto key = i.first;
			if (id == key) i.second(source, extra);
		}
	}
}