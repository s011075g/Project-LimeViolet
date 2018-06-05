#pragma once
#include <vector>
typedef void* EntityHandler;
typedef unsigned int uint32; 
typedef unsigned char uint8;

struct BasicComponent;

typedef uint32 (*ComponentCreateFunction)(std::vector<uint8*> memory, EntityHandler entity, BasicComponent* comp);
typedef void (*ComponentFreeFunction)(BasicComponent* comp);

struct BasicComponent
{
	static uint32 NextId();
	EntityHandler entity;
};

template<typename T>
struct Component : BasicComponent
{
	static const ComponentCreateFunction CREATE_FUNCTION;
	static const ComponentFreeFunction FREE_FUNCTION;
	static const uint32 ID;
	static const size_t SIZE;
};

struct Example : Component<Example>
{
	float x, y;
};