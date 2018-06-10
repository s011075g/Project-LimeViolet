#pragma once
#include "Component.h"
#include "System.h"
#include <map>

class ECS
{
public:
	ECS() = default;
	~ECS();
	//Entity methods
	EntityHandler MakeEntity(BaseComponent* componets, const uint32_t* componentIds, size_t numComponents);
	void RemoveEntity(EntityHandler handle);
	//Component methods
	template<class Component>
	void AddComponent(EntityHandler entity, Component* component);
	template<class Component>
	void RemoveComponent(EntityHandler entity);
	template<class Component>
	void GetComponent(EntityHandler entity);
	//System methods
	void AddSystem(BaseSystem& system);
	void UpdateSystems(const float delta);
	void RemoveSystem(BaseSystem& system);
private:
	std::vector<BaseSystem*> _systems;
	std::map<uint32_t, std::vector<uint8_t>> _components;
	std::vector<std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>*> _entities;

	static inline std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>* HandleToRawType(const EntityHandler handle);
	static inline uint32_t HandleToEntityIndex(const EntityHandler handle);
	static inline std::vector<std::pair<uint32_t, uint32_t>>& HandleToEntity(const EntityHandler handle);

	ECS(const ECS& other);
	void operator=(const ECS& other) const;
};

template <class Component>
void ECS::AddComponent(EntityHandler entity, Component* component)
{
	//todo finish 
}

template <class Component>
void ECS::RemoveComponent(EntityHandler entity)
{
	//todo finish 
}

template <class Component>
void ECS::GetComponent(EntityHandler entity)
{
	//todo finish 
}
