#include "Entity.h"

Entity::Entity()
{ }

Entity::~Entity()
{ }

template<typename Component>
bool Entity::AddComponent()
{
	return false;
}

template <typename Component>
void Entity::RemoveComponent()
{ }