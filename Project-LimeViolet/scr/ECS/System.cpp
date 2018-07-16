#include "System.h"

BaseSystem::BaseSystem()
{ }

const std::vector<uint32_t>& BaseSystem::GetComponentTypes() const
{
	return _componentTypes;
}

const std::vector<uint32_t>& BaseSystem::GetComponentFlags() const
{
	return _componentFlags;
}

bool BaseSystem::IsValid() const
{
	for (uint32_t i = 0; i < _componentFlags.size(); i++)
		if ((_componentFlags[i] & FLAG_OPTIONAL) == 0)
			return true;
	return false;
}

void BaseSystem::AddComponentType(const uint32_t componentType, const uint32_t componentFlag)
{
	_componentTypes.push_back(componentType);
	_componentFlags.push_back(componentFlag);
}

bool SystemList::AddSystem(BaseSystem& system)
{
	if (!system.IsValid())
		return false;
	_systems.push_back(&system);
	return true;
}

bool SystemList::RemoveSystem(BaseSystem& system)
{
	for (uint32_t i = 0; i < _systems.size(); i++)
		if (&system == _systems[i])
		{
			_systems.erase(_systems.begin() + i);
			return true;
		}
	return false;
}

uint32_t SystemList::Size() const
{
	return _systems.size();
}

BaseSystem* SystemList::operator[](const uint32_t& index)
{
	return _systems[index];
}
