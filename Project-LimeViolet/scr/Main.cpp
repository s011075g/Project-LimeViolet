#include "Common/Utilities.h"
#include <iostream>
#include "Render/IRender.h"
#include "Render/DX11/DX11Render.h"
//#include "Render/Vulkan/VulkanRender.h"
#include "IO/World/ConfigFileReader.h"
#include "ECS/ECS.h"

#include "Systems/RenderMeshSystem.h"

//#define MODEL_TESTING

#ifdef MODEL_TESTING
#include "IO/Model/OBJFileReader.h"
#include "Render/DX11/DX11VBOManager.h"
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
	RECT rc = { 0, 0, config.windowSize.x, config.windowSize.y };
	const char* windowTitle = config.windowTitle.c_str();
	if (FAILED(render->InitWindow(rc, windowTitle)))
	{
		Utilities::Write("FAILED: Created Window", Utilities::LEVEL::EXTREME_LEVEL);
		delete render;
		Utilities::CloseConsole();
		return -1;
	}
	else
		Utilities::Write("SUCCESS: Created Window", Utilities::LEVEL::NORMAL_LEVEL);

	if (FAILED(render->InitRenderer()))
	{
		Utilities::Write("FAILED: Render Init", Utilities::LEVEL::EXTREME_LEVEL);
		delete render;
		Utilities::CloseConsole();
		return -1;
	}
	else
		Utilities::Write("SUCCESS: Render Init", Utilities::LEVEL::NORMAL_LEVEL);

#ifdef MODEL_TESTING
	Utilities::Write("Running tests...", Utilities::LEVEL::NORMAL_LEVEL);

	std::chrono::time_point<std::chrono::system_clock> tMid;
	auto tStart = std::chrono::system_clock::now();

	//READ OBJ
	RawGeometry* ptr = OBJFileReader::ReadFile("resources/earth.obj", true);

	tMid = std::chrono::system_clock::now();

	Geometry* geometry = render->LoadRawGeometry(ptr);
	render->FreeGeometry(geometry);

	auto tEnd = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsedTime = tMid - tStart;
	Utilities::Write(std::to_string(elapsedTime.count()).c_str());
	elapsedTime = tEnd - tStart;
	Utilities::Write(std::to_string(elapsedTime.count()).c_str());
#endif
///ECS 
	ECS ecs;
	//Create Components
	TransformComponent transformComponent = {};
	transformComponent.position = Float3(0, 0, 0);

	CameraComponent cameraComponent = {};
	cameraComponent.up = Float4(0, 1, 0, 0);
	cameraComponent.eye = Float3(0, 0, 1);
	const Color4 color(0.0f, 1.0f, 0.42f, 0.25f);
	cameraComponent.clearColor = color;
	cameraComponent.farPlane = 100.0f;
	cameraComponent.nearPlane = 0.1f;
	//Create Entity
	EntityHandle entity = ecs.MakeEntity(transformComponent, cameraComponent);
	//Create Systems
	RenderMeshSystem renderSystem = RenderMeshSystem(render);
	SystemList renderList = SystemList();
	renderList.AddSystem(renderSystem);

	//Test Camera
	render->SetActiveCamera(ecs.GetComponent<CameraComponent>(entity));

	while (render->ShouldExit())
	{
		//System update

		render->Update();
		render->DrawStart();
		ecs.UpdateSystems(renderList, 0);
		render->DrawEnd();
	}

	ecs.RemoveEntity(entity);
	delete render;
	Utilities::CloseConsole();
	return 0;
}
