#include "Common/Utilities.h"
#include <windows.h>
#include <chrono>
#include <iostream>
#include "IO/Model/OBJFileReader.h"
#include "Render/IRender.h"
#include "Render/DX11Render.h"
#include "GameObject/GameObject.h"

int main()
{
#if defined(DEBUG) || defined(_DEBUG)
	Utilities::ShowConsole();
#else
	//Utilities::HideConsole();
#endif
	Utilities::Write("Running...", Utilities::NORMAL_LEVEL);
	//Utilities::PauseConsole();

	Utilities::Write("Running tests...");
	auto tStart = std::chrono::system_clock::now();

	//READ OBJ
	Geometry* ptr = OBJFileReader::ReadFile("earth.obj");

	auto tEnd = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsedTime = tEnd - tStart;
	std::cout << elapsedTime.count() << " : " << ptr << std::endl;
	Utilities::PauseConsole();
	delete  ptr;

	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	Utilities::CreateConsole();
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	Utilities::Write("Running...");

	IRender* render = new DX11Render();
	RECT rc = { 0, 0, 1280, 720 };
	if(FAILED(render->InitWindow(hInstance, nCmdShow, rc)))
		Utilities::Write("FAILED: Created Window");
	else
		Utilities::Write("SUCCESS: Created Window");

	if(FAILED(render->InitRenderer()))
		Utilities::Write("FAILED: Render Init");
	else
		Utilities::Write("SUCCESS: Render Init");

	//Test Camera
	GameObject* camera = new GameObject();
	camera->AddComponent<Camera>();
	Camera* cam = camera->GetComponent<Camera>();
	const Color color{0.0f, 1.0f, 0.42f, 1.0f};
	cam->SetClearColor(color);

	render->SetActiveCamera(cam);

	// Main message loop for testing
	MSG msg = { nullptr };

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			render->Draw();
		}
	}

	

	delete camera;
	delete render;
	Utilities::CloseConsole();
	return 0;
}