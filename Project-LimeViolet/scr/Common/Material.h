#pragma once
#include "../Maths/MathStructs.h"

struct Material //48 bytes, 4 byte alligned
{
	Material() = default;

	Float4 ambient; //16
	Float4 diffuse; //32
	Float3 specular; //44
	float specularPower; //48
};