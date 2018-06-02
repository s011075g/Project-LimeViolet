#include "GameObject.h"

GameObject::GameObject() 
	: _isStatic(false)
{
	_transform = new Transform();
	_components.push_back(_transform);
}

GameObject::~GameObject()
{
	for (auto component : _components)
		delete component;
}

void GameObject::Start()
{
	for (std::_Vector_iterator<IComponent *>::value_type component = _components.begin(); component != _components.end(); ++component)
		component->Start();
}

void GameObject::Update()
{
	for (std::_Vector_iterator<IComponent *>::value_type component = _components.begin(); component != _components.end(); ++component)
		component->Update();
}

Transform * GameObject::GetTransform() const
{
	return _transform;
}

void GameObject::SetStatic(const bool isStatic)
{
	_isStatic = isStatic;
}

constexpr bool GameObject::IsStatic() const
{
	return _isStatic;
}

template <class T>
bool GameObject::AddComponent()
{
	for (std::_Vector_iterator<IComponent *>::value_type component = _components.begin(); component != _components.end(); ++component)
	{
		if (T* c = dynamic_cast<T *>(component))
			return false;
	}
	_components.push_back(new T());
	return true;
}

template <class T>
bool GameObject::RemoveComponent()
{
	bool found = false;
	std::_Vector_iterator<IComponent *>::value_type component;
	for (component = _components.begin(); component != _components.end(); ++component)
		if (T * c = dynamic_cast<T *>(component))
		{
			found = true;
			break;
		}
	if (found)
	{
		_components.erase(component);
		delete component;
	}
	return found;
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
