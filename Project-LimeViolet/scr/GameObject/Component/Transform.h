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

	Float3 _position;
	Float3 _scale;
	Quaternion* _rotation;
public:
	Transform();
	~Transform();

	void Start() override;
	void Update() override;

	void UpdateMatrix();
	void UpdateMatrix(Float4x4& parentMatrix);

	void SetPosition(Float3& position);
	void SetRotation(Quaternion* quaternion);
	void SetScale(Float3& scale);
};
