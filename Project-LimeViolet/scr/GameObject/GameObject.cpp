#include "GameObject.h"

GameObject::GameObject() 
	: _isStatic(false)
{
	_transform = new Transform(this);
	_components.push_back(_transform);
}

GameObject::~GameObject()
{
	for (auto component : _components)
		delete component;
}

void GameObject::Start()
{
	for(auto component : _components)
		component->Start();
}

void GameObject::Update()
{
	for (auto component : _components)
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
	for(std::vector<IComponent*>::value_type component : _components)
	{
		if (T* c = dynamic_cast<T *>(component))
			return false;
	}
	_components.push_back(new T(this));
	return true;
}

template <class T>
bool GameObject::RemoveComponent()
{
	bool found = false;
	std::vector<IComponent*>::value_type result;
	for (auto component : _components)
		if (T * c = dynamic_cast<T *>(component))
		{
			found = true;
			result = component;
			break;
		}
	if (found)
	{
		_components.erase(result);
		delete result;
	}
	return found;
}

template <class T>
T * GameObject::GetComponent()
{
	for (auto component : _components)
		if (T const * c = dynamic_cast<T *>(component))
			return c;
	return nullptr;
}
