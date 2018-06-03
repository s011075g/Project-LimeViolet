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
	_worldMatrix = Float4x4::Identity() * _scaleMatrix * _rotationMatrix * _positionMatrix;
}

void Transform::UpdateMatrix(Float4x4& parentMatrix)
{
	_worldMatrix = Float4x4::Identity() * _scaleMatrix * _rotationMatrix * _positionMatrix * parentMatrix;
}

void Transform::SetPosition(Float3& position)
{
	_position = position;
	_positionMatrix.m41 = position.x;
	_positionMatrix.m42 = position.y;
	_positionMatrix.m43 = position.z;
	_positionMatrix.m44 = 1.0f;
}

void Transform::SetRotation(Quaternion& quaternion)
{
	_rotation = quaternion;
	_rotationMatrix = Float4x4(_rotation.GetRotationMatrix());
}

void Transform::SetScale(Float3& scale)
{
	_scale = scale;
	_scaleMatrix.m11 = scale.x;
	_scaleMatrix.m22 = scale.y;
	_scaleMatrix.m33 = scale.z;
	_scaleMatrix.m44 = 1.0f;
}

Float3 Transform::GetPosition() const
{
	return _position;
}

Quaternion Transform::GetRotation() const
{
	return _rotation;
}

Float3 Transform::GetScale() const
{
	return _scale;
}
