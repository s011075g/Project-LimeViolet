#pragma once
#include "Component.h"

class BaseSystem
{
public:
	enum
	{
		FLAG_OPTIONAL = 1
	};
	BaseSystem();
	virtual ~BaseSystem() = default;
	virtual void UpdateComponents(const float delta, BaseComponent** components) = 0;
	const std::vector<uint32_t>& GetComponentTypes() const;
	const std::vector<uint32_t>& GetComponentFlags() const;
	bool IsValid() const;
protected:
	void AddComponentType(uint32_t componentType, uint32_t componentFlag);
private:
	std::vector<uint32_t> _componentTypes;
	std::vector<uint32_t> _componentFlags;
};

class SystemList
{
public:
	bool AddSystem(BaseSystem& system);
	bool RemoveSystem(BaseSystem& system);

	uint32_t Size() const;
	BaseSystem* operator[](const uint32_t& index);
private:
	std::vector<BaseSystem*> _systems;
};