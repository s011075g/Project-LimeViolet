#pragma once
#include "IComponent.h"
#include "../../Maths/Quaternion.h"

class Transform : public IComponent
{
private:
	Float4x4 _positionMatrix;
	Float4x4 _rotationMatrix;
	Float4x4 _scaleMatrix;

	Float4x4 _worldMatrix;

	Quaternion _rotation;
	Float3 _position;
	Float3 _scale;
public:
	Transform();
	~Transform();

	void Start() override;
	void Update() override;

	void UpdateMatrix();
	void UpdateMatrix(Float4x4& parentMatrix);

	void SetPosition(Float3& position);
	void SetRotation(Quaternion& quaternion);
	void SetScale(Float3& scale);

	Float3 GetPosition() const;
	Quaternion GetRotation() const;
	Float3 GetScale() const;
};
