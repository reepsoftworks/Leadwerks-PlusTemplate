#pragma once
#include "pch.h"

using namespace Leadwerks;

static void RegisterActors(Leadwerks::Entity* entity)
{
	ATTACH_ENTITY_TO_ACTOR(entity, Actor);
}