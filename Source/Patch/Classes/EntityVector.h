#pragma once
#include "LeadwerksPlus.h"

namespace Leadwerks
{
	class EntityVector
	{
	public:
		std::vector<Entity*> entities;

		EntityVector();
		~EntityVector();

		bool Find(Entity* entity);
		void Add(Entity* entity);
		bool Remove(Entity* entity);
	};

	class EntityQueue
	{
		std::size_t m_maxsize;
	public:
		std::queue<Entity*> entities;
		bool release_on_pop;

		EntityQueue();
		EntityQueue(std::size_t maxsize);
		~EntityQueue();

		void Push(Entity* entity);
	};
}