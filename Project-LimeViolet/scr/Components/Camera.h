﻿#pragma once
#include "../ECS/Component.h"
#include "../Maths/MathStructs.h"
#include "../Common/Color4.h"

struct Camera : Component<Camera>
{
	//Camera's up vector
	Float4 up;
	//What the background is cleared to
	Color4 clearColor;
	float fieldOfView;
	//Clipping plane
	float near, far;
};