//========= Copyright Reep Softworks, All rights reserved. ============//
//
// Purpose: 
//
//=====================================================================//
#include "pch.h"
#include "SmartObject.h"

namespace Leadwerks
{
	SmartObject::SmartObject()
	{
	}

	SmartObject::~SmartObject()
	{
	}

	shared_ptr<SmartObject> SmartObject::Self()
	{
		return shared_from_this();
	}

	std::string SmartObject::GetAddress()
	{
#ifndef __APPLE__
		const char* add = reinterpret_cast<const char*>(&Self());
		std::string str = add;
		return add;
#else
        return "STUBBED: Fix Object::GetAddress() for macOS!";
#endif
	}
}
