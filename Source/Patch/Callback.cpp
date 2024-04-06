#include "pch.h"
#include "LeadwerksPlus.h"

namespace Leadwerks
{
	static std::vector<Callback*> rawcallbacks;
	void SetCallback(const int id, Object* source, std::function<void(Object*, Object*)> func, Object* extra)
	{
		auto callback = new Callback();
		callback->id = id;
		callback->function = func;
		Debug::Assert(source != NULL, "Callback source can not be NULL!");
		callback->source = source;
		callback->extra = extra;
		rawcallbacks.push_back(callback);
	}

	void FireCallback(const int id, Object* source, Object* extra)
	{
		if (!rawcallbacks.empty())
		{
			for (auto callback : rawcallbacks)
			{
				Debug::Assert(callback->source != NULL, "Callback source is NULL! Did you forget to use RemoveCallback()?");
				if (callback->id == id) callback->function(callback->source, callback->extra);
			}
		}
	}

	void RemoveCallback(Object* source)
	{
		if (!rawcallbacks.empty())
		{
			for (const auto& cb : rawcallbacks)
			{
				auto s = cb->source;
				if (s == source)
				{
					const bool found = std::find(rawcallbacks.begin(), rawcallbacks.end(), cb) != rawcallbacks.end();
					if (found)
					{
						rawcallbacks.erase(std::remove(rawcallbacks.begin(), rawcallbacks.end(), cb), rawcallbacks.end());
					}
				}
			}
		}
	}
}