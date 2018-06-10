#include "System.h"

BaseSystem::BaseSystem(std::vector<uint32_t>& componentTypes)
	: _componentTypes(componentTypes)
{ }

const std::vector<uint32_t>& BaseSystem::GetComponentTypes()
{
	return _componentTypes;
}
