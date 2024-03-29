#include "LeadwerksPlus.h"

namespace Leadwerks
{
	extern void SetCallback(const int id, std::function<void(Object*, Object*)> func);
	extern void SetCallback(const int id, std::function<void(std::shared_ptr<SmartObject>, std::shared_ptr<SmartObject>)> func);
	extern void FireCallback(const int id, Object* source, Object* extra);
	extern void FireCallback(const int id, std::shared_ptr<SmartObject> source = NULL, std::shared_ptr<SmartObject> extra = NULL);
}