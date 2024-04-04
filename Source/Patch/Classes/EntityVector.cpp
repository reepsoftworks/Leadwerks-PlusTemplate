#include "pch.h"
#include "../LeadwerksPlus.h"

namespace Leadwerks
{
	// Entity Vector
	EntityVector::EntityVector()
	{
		entities.clear();
	}

	EntityVector::~EntityVector()
	{
		entities.clear();
	}

	bool EntityVector::Find(Entity* entity)
	{
		if (entities.empty()) return false;
		return std::find(entities.begin(), entities.end(), entity) != entities.end();
	}

	void EntityVector::Add(Entity* entity)
	{
		entities.push_back(entity);
	}

	bool EntityVector::Remove(Entity* entity)
	{
		if (entities.empty()) return false;
		if (!Find(entity)) return false;
		entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
		return true;
	}

	// Entity Queue
	EntityQueue::EntityQueue()
	{
		m_maxsize = 1;
		release_on_pop = false;
	}

	EntityQueue::EntityQueue(std::size_t maxsize)
	{
		m_maxsize = maxsize;
		release_on_pop = false;
	}

	EntityQueue::~EntityQueue()
	{
		while (!entities.empty())
		{
			entities.pop();
		}
	}

	void EntityQueue::Push(Entity* entity)
	{
		entities.push(entity);
		if (entities.size() > m_maxsize)
		{
			if (entities.front() != NULL)
			{
				if (release_on_pop) entities.front()->Release();
			}
			entities.pop();
		}
	}
}