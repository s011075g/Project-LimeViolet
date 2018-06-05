#pragma once
#include "IComponent.h"
#include "../../Maths/Quaternion.h"

class Transform : public IComponent
{
private:
	Float4x4 position_matrix;
	Float4x4 rotationMatrix;
	Float4x4 scaleMatrix;

	Float4x4 worldMatrix;

	Quaternion rotation;
	Float3 position;
	Float3 scale;
public:
	Transform(GameObject* gameObject);
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
