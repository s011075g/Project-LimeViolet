#pragma once
#include "Component.h"
#include "../../Maths/Quaternion.h"

struct Transform : Component<Transform>
{
	Float4x4 positionMatrix;
	Float4x4 rotationMatrix;
	Float4x4 scaleMatrix;

	Float4x4 worldMatrix;

	Quaternion rotation;
	Float3 position;
	Float3 scale;
};