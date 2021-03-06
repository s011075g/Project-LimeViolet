#include "ECS.h"
#include <algorithm>

ECS::~ECS()
{
	for(std::map<uint32_t, std::vector<uint8_t>>::iterator it = _components.begin(); it != _components.end(); ++it)
	{
		size_t typeSize = BaseComponent::GetTypeSize(it->first);
		ComponentFreeFunction free = BaseComponent::GetTypeFreeFunction(it->first);
		for (size_t i = 0; i < it->second.size(); i += typeSize)
			free(reinterpret_cast<BaseComponent*>(&it->second[i]));
	}
	for (size_t i = 0; i < _entities.size(); i++)
		delete _entities[i];
}

EntityHandle ECS::MakeEntity(BaseComponent** entityComponents, const uint32_t* componentIds, const size_t numComponents)
{
	auto newEntity = new std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>();
	EntityHandle handle = static_cast<EntityHandle>(newEntity);
	for(uint32_t i = 0; i < numComponents; i++)
	{
		if(!BaseComponent::IsTypeValid(componentIds[i]))
		{
			Utilities::Write("ECS Component Type not valid: " + componentIds[i], Utilities::LEVEL::LEVEL_ERROR);
			delete newEntity;
			return nullptr;
		}
		AddComponentInternal(handle, newEntity->second, componentIds[i], entityComponents[i]);
	}

	newEntity->first = static_cast<uint32_t>(_entities.size());
	_entities.push_back(newEntity);
	return handle;
}

void ECS::RemoveEntity(const EntityHandle handle)
{
	std::vector<std::pair<uint32_t, uint32_t>>& entity = HandleToEntity(handle);
	for (uint32_t i = 0; i < entity.size(); i++)
		DeleteComponent(entity[i].first, entity[i].second);

	uint32_t destination = HandleToEntityIndex(handle);
	size_t source = _entities.size() - 1;
	delete _entities[destination];
	_entities[destination] = _entities[source];
	_entities[destination]->first = destination;
	_entities.pop_back();
}

void ECS::UpdateSystems(SystemList& systems, const float delta)
{
	std::vector<BaseComponent*> componentParam;
	std::vector<std::vector<uint8_t>*> componentArrays;
	for(uint32_t i = 0; i < systems.Size(); i++)
	{
		const std::vector<uint32_t>& componentTypes = systems[i]->GetComponentTypes();
		if (componentTypes.size() == 1)
		{
			size_t typeSize = BaseComponent::GetTypeSize(componentTypes[0]);
			std::vector<uint8_t>& array = _components[componentTypes[0]];
			for (size_t j = 0; j < array.size(); j += typeSize)
			{
				BaseComponent* component = reinterpret_cast<BaseComponent*>(&array[j]);
				systems[i]->UpdateComponents(delta, &component);
			}
		}
		else
			UpdateSystemWithMultipleComponents(i, systems, delta, componentTypes, componentParam, componentArrays);
	}
}

void ECS::DeleteComponent(const uint32_t componentId, const uint32_t index)
{
	std::vector<uint8_t>& array = _components[componentId];
	ComponentFreeFunction free = BaseComponent::GetTypeFreeFunction(componentId);
	size_t typeSize = BaseComponent::GetTypeSize(componentId);
	size_t srcIndex = array.size() - typeSize;

	BaseComponent* destComponent = reinterpret_cast<BaseComponent*>(&array[index]);
	BaseComponent* srcComponent = reinterpret_cast<BaseComponent*>(&array[srcIndex]);
	free(destComponent);

	if (index == srcIndex)
	{
		array.resize(srcIndex);
		return;
	}
	memcpy(destComponent, srcComponent, typeSize);

	std::vector<std::pair<uint32_t, uint32_t>>& srcComponents = HandleToEntity(srcComponent->entity);
	for (uint32_t i = 0; i <srcComponents.size(); i++)
		if (componentId == srcComponents[i].first && srcIndex == srcComponents[i].second)
		{
			srcComponents[i].second = index;
			break;
		}
	array.resize(srcIndex);
}

void ECS::UpdateSystemWithMultipleComponents(const uint32_t index, SystemList& systems, const float delta, const std::vector<uint32_t>& componentTypes, std::vector<BaseComponent*>& componentParam, std::vector<std::vector<uint8_t>*>& componentArrays)
{
	const std::vector<uint32_t>& componentFlags = systems[index]->GetComponentFlags();

	componentParam.resize(std::max(componentParam.size(), componentTypes.size()));
	componentArrays.resize(std::max(componentArrays.size(), componentTypes.size()));

	for (uint32_t i = 0; i < componentTypes.size(); i++)
		componentArrays[i] = &_components[componentTypes[i]];

	uint32_t minSizeIndex = FindLeastCommonComponent(componentTypes, componentFlags);
	size_t typeSize = BaseComponent::GetTypeSize(componentTypes[minSizeIndex]);
	
	std::vector<uint8_t>& array = *componentArrays[minSizeIndex];

	for(size_t i = 0; i < array.size(); i += typeSize)
	{
		componentParam[minSizeIndex] = reinterpret_cast<BaseComponent*>(&array[i]);
		std::vector<std::pair<uint32_t, uint32_t>>& entityComponents = HandleToEntity(componentParam[minSizeIndex]->entity);

		bool isValid = true;
		for (uint32_t j = 0; j < componentTypes.size(); j++)
		{
			if (j == minSizeIndex)
				continue;

			componentParam[j] = GetComponentInternal(entityComponents, *componentArrays[j], componentTypes[j]);
			if(componentParam[j] == nullptr && (componentFlags[j] & BaseSystem::FLAG_OPTIONAL) == 0)
			{
				isValid = false;
				break;
			}
		}
		if (isValid)
			systems[index]->UpdateComponents(delta, &componentParam[0]);
	}
}

uint32_t ECS::FindLeastCommonComponent(const std::vector<uint32_t>& componentTypes, const std::vector<uint32_t>& componentFlags)
{
	size_t minSize = static_cast<size_t>(-1);
	uint32_t minIndex = static_cast<uint32_t>(-1);
	for(uint32_t i = 0; i < componentTypes.size(); i++) 
	{
		if ((componentFlags[i] & BaseSystem::FLAG_OPTIONAL) != 0)
			continue;
		const size_t typeSize = BaseComponent::GetTypeSize(componentTypes[i]);
		const size_t size = _components[componentTypes[i]].size() / typeSize;
		if(size <= minSize)
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
