#include "Component.h"
#include <array>

std::vector<std::tuple<ComponentCreateFunction, ComponentFreeFunction, size_t>> BaseComponent::_componentTypes;

//Returns the next Entity ID
uint32_t BaseComponent::RegisterComponentType(ComponentCreateFunction create, ComponentFreeFunction free, size_t size)
{
	uint32_t componentId = _componentTypes.size();
	_componentTypes.push_back(std::tuple<ComponentCreateFunction, ComponentFreeFunction, size_t>(create, free, size));
	return componentId++;
}

ComponentCreateFunction BaseComponent::GetTypeCreateFunction(const uint32_t id)
{
	return std::get<0>(_componentTypes[id]);
}

ComponentFreeFunction BaseComponent::GetTypeFreeFunction(const uint32_t id)
{
	return std::get<1>(_componentTypes[id]);
}

size_t BaseComponent::GetTypeSize(const uint32_t id)
{
	return std::get<2>(_componentTypes[id]);
}
