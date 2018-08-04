#pragma once
#include "../Maths/Quaternion.h"

class Transform
{
	Quaternion _rotation;
	Float3 _position;
	Float3 _scale;
public:
	Transform();
	explicit Transform(const Float3 position, const Float3 scale = Float3(1,1,1), const Quaternion& rotation = Quaternion());
	~Transform() = default;

	Float4x4 ToMatrix() const;

	void SetPosition(Float3& position);
	void SetRotation(Quaternion& quaternion);
	void SetScale(Float3& scale);

	Float3 GetPosition() const;
	Quaternion GetRotation() const;
	Float3 GetScale() const;
};