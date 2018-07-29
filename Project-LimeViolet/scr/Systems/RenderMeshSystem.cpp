#include "RenderMeshSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/RenderableMeshComponent.h"
#include "../Components/MeshRenderComponent.h"

RenderMeshSystem::RenderMeshSystem(const IRender*& render)
	: BaseSystem(), _render(render)
{
	AddComponentType(TransformComponent::ID);
	AddComponentType(RenderableMeshComponent::ID);
	AddComponentType(MeshRenderComponent::ID);
}

RenderMeshSystem::~RenderMeshSystem()
{ }

void RenderMeshSystem::UpdateComponents(const float delta, BaseComponent** components)
{
	_render->DrawObject((TransformComponent*)components[0], (RenderableMeshComponent*)components[1], (MeshRenderComponent*)components[2]);
}
