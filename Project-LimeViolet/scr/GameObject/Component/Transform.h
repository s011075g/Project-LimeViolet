#pragma once
#include "IComponent.h"
#include "../../Maths/Quaternion.h"

class Transform : public IComponent
{
private:
	Float3 _position;
	Float3 _scale;
	Quaternion* _rotation;
public:
	Transform();
	~Transform();

	void Start() override;
	void Update() override;
};
