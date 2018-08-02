#pragma once
#include "Component.h"
#include "System.h"
#include <map>
#include "../Common/Utilities.h"

//TheBennyBox ECS tutorial was used as a base

class ECS
{
public:
	ECS() = default;
	~ECS();

	//Entity methods
	EntityHandle MakeEntity(BaseComponent** entityComponents, const uint32_t* componentIds, size_t numComponents);
	void RemoveEntity(EntityHandle handle);
	//Takes in a list of components to produce an entity
	template<class... Component>
	EntityHandle MakeEntity(Component&... component);
	//Component methods
	template<class Component>
	void AddComponent(EntityHandle entity, Component* component);
	template<class Component>
	bool RemoveComponent(EntityHandle entity);
	template<class Component>
	Component* GetComponent(EntityHandle entity);

	//System methods
	void UpdateSystems(SystemList& systems, const float delta);
private:
	
	std::map<uint32_t, std::vector<uint8_t>> _components;
	std::vector<std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>*> _entities;

	static inline std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>* HandleToRawType(const EntityHandle handle);
	static inline uint32_t HandleToEntityIndex(const EntityHandle handle);
	static inline std::vector<std::pair<uint32_t, uint32_t>>& HandleToEntity(const EntityHandle handle);

	inline void AddComponentInternal(EntityHandle handle, std::vector<std::pair<uint32_t, uint32_t>>& entity, uint32_t componentId, BaseComponent* component);
	void DeleteComponent(uint32_t componentId, uint32_t index);
	inline bool RemoveComponentInternal(EntityHandle handle, uint32_t componentId);
	inline static BaseComponent* GetComponentInternal(std::vector<std::pair<uint32_t, uint32_t>>& entityComponents, std::vector<uint8_t>& array, uint32_t componentId);

	void UpdateSystemWithMultipleComponents(uint32_t index, SystemList& systems, float delta, const std::vector<uint32_t>& componentTypes, std::vector<BaseComponent*>& componentParam, std::vector<std::vector<uint8_t>*>& componentArrays);
	uint32_t FindLeastCommonComponent(const std::vector<uint32_t>& componentTypes, const std::vector<uint32_t>& componentFlags);

	ECS(const ECS& other);
	void operator=(const ECS& other) const;
};

//Untested
template <class ... Component>
EntityHandle ECS::MakeEntity(Component&... component)
{
	auto newEntity = new std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>();
	EntityHandle handle = static_cast<EntityHandle>(newEntity);

	//Expander trick used - https://stackoverflow.com/questions/30563254/how-can-i-expand-call-to-variadic-template-base-classes/30563282#30563282
	//C++17 could use the fold expression 
	using expander = bool[];
	//No error checking - see next comment
	(void)expander {(AddComponentInternal(handle, newEntity->second, Component::ID, static_cast<BaseComponent*>(&component)),false)...};

	/*if (!BaseComponent::IsTypeValid(Component::ID))
	{
		Utilities::Write("ECS Component Type not valid: " + Component::ID, Utilities::LEVEL::ERROR_LEVEL);
		delete newEntity;
		return;
	}
	AddComponentInternal(handle, newEntity->second, Component::ID, comp);*/

	newEntity->first = static_cast<uint32_t>(_entities.size());
	_entities.push_back(newEntity);
	return handle;
}

template <class Component>
void ECS::AddComponent(EntityHandle entity, Component* component)
{
	AddComponentInternal(entity, HandleToEntity(entity), Component::ID, component);
}

template <class Component>
bool ECS::RemoveComponent(const EntityHandle entity)
{
	return RemoveComponentInternal(entity, Component::ID);
}

template <class Component>
Component* ECS::GetComponent(const EntityHandle entity)
{
	return static_cast<Component*>(GetComponentInternal(HandleToEntity(entity), _components[Component::ID], Component::ID));
}

inline std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>* ECS::HandleToRawType(const EntityHandle handle)
{
	return static_cast<std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>*>(handle);
}

inline uint32_t ECS::HandleToEntityIndex(const EntityHandle handle)
{
	return HandleToRawType(handle)->first;
}

inline std::vector<std::pair<uint32_t, uint32_t>>& ECS::HandleToEntity(const EntityHandle handle)
{
	return HandleToRawType(handle)->second;
}

inline void ECS::AddComponentInternal(const EntityHandle handle, std::vector<std::pair<uint32_t, uint32_t>>& entity, const uint32_t componentId, BaseComponent* component)
{
	ComponentCreateFunction create = BaseComponent::GetTypeCreateFunction(componentId);
	std::pair<uint32_t, uint32_t> pair;
	pair.first = componentId;
	pair.second = create(_components[componentId], handle, component);
	entity.push_back(pair);
}

inline bool ECS::RemoveComponentInternal(const EntityHandle handle, const uint32_t componentId)
{
	std::vector<std::pair<uint32_t, uint32_t>>& entityComponents = HandleToEntity(handle);
	for (uint32_t destIndex = 0; destIndex < entityComponents.size(); destIndex++)
		if (componentId == entityComponents[destIndex].first)
		{
			DeleteComponent(entityComponents[destIndex].first, entityComponents[destIndex].second);
			size_t srcIndex = entityComponents.size() - 1;
			entityComponents[destIndex] = entityComponents[srcIndex];
			entityComponents.pop_back();
			return true;
		}
	return false;
}

inline BaseComponent* ECS::GetComponentInternal(std::vector<std::pair<uint32_t, uint32_t>>& entityComponents, std::vector<uint8_t>& array, const uint32_t componentId)
{
	for (uint32_t i = 0; i < entityComponents.size(); i++)
		if (componentId == entityComponents[i].first)
			return reinterpret_cast<BaseComponent*>(&array[entityComponents[i].second]);
	return nullptr;
}