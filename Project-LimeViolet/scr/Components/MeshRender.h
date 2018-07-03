#pragma once
#include "../ECS/Component.h"
#include "../Common/Material.h"

struct MeshRender : Component<MeshRender>
{
	std::vector<Material> materials;
};
