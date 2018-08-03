#include "RenderMeshSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/RenderableMeshComponent.h"
#include "../Components/MaterialComponent.h"

RenderMeshSystem::RenderMeshSystem(IRender*& render)
	: BaseSystem(), _render(render)
{
	AddComponentType(TransformComponent::ID);
	AddComponentType(RenderableMeshComponent::ID);
	AddComponentType(MaterialComponent::ID);
}

RenderMeshSystem::~RenderMeshSystem()
{ }

void RenderMeshSystem::UpdateComponents(const float delta, BaseComponent** components)
{
	_render->DrawObject(static_cast<TransformComponent*>(components[0]), static_cast<RenderableMeshComponent*>(components[1]), static_cast<MaterialComponent*>(components[2]));
}
