#pragma once
#include "../Maths/MathStructs.h"

//48 bytes
struct ObjectVertex 
{
	Float3 vertex; //12
	Float2 uv; //8
	Float3 normal; //12
	Float4 tangent;//16
};