#include "RenderMeshSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/RenderableMeshComponent.h"
#include "../Components/MeshRenderComponent.h"

RenderMeshSystem::RenderMeshSystem()
	: BaseSystem()
{
	AddComponentType(TransformComponent::ID);
	AddComponentType(RenderableMeshComponent::ID);
	AddComponentType(MeshRenderComponent::ID);
}

RenderMeshSystem::~RenderMeshSystem()
{ }

void RenderMeshSystem::UpdateComponents(const float delta, BaseComponent** components)
{

}
