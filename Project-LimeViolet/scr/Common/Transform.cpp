#include "Transform.h"

Transform::Transform() 
:_position(0,0,0), _scale(1,1,1)
{ }

Transform::Transform(const Float3 position, const Float3 scale, const Quaternion& rotation)
	: _rotation(rotation), _position(position), _scale(scale)
{
	_rotation.Normalise();
}

Float4x4 Transform::ToMatrix() const
{
	Float4x4 positionMatrix = Float4x4::Identity();
	positionMatrix.m41 = _position.x;
	positionMatrix.m42 = _position.y;
	positionMatrix.m43 = _position.z;
	Float4x4 rotationMatrix = Float4x4(_rotation.GetRotationMatrix());
	rotationMatrix.m44 = 1.0f;
	Float4x4 scaleMatrix = Float4x4::Identity();
	scaleMatrix.m11 = _scale.x;
	scaleMatrix.m22 = _scale.y;
	scaleMatrix.m33 = _scale.z;
	//return positionMatrix * rotationMatrix * scaleMatrix * Float4x4::Identity();
	return Float4x4::Identity() * scaleMatrix * rotationMatrix * positionMatrix;
}

void Transform::SetPosition(Float3& position)
{
	_position = position;
}

void Transform::SetPosition(const Float3 position)
{
	_position = position;
}

void Transform::SetRotation(Quaternion& quaternion)
{
	_rotation = quaternion;
	_rotation.Normalise();
}
void Transform::SetScale(Float3& scale)
{
	_scale = scale;
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
