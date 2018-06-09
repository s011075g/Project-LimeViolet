#pragma once
#include "Component/Component.h"

class Entity //Will have a list of Components
{ //todo add an ID to each Entity
private:
	std::vector<uint8_t> _memory;
	const uint32_t _id;
public:
	Entity();
	~Entity();
	
	template<typename Component>
	bool AddComponent(); //todo add component only once

	template<typename Component>
	void RemoveComponent(); //todo remove component correctly

	uint32_t GetId() const;
private:
	static uint32_t NextId();
};

template<typename Component>
bool Entity::AddComponent()
{
	return false;
}

template <typename Component>
void Entity::RemoveComponent()
{ }