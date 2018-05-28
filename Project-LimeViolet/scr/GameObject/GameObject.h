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

	bool AddComponent(IComponent * component);
	template<class T> bool RemoveComponent();
	template<class T> T * GetComponent();

	Transform * GetTransform() const;

	void SetStatic(const bool isStatic);
	constexpr bool IsStatic() const;
};
