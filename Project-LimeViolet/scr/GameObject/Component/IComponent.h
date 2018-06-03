#pragma once

class GameObject;

class IComponent
{
protected:
	GameObject * _gameObject;
public:
	IComponent(GameObject* const gameObject);
	virtual ~IComponent();

	virtual void Start() = 0;
	virtual void Update() = 0;

	GameObject* GetGameObject() const;
};
