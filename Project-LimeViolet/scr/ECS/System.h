#pragma once
#include "Component.h"

class BaseSystem
{
public:
	BaseSystem(std::vector<uint32_t>& componentTypes);
	virtual ~BaseSystem() = default;
	virtual void UpdateComponents(const float delta, BaseComponent** components) = 0;
	const std::vector<uint32_t>& GetComponentTypes() const;
private:
	std::vector<uint32_t> _componentTypes;
};