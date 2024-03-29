#pragma once
#include "../LeadwerksPlus.h"

namespace Leadwerks
{
	class SmartObject : public std::enable_shared_from_this<SmartObject>
	{
	protected:
		SmartObject();

	public:
		virtual ~SmartObject();

		template <class T>
		shared_ptr<T> As() { return std::dynamic_pointer_cast<T>(Self()); }
		shared_ptr<SmartObject> Self();
		std::string GetAddress();
	};
}