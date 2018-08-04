#pragma once
#include "../ECS/Component.h"
#include "../Common/Material.h"

typedef void* Shader;

struct MaterialComponent : Component<MaterialComponent>
{
	std::vector<Material*> materials;
	Shader shader;
};
