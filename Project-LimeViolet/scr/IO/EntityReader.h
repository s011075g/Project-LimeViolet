#pragma once
#include <json/json.h>
#include "../ECS/ECS.h"
class EntityReader
{
public:
	static EntityHandle ReadEntity(Json::Value value, ECS* ecs);
};
