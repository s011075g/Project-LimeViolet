#include "IComponent.h"

IComponent::IComponent(GameObject* const gameObject)
	:_gameObject(gameObject)
{ }

IComponent::~IComponent()
{ }

GameObject* IComponent::GetGameObject() const
{
	return _gameObject;
}
