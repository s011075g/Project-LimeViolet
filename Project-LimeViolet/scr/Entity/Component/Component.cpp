#include "Component.h"
#include <array>

//Awful public static variable
static uint32 componentID = 0;

uint32 BasicComponent::NextID()
{
	return componentID++;
}

template<typename Component>
uint32 ComponentCreate(std::vector<uint8*> memory, EntityHandler entity, BasicComponent* comp)
{
	uint32 index = memory.size();
	memory.resize(index + Component::SIZE);
	//new keyword can take in a memory address
	Component* component = new(&memory[index]) Component(*static_cast<Component*>(comp));
	component->entity = entity;
	return index;
}

template<typename Component>
void ComponentFree(BasicComponent* comp)
{
	Component* component = static_cast<Component*>(comp);
	component->~Component();
}

template<typename T>
const uint32 Component<T>::ID(NextID());

template<typename T>
const size_t Component<T>::SIZE(sizeof(T));

template<typename T>
const ComponentCreateFunction Component<T>::CREATE_FUNCTION(ComponentCreate<T>);

template<typename T>
const ComponentFreeFunction Component<T>::FREE_FUNCTION(ComponentFree<T>);