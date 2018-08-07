#pragma once
#include "../ECS/Component.h"
#include "../Maths/MathStructs.h"
#include "../Common/Color.h"

struct CameraComponent : Component<CameraComponent>
{
	Color4 clearColor;
	//Up vector
	Float3 up;
	//Position
	Float3 eye;
	float fieldOfView;
	//Looking at
	Float3 at;
	float nearPlane, farPlane;
};
