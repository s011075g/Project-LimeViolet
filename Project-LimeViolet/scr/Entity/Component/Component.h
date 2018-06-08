#pragma once
#include <vector>
//Means the Component doesn't have to know anything about the entity
typedef void* EntityHandler;

//Forward declare
struct BaseComponent;

//Typedef function pointers so code is more readable
typedef uint32_t(*ComponentCreateFunction)(std::vector<uint8_t>& memory, EntityHandler entity, BaseComponent* comp);
typedef void (*ComponentFreeFunction)(BaseComponent* comp);

struct BaseComponent
{
	//Gets the next Component ID so we can then identify it
	static uint32_t NextId();
	EntityHandler entity = nullptr;
};

template<typename T>
struct Component : BaseComponent
{
	static const ComponentCreateFunction CREATE_FUNCTION;
	static const ComponentFreeFunction FREE_FUNCTION;
	static const uint32_t ID;
	static const size_t SIZE;
};

//Creates the component at a certain point in memory. As we're putting it in an array to hold it all
template<typename Component>
uint32_t ComponentCreate(std::vector<uint8_t>& memory, const EntityHandler entity, BaseComponent* comp)
{
	const uint32_t index = memory.size();
	memory.resize(index + Component::SIZE); //Extends the memory pool to add the new Component
	Component* component = new(&memory[index]) Component(*static_cast<Component*>(comp));
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

//Sets it to the next possible ID
template<typename T>
const uint32_t Component<T>::ID(NextId());

//Calculates the Components size so memory required can be easily know
template<typename T>
const size_t Component<T>::SIZE(sizeof(T));

//Link the function pointer to a function
template<typename T>
const ComponentCreateFunction Component<T>::CREATE_FUNCTION(ComponentCreate<T>);

//Link the function pointer to a function
template<typename T>
const ComponentFreeFunction Component<T>::FREE_FUNCTION(ComponentFree<T>);