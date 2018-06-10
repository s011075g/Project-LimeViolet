#include "ECS.h"

ECS::~ECS()
{ }

EntityHandler ECS::MakeEntity(BaseComponent* componets, const uint32_t* componentIds, size_t numComponents)
{
	//todo finish 
}

void ECS::RemoveEntity(EntityHandler handle)
{
	//todo finish 
}

void ECS::AddSystem(BaseSystem& system)
{
	_systems.push_back(&system);
}

void ECS::UpdateSystems(const float delta)
{
	//todo finish 
}

void ECS::RemoveSystem(BaseSystem& system)
{
	//todo finish 
}

std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>* ECS::HandleToRawType(const EntityHandler handle)
{
	return static_cast<std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>*>(handle);
}

uint32_t ECS::HandleToEntityIndex(const EntityHandler handle)
{
	return HandleToRawType(handle)->first;
}

std::vector<std::pair<uint32_t, uint32_t>>& ECS::HandleToEntity(const EntityHandler handle)
{
	return HandleToRawType(handle)->second;
}

ECS::ECS(const ECS& other)
{
	(void)other;
}

void ECS::operator=(const ECS& other) const
{
	(void)other;
}
