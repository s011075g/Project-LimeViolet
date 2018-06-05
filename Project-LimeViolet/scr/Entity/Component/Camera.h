#pragma once
#include "Component.h"
#include "../../Maths/MathStructs.h"
#include "../../Common/Color.h"

struct Camera : Component<Camera>
{
	//Camera's up vector
	Float4 up;
	//What the background is cleared to
	Color clearColor;
	float fieldOfView;
	//Clipping plane
	float near, far;
};
