#pragma once
#include "../ECS/Component.h"
#include "../Render/Geometry.h"

struct RenderModel : Component<RenderModel>
{
	//todo plan what the rendermodel needs to render
	Geometry* geometry;
};
