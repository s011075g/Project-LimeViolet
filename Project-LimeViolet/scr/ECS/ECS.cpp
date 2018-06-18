#include "ECS.h"
#include <algorithm>
#include "../Common/Utilities.h"

ECS::~ECS()
{
	for(std::map<uint32_t, std::vector<uint8_t>>::iterator it = _components.begin(); it != _components.end(); ++it)
	{
		size_t typeSize = BaseComponent::GetTypeSize(it->first);
		ComponentFreeFunction free = BaseComponent::GetTypeFreeFunction(it->first);
		for (uint32_t i = 0; i < it->second.size(); i += typeSize)
			free(reinterpret_cast<BaseComponent*>(&it->second[i]));
	}
	for (uint32_t i = 0; i < _entities.size(); i++)
		delete _entities[i];
}

EntityHandle ECS::MakeEntity(BaseComponent* entityComponets, const uint32_t* componentIds, const size_t numComponents)
{
	auto newEntity = new std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>();
	EntityHandle handle = static_cast<EntityHandle>(newEntity);
	for(uint32_t i = 0; i < numComponents; i++)
	{
		if(!BaseComponent::IsTypeValid(componentIds[i]))
		{
			Utilities::Write("ECS Component Type not valid: " + componentIds[i], Utilities::LEVEL::ERROR_LEVEL);
			delete newEntity;
			return nullptr;
		}
		AddComponentInternal(handle, newEntity->second, componentIds[i], &entityComponets[i]);
	}

	newEntity->first = _entities.size();
	_entities.push_back(newEntity);
	return handle;
}

void ECS::RemoveEntity(const EntityHandle handle)
{
	std::vector<std::pair<uint32_t, uint32_t>>& entity = HandleToEntity(handle);
	for (uint32_t i = 0; i < entity.size(); i++)
		DeleteComponent(entity[i].first, entity[i].second);

	uint32_t destination = HandleToEntityIndex(handle);
	uint32_t source = _entities.size() - 1;
	delete _entities[destination];
	_entities[destination] = _entities[source];
	_entities.pop_back();
}

void ECS::AddSystem(BaseSystem& system)
{
	_systems.push_back(&system);
}

void ECS::UpdateSystems(const float delta)
{
	std::vector<BaseComponent*> componentParam;
	std::vector<std::vector<uint8_t>*> componentArrays;
	for(uint32_t i = 0; i < _systems.size(); i++)
	{
		const std::vector<uint32_t>& componentTypes = _systems[i]->GetComponentTypes();
		if (componentTypes.size() == 1)
		{
			size_t typeSize = BaseComponent::GetTypeSize(componentTypes[0]);
			std::vector<uint8_t>& array = _components[componentTypes[0]];
			for (uint32_t j = 0; j < array.size(); j += typeSize)
			{
				BaseComponent* component = reinterpret_cast<BaseComponent*>(&array[j]);
				_systems[i]->UpdateComponents(delta, &component);
			}
		}
		else
			UpdateSystemsWithMultipleComponents(i, delta, componentTypes, componentParam, componentArrays);
	}
}

bool ECS::RemoveSystem(BaseSystem& system)
{
	for(uint32_t i = 0; i < _systems.size(); i++)
		if(&system == _systems[i])
		{
			_systems.erase(_systems.begin() + i);
			return true;
		}
	return false;
}

std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>* ECS::HandleToRawType(const EntityHandle handle)
{
	return static_cast<std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>*>(handle);
}

uint32_t ECS::HandleToEntityIndex(const EntityHandle handle)
{
	return HandleToRawType(handle)->first;
}

std::vector<std::pair<uint32_t, uint32_t>>& ECS::HandleToEntity(const EntityHandle handle)
{
	return HandleToRawType(handle)->second;
}

void ECS::AddComponentInternal(const EntityHandle handle, std::vector<std::pair<uint32_t, uint32_t>>& entity, const uint32_t componentId, BaseComponent* component)
{
	ComponentCreateFunction create = BaseComponent::GetTypeCreateFunction(componentId);
	std::pair<uint32_t, uint32_t> pair;
	pair.first = componentId;
	pair.second = create(_components[componentId], handle, component);
	entity.push_back(pair);
}

void ECS::DeleteComponent(const uint32_t componentId, const uint32_t index)
{
	std::vector<uint8_t>& array = _components[componentId];
	ComponentFreeFunction free = BaseComponent::GetTypeFreeFunction(componentId);
	size_t typeSize = BaseComponent::GetTypeSize(componentId);
	uint32_t srcIndex = array.size() - typeSize;

	BaseComponent* destComponent = reinterpret_cast<BaseComponent*>(&array[index]);
	BaseComponent* srcComponent = reinterpret_cast<BaseComponent*>(&array[srcIndex]);
	free(destComponent);
	
	if(index == srcIndex)
	{
		array.resize(srcIndex);
		return;
	}
	memcpy(destComponent,srcComponent, typeSize);

	std::vector<std::pair<uint32_t, uint32_t>>& srcComponents = HandleToEntity(srcComponent->entity);
	for(uint32_t i = 0; i <srcComponents.size(); i++)
		if(componentId == srcComponents[i].first && srcIndex == srcComponents[i].second)
		{
			srcComponents[i].second = index;
			break;
		}
	array.resize(srcIndex);
}

bool ECS::RemoveComponentInternal(const EntityHandle handle, const uint32_t componentId)
{
	std::vector<std::pair<uint32_t, uint32_t>>& entityComponents = HandleToEntity(handle);
	for(uint32_t destIndex = 0; destIndex < entityComponents.size(); destIndex++)
		if(componentId == entityComponents[destIndex].first)
		{
			DeleteComponent(entityComponents[destIndex].first, entityComponents[destIndex].second);
			uint32_t srcIndex = entityComponents.size() - 1;
			entityComponents[destIndex] = entityComponents[srcIndex];
			entityComponents.pop_back();
			return true;
		}
	return false;
}

BaseComponent* ECS::GetComponentInternal(std::vector<std::pair<uint32_t, uint32_t>>& entityComponents, std::vector<uint8_t>& array, const uint32_t componentId)
{
	for (uint32_t i = 0; i < entityComponents.size(); i++)
		if (componentId == entityComponents[i].first)
			return reinterpret_cast<BaseComponent*>(&array[entityComponents[i].second]);
	return nullptr;
}



void ECS::UpdateSystemsWithMultipleComponents(const uint32_t index, const float delta, const std::vector<uint32_t>& componentTypes, std::vector<BaseComponent*> componentParam, std::vector<std::vector<uint8_t>*>& componentArrays)
{
	componentParam.resize(std::max(componentParam.size(), componentTypes.size()));
	componentArrays.resize(std::max(componentArrays.size(), componentTypes.size()));

	for (uint32_t i = 0; i < componentTypes.size(); i++)
		componentArrays[i] = &_components[componentTypes[i]];

	uint32_t minSizeIndex = FindLeastCommonComponent(componentTypes);
	size_t typeSize = BaseComponent::GetTypeSize(componentTypes[minSizeIndex]);
	std::vector<uint8_t>& array = *componentArrays[minSizeIndex];
	
	for(uint32_t i = 0; i < array.size(); i += typeSize)
	{
		componentParam[minSizeIndex] = reinterpret_cast<BaseComponent*>(&array[i]);
		std::vector<std::pair<uint32_t, uint32_t>>& entityComponents = HandleToEntity(componentParam[minSizeIndex]->entity);

		bool isValid = true;
		for (uint32_t j = 0; j < componentTypes.size(); j++)
		{
			if (j == minSizeIndex)
				continue;
			componentParam[j] = GetComponentInternal(entityComponents, *componentArrays[j], componentTypes[j]);

			if(componentParam[j] == nullptr)
			{
				isValid = false;
				break;
			}
		}
		if (isValid)
			_systems[index]->UpdateComponents(delta, &componentParam[0]);
	}
}

uint32_t ECS::FindLeastCommonComponent(const std::vector<uint32_t>& componentTypes)
{
	uint32_t minSize = _components[componentTypes[0]].size() / BaseComponent::GetTypeSize(componentTypes[0]);
	uint32_t minIndex = 0;
	for(uint32_t i = 1; i < componentTypes.size(); i++)
	{
		size_t typeSize = BaseComponent::GetTypeSize(componentTypes[i]);
		uint32_t size = _components[componentTypes[i]].size() / typeSize;
		if(size < minSize)
		{
			minSize = size;
			minIndex = i;
		}
	}
	return minIndex;
}

ECS::ECS(const ECS& other)
{
	(void)other;
}

void ECS::operator=(const ECS& other) const
{
	(void)other;
}
