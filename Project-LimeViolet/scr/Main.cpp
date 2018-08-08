#include "Common/Utilities.h"
#include <iostream>
#include "Render/DX11/DX11Render.h"
#include "IO/World/ConfigFileReader.h"
#include "ECS/ECS.h"

#include "Systems/RenderMeshSystem.h"
#include "IO/Model/MTLFileReader.h"
#include "Application.h"

#define MODEL_TESTING

#ifdef MODEL_TESTING
#include "IO/Model/OBJFileReader.h"
#include <chrono>
#endif

int main()
{
#if defined(DEBUG) || defined(_DEBUG)
	Utilities::CreateConsole();
	Utilities::ShowConsole();
#else
	//Utilities::HideConsole();
#endif

	Utilities::Write("Running...");

	IRender* render = new DX11Render();
	Config config = ConfigFileReader::ReadFile("resources/Config.json");

	Application* app = new Application(config, render);
	
	app->Start();

#ifdef MODEL_TESTING
	Utilities::Write("Running tests...", Utilities::LEVEL::NORMAL_LEVEL);

	const auto tStart = std::chrono::system_clock::now();

	RawGeometry* ptr = OBJFileReader::ReadFile("resources/cube.obj", true);

	const auto tMid = std::chrono::system_clock::now();

	Geometry* geometry = render->LoadRawGeometry(ptr);

	const auto tEnd = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsedTime = tMid - tStart;
	Utilities::Write(std::to_string(elapsedTime.count()).c_str());
	elapsedTime = tEnd - tStart;
	Utilities::Write(std::to_string(elapsedTime.count()).c_str());
#endif
///ECS 
	ECS ecs;
	//Create Components
	CameraComponent cameraComponent = {};
	cameraComponent.up = Float3(0, 1, 0);
	cameraComponent.at = Float3(0, 0, 1);
	cameraComponent.eye = Float3(0, 0, 0);
	const Color4 color(0.0f, 0.0f, 0.0f, 1.0f); //0,1,0.42
	cameraComponent.fieldOfView = 70.0f;
	cameraComponent.clearColor = color;
	cameraComponent.farPlane = 100.0f;
	cameraComponent.nearPlane = 0.1f;

	TransformComponent transformComponent = {};
	transformComponent.transform = Transform(Float3(0, 0, 50.0f));

	RenderableMeshComponent renderableComponent = RenderableMeshComponent();
	renderableComponent.geometry = geometry;

	std::vector<std::pair<const char*, Material*>>* mat = MTLFileReader::ReadFile("resources/earth.mtl");
	std::vector<std::pair<const char*, Material*>>& ma = *mat;
	std::vector<Material*> materials;
	materials.push_back(ma[0].second);
	delete ma[0].first;
	delete mat;
	MaterialComponent materialComponent = MaterialComponent();
	materialComponent.materials = materials;
	materialComponent.shader = dynamic_cast<DX11Render*>(render)->CreateShader("resources/shaders/DX11/Basic.fx", VERTEX | PIXEL);

	//Create Entity
	EntityHandle camera = ecs.MakeEntity(cameraComponent);
	EntityHandle entity = ecs.MakeEntity(transformComponent, renderableComponent, materialComponent);
	for(int i = 0; i < 690; i++)
	{
		transformComponent.transform.SetPosition(Float3((float)(std::rand() % 1000 - 500) / 10.0f, (float)(std::rand() % 1000 - 500) / 10.0f, 50.0f));
		ecs.MakeEntity(transformComponent, renderableComponent, materialComponent);
	}
	//Create Systems
	RenderMeshSystem renderSystem = RenderMeshSystem(render);
	SystemList mainSystems = SystemList();
	SystemList renderPipeline = SystemList();
	renderPipeline.AddSystem(renderSystem);

	//Test Camera
	render->SetActiveCamera(ecs.GetComponent<CameraComponent>(camera));


	app->SetMainSystems(mainSystems);
	app->SetRenderPipeline(renderPipeline);
	//Gameloop
	app->GameLoop();
	delete app;
#ifdef MODEL_TESTING
	render->FreeGeometry(geometry);
	delete materials[0];
#endif
	delete static_cast<DX11Shader*>(ecs.GetComponent<MaterialComponent>(entity)->shader);
	ecs.RemoveEntity(camera);
	delete render;
	Utilities::CloseConsole();
	return 0;
}
