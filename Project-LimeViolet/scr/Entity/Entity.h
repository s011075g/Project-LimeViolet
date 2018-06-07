#pragma once
#include "Component/Component.h"

class Entity //Will have a list of Components
{ //todo add an ID to each Entity
private:
	std::vector<uint8_t> _memory;
public:
	Entity();
	~Entity();
	
	template<typename Component>
	bool AddComponent(); //todo add component only once

	template<typename Component>
	void RemoveComponent(); //todo remove component correctly
};