#pragma once
#include "Component/Component.h"

class Entity //Will have a list of Components
{
public:
	Entity();
	~Entity();
	//todo Better to make it templated so memory is correct assigned
	bool AddComponent(); //todo add component only once
	bool RemoveComponent(); //todo remove component correctly
};

