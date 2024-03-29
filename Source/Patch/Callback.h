#include "LeadwerksPlus.h"

namespace Leadwerks
{
	struct Callback
	{
		int id = -1;
		Object* source = NULL;
		Object* extra = NULL;
		std::function<void(Object*, Object*)> function;
	};

	extern void SetCallback(const int id, Object* source, std::function<void(Object*, Object*)> func, Object* extra = NULL);
	//extern void SetCallback(const int id, std::function<void(std::shared_ptr<SmartObject>, std::shared_ptr<SmartObject>)> func);
	extern void FireCallback(const int id, Object* source, Object* extra);
	//extern void FireCallback(const int id, std::shared_ptr<SmartObject> source = NULL, std::shared_ptr<SmartObject> extra = NULL);
	extern void RemoveCallback(Object* source);
}