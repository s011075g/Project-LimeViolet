#pragma once
#include "../ECS/Component.h"
#include "../Common/Material.h"

struct MeshRenderComponent : Component<MeshRenderComponent>
{
	std::vector<Material> materials;
};
