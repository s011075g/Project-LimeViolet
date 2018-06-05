#include "Common/Utilities.h"
#include <chrono>
#include <iostream>
#include "IO/Model/OBJFileReader.h"

int main()
{
#if defined(DEBUG) || defined(_DEBUG)
	Utilities::ShowConsole();
#else
	Utilities::HideConsole();
#endif

	Utilities::Write("Running...", Utilities::NORMAL_LEVEL);
	Utilities::PauseConsole();

	/*Utilities::Write("Running tests...");
	auto tStart = std::chrono::system_clock::now();
	
	//READ OBJ
	Geometry* ptr = OBJFileReader::Instance()->ReadFile("earth.obj");
	
	auto tEnd = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsedTime = tEnd - tStart;
	std::cout << elapsedTime.count() << " : " << ptr << std::endl;
	Utilities::PauseConsole();
	delete  ptr;*/
	return 0;
}
