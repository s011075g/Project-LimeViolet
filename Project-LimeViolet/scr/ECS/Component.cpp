#include "Component.h"
#include <array>

std::vector<std::tuple<ComponentCreateFunction, ComponentFreeFunction, size_t>>* BaseComponent::_componentTypes;

//Returns the next Entity ID
uint32_t BaseComponent::RegisterComponentType(ComponentCreateFunction create, ComponentFreeFunction free, size_t size)
{
	if (_componentTypes == nullptr)
		_componentTypes = new std::vector<std::tuple<ComponentCreateFunction, ComponentFreeFunction, size_t>>();
	uint32_t componentId = static_cast<uint32_t>(_componentTypes->size());
	_componentTypes->push_back(std::tuple<ComponentCreateFunction, ComponentFreeFunction, size_t>(create, free, size));
	return componentId++;
}

ComponentCreateFunction BaseComponent::GetTypeCreateFunction(const uint32_t id)
{
	return std::get<0>((*_componentTypes)[id]);
}

ComponentFreeFunction BaseComponent::GetTypeFreeFunction(const uint32_t id)
{
	return std::get<1>((*_componentTypes)[id]);
}

size_t BaseComponent::GetTypeSize(const uint32_t id)
{
	return std::get<2>((*_componentTypes)[id]);
}

bool BaseComponent::IsTypeValid(const uint32_t id)
{
	return id < _componentTypes->size();
}
