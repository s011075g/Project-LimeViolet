#pragma once
#include "../ECS/Component.h"
#include "../Common/Material.h"

typedef void* Shader;

struct MeshRenderComponent : Component<MeshRenderComponent>
{
	std::vector<Material> materials;
	Shader shader;
};
