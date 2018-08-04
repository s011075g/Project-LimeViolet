#pragma once
#include "../ECS/Component.h"
#include "../Common/Transform.h"

struct TransformComponent : Component<TransformComponent>
{
	Transform transform;
};
