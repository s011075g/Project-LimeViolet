#include "Common/Utilities.h"
#include <windows.h>
#include <chrono>
#include <iostream>
#include "IO/Model/OBJFileReader.h"
#include "Render/IRender.h"
#include "Render/DX11/DX11Render.h"
#include "GameObject/GameObject.h"
#include <string>
#include "Render/DX11/DX11VBOManager.h"
#include "Render/Vulkan/VulkanRender.h"

//#define MODEL_TESTING

int main()
{
#if defined(DEBUG) || defined(_DEBUG)
	Utilities::ShowConsole();
#else
	//Utilities::HideConsole();
#endif

	Utilities::CreateConsole();
	Utilities::ShowConsole();
	Utilities::Write("Running...");

	IRender* render = new DX11Render();
	RECT rc = { 0, 0, 1280, 720 };
	const char* title = "Window";
	if (FAILED(render->InitWindow(rc, title)))
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

	auto tStart = std::chrono::system_clock::now();

	//READ OBJ
	RawGeometry* ptr = OBJFileReader::ReadFile("earth.obj");

	auto tMid = std::chrono::system_clock::now();

	Geometry* geometry = render->LoadRawGeometry(ptr);
	render->FreeGeometry(geometry);

	auto tEnd = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsedTime = tMid - tStart;
	Utilities::Write(std::to_string(elapsedTime.count()).c_str());
	elapsedTime = tEnd - tStart;
	Utilities::Write(std::to_string(elapsedTime.count()).c_str());
#endif

	//Test Camera
	GameObject* camera = new GameObject();
	camera->AddComponent<Camera>();
	Camera* cam = camera->GetComponent<Camera>();
	const Color4 color(0.0f, 1.0f, 0.42f, 0.25f);
	cam->SetClearColor(color);
	cam->SetUp(Float3(0, 1, 0));

	render->SetActiveCamera(cam);

	while (render->ShouldExit())
	{
		render->Update();

		render->Draw();
	}

	delete camera;
	delete render;
	Utilities::CloseConsole();
	return 0;
}
