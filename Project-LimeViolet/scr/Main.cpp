#include "Common/Utilities.h"
#include <windows.h>
#include <chrono>
#include <iostream>
#include "IO/Model/OBJFileReader.h"
#include "Render/IRender.h"
#include "Render/DX11Render.h"

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

	delete render;
	Utilities::CloseConsole();
	return 0;
}