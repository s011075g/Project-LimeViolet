#include "Component.h"
#include <array>

//Awful public static variable, but quick and easy to do
static uint32_t ComponentId = 0;
//Returns the next Entity ID
uint32_t BaseComponent::NextId()
{
	return ComponentId++;
}
