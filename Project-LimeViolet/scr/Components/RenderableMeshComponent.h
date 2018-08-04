#pragma once
#include "../ECS/Component.h"
#include "../Render/Geometry.h"

struct RenderableMeshComponent : Component<RenderableMeshComponent>
{
	Geometry* geometry;
};
