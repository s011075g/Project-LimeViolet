#include "EntityReader.h"
#include "JsonFileReader.h"
#include "../Components/Transform.h"
#include "../Components/Camera.h"
static Transform* CreateTransform(Json::Value value);
static Camera* CreateCamera(Json::Value value);

EntityHandle EntityReader::ReadEntity(Json::Value value, ECS* ecs)
{
	std::vector<BaseComponent*> components;
	std::vector<uint32_t> componentIDs;
	if (!value["Transform"].empty())//Transform
	{
		Transform* transform = CreateTransform(value);
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
		Camera* camera = CreateCamera(value);
		components.push_back(camera);
		componentIDs.push_back(camera->ID);
	}

	const uint32_t* ids = &componentIDs[0];
	return ecs->MakeEntity(&components[0], ids, components.size());
}

Transform* CreateTransform(Json::Value value)
{
	Transform* transform = new Transform();
	transform->position = JsonFileReader::ReadFloat3(value["Transform"]["Position"]);
	transform->rotation = Quaternion(JsonFileReader::ReadFloat4(value["Transform"]["Rotation"]));
	transform->scale = JsonFileReader::ReadFloat3(value["Transform"]["Scale"]);
	return transform;
}

Camera* CreateCamera(Json::Value value)
{
	Camera* camera = new Camera();
	camera->up = JsonFileReader::ReadFloat4(value["Camera"]["Up"]);
	camera->clearColor = JsonFileReader::ReadFloat4(value["Camera"]["Color"]);
	camera->fieldOfView = value["Camera"]["FOV"].asFloat();
	camera->near = value["Camera"]["Near"].asFloat();
	camera->far = value["Camera"]["Far"].asFloat();
	return camera;
}