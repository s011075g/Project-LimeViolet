#include "Entity.h"

static uint32_t Id = 0;

uint32_t Entity::NextId()
{
	return Id++;
}

Entity::Entity()
	: _id(NextId())
{ }

Entity::~Entity()
{ }

uint32_t Entity::GetId() const
{
	return _id;
}