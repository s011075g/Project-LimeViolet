#pragma once
#include "../ECS/Component.h"
#include "../Maths/Quaternion.h"

struct Transform : Component<Transform>
{
	Float4x4 positionMatrix; //Position Matrix (Could possibly replaced in the future)
	Float4x4 rotationMatrix; //Rotation Matrix
	Float4x4 scaleMatrix; //Scale matrix

	Float4x4 worldMatrix; //todo possibly make it the only matrix. Would have to recalculate matrices we may not need to though - testing required

	Quaternion rotation;
	Float3 position;
	Float3 scale;
};
