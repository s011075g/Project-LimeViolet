#pragma once
#include "../ECS/Component.h"
#include "../Render/Geometry.h"

struct RenderableMeshComponent : Component<RenderableMeshComponent>
{
	//todo plan what the rendermodel needs to render
	Geometry* geometry;
};
