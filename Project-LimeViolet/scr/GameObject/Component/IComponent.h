#pragma once

class IComponent
{
public:
	IComponent();
	virtual ~IComponent();

	virtual void Start() = 0;
	virtual void Update() = 0;
};
