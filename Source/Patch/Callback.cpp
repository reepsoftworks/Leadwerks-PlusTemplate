#include "pch.h"
#include "LeadwerksPlus.h"

namespace Leadwerks
{
	//static std::multimap<const int, std::function<void(Object*, Object*)>> rawcallbacks;
	static std::vector<Callback> rawcallbacks;
	//static std::multimap<const int, std::function<void(std::shared_ptr<SmartObject>, std::shared_ptr<SmartObject>)>> callbacks;

	void SetCallback(const int id, Object* source, std::function<void(Object*, Object*)> func, Object* extra)
	{
		auto callback = Callback();
		callback.id = id;
		callback.function = func;
		callback.source = source;
		callback.extra = extra;
		rawcallbacks.push_back(callback);

		//rawcallbacks.insert(pair<const int, std::function<void(Object*, Object*)>>(id, func));
	}

	void FireCallback(const int id, Object* source, Object* extra)
	{
		if (!rawcallbacks.empty())
		{
			for (const auto& l : rawcallbacks)
			{
				auto callback = l;
				if (callback.id == id)
				{
					if (source != NULL)
					{
						if (source == callback.source)
						{
							callback.function(callback.source, callback.extra);
						}
					}
					else
					{
						callback.function(callback.source, callback.extra);
					}
				}
			}
		}

		//for (const auto& i : rawcallbacks)
		//{
		//	auto key = i.first;
		//	if (id == key) i.second(source, extra);
		//}
	}

	void RemoveCallback(Object* source)
	{
		if (!rawcallbacks.empty())
		{
			for (const auto& cb : rawcallbacks)
			{
				auto s = cb.source;
				if (s = source)
				{
					rawcallbacks.erase(
						std::remove_if(rawcallbacks.begin(), rawcallbacks.end(), [&](Callback const& pet) {
							return cb.source == source;
							}),
						rawcallbacks.end());
					break;
				}
			}
		}
	}

	void SetCallback(const int id, std::function<void(std::shared_ptr<SmartObject>, std::shared_ptr<SmartObject>)> func)
	{
		//callbacks.insert(pair<const int, std::function<void(std::shared_ptr<SmartObject>, std::shared_ptr<SmartObject>)>>(id, func));
	}

	void FireCallback(const int id, std::shared_ptr<SmartObject> source, std::shared_ptr<SmartObject> extra)
	{
		//for (const auto& i : callbacks)
		//{
		//	auto key = i.first;
		//	if (id == key) i.second(source, extra);
		//}
	}
}