#include "GameObject.h"

GameObject::GameObject()
{
	_transform = new Transform();
	AddComponent(_transform);
}

GameObject::~GameObject()
{
	for (auto component : _components)
		delete component;
}

bool GameObject::AddComponent(IComponent * component)
{
	_components.resize(_components.size() + 1);
	_components.push_back(component);
	component->Start();
	return true; //TODO: Check if gameobject already has component
}

Transform * GameObject::GetTransform() const
{
	return _transform;
}

template <class T>
bool GameObject::RemoveComponent()
{
	for (std::_Vector_iterator<IComponent *>::value_type component = _components.begin(); component != _components.end(); ++component)
	{
		if (T * c = dynamic_cast<T *>(component))
		{
			delete c;
			return true;
		}
	}
	return false;
}

template <class T>
T * GameObject::GetComponent()
{
	for (std::_Vector_iterator<IComponent *>::value_type component = _components.begin(); component != _components.end(); ++component)
	{
		if (T const * c = dynamic_cast<T *>(component))
		{
			return c;
		}
	}
	return nullptr;
}
