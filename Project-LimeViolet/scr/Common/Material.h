#pragma once
#include "../Common/Color.h"
#include "../Common/Texture.h"

struct Material
{
	texture diffuseTexture;
	Color4 diffuseColor;
	texture specularTexture;
	Color3 specularColor;
	float specularPower;
	texture normalTexture;
	texture occlusionTexture;
	Material();
};

inline Material::Material()
	: diffuseTexture(nullptr), diffuseColor(Color4(1, 1, 1, 1)), specularTexture(nullptr), specularColor(Color3(1, 1, 1)), specularPower(0.5f), normalTexture(nullptr), occlusionTexture(nullptr)
{ };
