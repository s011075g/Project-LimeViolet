#pragma once
#include "../Maths/MathStructs.h"
#include "../ECS/Component.h"
#include "Material.h"
#include "../Common/Color4.h"
#include "../Common/Texture.h"

struct Material : Component<Material>
{
	texture diffuseTexture;
	Color4 diffuseColor;
	texture specularTexture;
	Color3 specularColor;
	float specularPower;
	texture normalTexture;
	texture occlusionTexture;
};
