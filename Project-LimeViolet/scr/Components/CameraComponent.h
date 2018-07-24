#pragma once
#include "../ECS/Component.h"
#include "../Maths/MathStructs.h"
#include "../Common/Color.h"

struct CameraComponent : Component<CameraComponent>
{
	//Camera's up vector
	Float4 up;
	//What the background is cleared to
	Color4 clearColor;
	Float3 eye;
	float fieldOfView;
	Float3 at;
	//Clipping plane
	float near, far;
};
