#pragma once
#include <vector>
#include "Component/IComponent.h"
#include "Component/Transform.h"

class GameObject
{
private:
	std::vector<IComponent *> _components;

	Transform * _transform;

	//If object can move at runtime
	bool _isStatic;
public:
	GameObject();
	~GameObject();

	void Start();
	void Update();
	void Draw();

	template<class T> inline bool AddComponent();
	template<class T> inline bool RemoveComponent();
	template<class T> inline T * GetComponent();

	Transform * GetTransform() const;

	void SetStatic(const bool isStatic);
	constexpr bool IsStatic() const;
};

template <class T>
bool GameObject::AddComponent()
{
	for (std::vector<IComponent*>::value_type component : _components)
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
		if (T * c = dynamic_cast<T *>(component))
			return c;
	return nullptr;
}
