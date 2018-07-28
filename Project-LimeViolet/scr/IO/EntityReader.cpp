#include "EntityReader.h"
#include "JsonFileReader.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
static TransformComponent* CreateTransformComponent(Json::Value value);
static CameraComponent* CreateCameraComponent(Json::Value value);

EntityHandle EntityReader::ReadEntity(Json::Value value, ECS* ecs)
{
	std::vector<BaseComponent*> components;
	std::vector<uint32_t> componentIDs;
	if (!value["Transform"].empty())//Transform
	{
		TransformComponent* transform = CreateTransformComponent(value);
		components.push_back(transform);
		componentIDs.push_back(transform->ID);
	}
	if(!value["Model"].empty())//Model
	{
		value["Model"]["File"].asString();
		value["Model"]["Shader"]["DX11"].asString();
		value["Model"]["Shader"]["Vulkan"].asString();
	}
	if(!value["Camera"].empty())//Camera
	{
		CameraComponent* camera = CreateCameraComponent(value);
		components.push_back(camera);
		componentIDs.push_back(camera->ID);
	}

	const uint32_t* ids = &componentIDs[0];
	return ecs->MakeEntity(&components[0], ids, components.size());
}

TransformComponent* CreateTransformComponent(Json::Value value)
{
	TransformComponent* transform = new TransformComponent();
	transform->position = JsonFileReader::ReadFloat3(value["Transform"]["Position"]);
	transform->rotation = Quaternion(JsonFileReader::ReadFloat4(value["Transform"]["Rotation"]));
	transform->scale = JsonFileReader::ReadFloat3(value["Transform"]["Scale"]);
	return transform;
}

CameraComponent* CreateCameraComponent(Json::Value value)
{
	CameraComponent* camera = new CameraComponent();
	camera->up = JsonFileReader::ReadFloat4(value["Camera"]["Up"]);
	camera->clearColor = JsonFileReader::ReadFloat4(value["Camera"]["Color"]);
	camera->fieldOfView = value["Camera"]["FOV"].asFloat();
	camera->nearPlane = value["Camera"]["Near"].asFloat();
	camera->farPlane = value["Camera"]["Far"].asFloat();
	return camera;
}