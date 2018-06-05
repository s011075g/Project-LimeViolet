#include "Transform.h"

Transform::Transform(GameObject* gameObject)
	: IComponent(gameObject)
{ }

Transform::~Transform()
{ }

void Transform::Start()
{ }

void Transform::Update()
{ }

void Transform::UpdateMatrix()
{
	worldMatrix = Float4x4::Identity() * scaleMatrix * rotationMatrix * position_matrix;
}

void Transform::UpdateMatrix(Float4x4& parentMatrix)
{
	worldMatrix = Float4x4::Identity() * scaleMatrix * rotationMatrix * position_matrix * parentMatrix;
}

void Transform::SetPosition(Float3& position)
{
	position = position;
	position_matrix.m41 = position.x;
	position_matrix.m42 = position.y;
	position_matrix.m43 = position.z;
	position_matrix.m44 = 1.0f;
}

void Transform::SetRotation(Quaternion& quaternion)
{
	rotation = quaternion;
	rotationMatrix = Float4x4(rotation.GetRotationMatrix());
}

void Transform::SetScale(Float3& scale)
{
	scale = scale;
	scaleMatrix.m11 = scale.x;
	scaleMatrix.m22 = scale.y;
	scaleMatrix.m33 = scale.z;
	scaleMatrix.m44 = 1.0f;
}

Float3 Transform::GetPosition() const
{
	return position;
}

Quaternion Transform::GetRotation() const
{
	return rotation;
}

Float3 Transform::GetScale() const
{
	return scale;
}
