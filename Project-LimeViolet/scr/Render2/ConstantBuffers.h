#pragma once
#include "../Maths/MathStructs.h"

struct PerDrawBuffer
{
	Float4x4 viewProjection;
};

struct PerObjectBuffer
{
	Float4x4 world;
	Float4 diffuseColor;
	Float3 specularColor;
	float specularPower;
};