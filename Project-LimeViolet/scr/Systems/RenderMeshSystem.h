#pragma once
#include "../ECS/System.h"
#include "../Render/IRender.h"
class RenderMeshSystem : public BaseSystem
{
public:
	RenderMeshSystem(const IRender*& render);
	~RenderMeshSystem();
	void UpdateComponents(const float delta, BaseComponent** components) override;
private:
	const IRender * & _render;
};

