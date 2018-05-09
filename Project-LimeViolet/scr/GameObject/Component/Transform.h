#pragma once
#include "IComponent.h"

class Transform : public IComponent
{
private:

public:
	Transform();
	~Transform();

	void Start() override;
	void Update() override;
};
