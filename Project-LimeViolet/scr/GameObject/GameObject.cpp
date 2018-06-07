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

