#pragma once
#include "../ECS/Component.h"
#include "../Maths/MathStructs.h"
#include "../Common/Color.h"

struct CameraComponent : Component<CameraComponent>
{
	Float4 up;
	Color4 clearColor;
	Float3 eye;
	float fieldOfView;
	Float3 at;
	float nearPlane, farPlane;
};
