#include "Component.h"
#include <array>

//Awful public static variable, but quick and easy to do
static uint32 componentID = 0;

uint32 BasicComponent::NextId()
{
	return componentID++;
}

//Creates the component at a certain point in memory. As we're putting it in an array to hold it all
template<typename Component>
uint32 ComponentCreate(std::vector<uint8*> memory, EntityHandler entity, BasicComponent* comp)
{
	uint32 index = memory.size();
	memory.resize(index + Component::SIZE); //request memory
	//new keyword can take in a memory address
	Component* component = new(&memory[index]) Component(*static_cast<Component*>(comp));
	component->entity = entity;
	return index;
}

//Used to free up memory rather than the traditional delete
template<typename Component>
void ComponentFree(BasicComponent* comp)
{
	Component* component = static_cast<Component*>(comp);
	component->~Component();
}

template<typename T>
const uint32 Component<T>::ID(NextId());

template<typename T>
const size_t Component<T>::SIZE(sizeof(T));

//Link the function pointer to a function
template<typename T>
const ComponentCreateFunction Component<T>::CREATE_FUNCTION(ComponentCreate<T>);

//Link the function pointer to a function
template<typename T>
const ComponentFreeFunction Component<T>::FREE_FUNCTION(ComponentFree<T>);