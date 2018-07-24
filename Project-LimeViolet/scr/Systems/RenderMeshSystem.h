#pragma once
#include "../ECS/System.h"
class RenderMeshSystem : public BaseSystem
{
public:
	RenderMeshSystem();
	~RenderMeshSystem();
	void UpdateComponents(const float delta, BaseComponent** components) override;
};

