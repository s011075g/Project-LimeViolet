#pragma once
#include <vector>
typedef void* EntityHandler;

struct BaseComponent;

typedef uint32_t(*ComponentCreateFunction)(std::vector<uint8_t>& memory, EntityHandler entity, BaseComponent* comp);
typedef void (*ComponentFreeFunction)(BaseComponent* comp);

struct BaseComponent
{
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