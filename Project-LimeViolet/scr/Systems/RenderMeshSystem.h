#pragma once
#include "../ECS/System.h"
#include "../Render/IRender.h"

class RenderMeshSystem : public BaseSystem
{
public:
	RenderMeshSystem(IRender*& render);
	~RenderMeshSystem() = default;;
	void UpdateComponents(const float delta, BaseComponent** components) override;
private:
	IRender * & _render;
};