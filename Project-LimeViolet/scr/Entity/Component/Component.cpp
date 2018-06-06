#include "Component.h"
#include <array>

//Awful public static variable, but quick and easy to do
static uint32_t ComponentId = 0;

uint32_t BaseComponent::NextId()
{
	return ComponentId++;
}

//Creates the component at a certain point in memory. As we're putting it in an array to hold it all
template<typename Component>
uint32_t ComponentCreate(std::vector<uint8_t>& memory, const EntityHandler entity, BaseComponent* comp)
{
	const uint32_t index = memory.size();
	memory.resize(index + Component::SIZE);
	Component* component = new(&memory[index]/*Tells new a point in memory where to put it*/) Component(*static_cast<Component*>(comp));
	component->entity = entity;
	return index;
}

//Used to free up memory rather than the traditional delete
template<typename Component>
void ComponentFree(BaseComponent* comp)
{
	Component* component = static_cast<Component*>(comp);
	component->~Component(); //Frees Component not memory, handled elsewhere
}

template<typename T>
const uint32_t Component<T>::ID(NextId());

template<typename T>
const size_t Component<T>::SIZE(sizeof(T));

//Link the function pointer to a function
template<typename T>
const ComponentCreateFunction Component<T>::CREATE_FUNCTION(ComponentCreate<T>);

//Link the function pointer to a function
template<typename T>
const ComponentFreeFunction Component<T>::FREE_FUNCTION(ComponentFree<T>);