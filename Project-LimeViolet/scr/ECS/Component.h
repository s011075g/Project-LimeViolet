#pragma once
#include <vector>
//#include <tuple> //Included in <vector>

//Means the Component doesn't have to know anything about the entity
typedef void* EntityHandle;

//Forward declare
struct BaseComponent;

//Typedef function pointers so code is more readable
typedef uint32_t(*ComponentCreateFunction)(std::vector<uint8_t>& memory, EntityHandle entity, BaseComponent* comp);
typedef void (*ComponentFreeFunction)(BaseComponent* comp);

struct BaseComponent
{
public:
	static uint32_t RegisterComponentType(ComponentCreateFunction create, ComponentFreeFunction free, size_t size);
	EntityHandle entity = nullptr;

	static ComponentCreateFunction GetTypeCreateFunction(const uint32_t id);
	static ComponentFreeFunction GetTypeFreeFunction(const uint32_t id);
	static size_t GetTypeSize(const uint32_t id);
	static bool IsTypeValid(uint32_t id);
private:
	static std::vector<std::tuple<ComponentCreateFunction, ComponentFreeFunction, size_t>> _componentTypes;
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
uint32_t ComponentCreate(std::vector<uint8_t>& memory, const EntityHandle entity, BaseComponent* comp)
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

///Code dup ensure it is changed correctly
//Sets it to the next possible ID
template<typename T>
const uint32_t Component<T>::ID(RegisterComponentType(ComponentCreate<T>, ComponentFree<T>, sizeof(T)));

//Calculates the Components size so memory required can be easily know
template<typename T>
const size_t Component<T>::SIZE(sizeof(T));

//Link the function pointer to a function
template<typename T>
const ComponentCreateFunction Component<T>::CREATE_FUNCTION(ComponentCreate<T>);

//Link the function pointer to a function
template<typename T>
const ComponentFreeFunction Component<T>::FREE_FUNCTION(ComponentFree<T>);