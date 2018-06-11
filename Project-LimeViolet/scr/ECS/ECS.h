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
	EntityHandle MakeEntity(BaseComponent* entityComponets, const uint32_t* componentIds, size_t numComponents);
	void RemoveEntity(EntityHandle handle);
	//Component methods
	template<class Component>
	inline void AddComponent(EntityHandle entity, Component* component);
	template<class Component>
	bool RemoveComponent(EntityHandle entity);
	template<class Component>
	Component* GetComponent(EntityHandle entity);
	//System methods
	void AddSystem(BaseSystem& system);
	void UpdateSystems(const float delta);
	bool RemoveSystem(BaseSystem& system);
private:
	std::vector<BaseSystem*> _systems;
	std::map<uint32_t, std::vector<uint8_t>> _components;
	std::vector<std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>*> _entities;

	static inline std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>* HandleToRawType(const EntityHandle handle);
	static inline uint32_t HandleToEntityIndex(const EntityHandle handle);
	static inline std::vector<std::pair<uint32_t, uint32_t>>& HandleToEntity(const EntityHandle handle);

	void AddComponentInternal(EntityHandle handle, std::vector<std::pair<uint32_t, uint32_t>>& entity, uint32_t componentId, BaseComponent* component);
	void DeleteComponent(uint32_t componentId, uint32_t index);
	bool RemoveComponentInternal(EntityHandle handle, uint32_t componentId);
	static BaseComponent* GetComponentInternal(std::vector<std::pair<uint32_t, uint32_t>>& entityComponents, std::vector<uint8_t>& array, uint32_t componentId);

	void UpdateSystemsWithMultipleComponents(uint32_t index, float delta, const std::vector<uint32_t>& componentTypes, std::vector<BaseComponent*> componentParam, std::vector<std::vector<uint8_t>*>& componentArrays);
	uint32_t FindLeastCommonComponent(const std::vector<uint32_t>& componentTypes);

	ECS(const ECS& other);
	void operator=(const ECS& other) const;
};

template <class Component>
inline void ECS::AddComponent(EntityHandle entity, Component* component)
{
	AddComponentInternal(entity, HandleToEntity(entity), Component::ID, component);
}

template <class Component>
bool ECS::RemoveComponent(EntityHandle entity)
{
	return RemoveComponentInternal(entity, Component::ID);
}

template <class Component>
Component* ECS::GetComponent(const EntityHandle entity)
{
	return GetComponentInternal(HandleToEntity(entity), _components[Component::ID], Component::ID);
}
