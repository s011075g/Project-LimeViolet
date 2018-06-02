#include "Utilities.h"
#include <Windows.h>
#include <iostream>

void Utilities::Write(const char* ptr, const LEVEL level)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), level);
	std::cout << ptr << std::endl;
}

void Utilities::PauseConsole()
{
	std::cin.get();
}

void Utilities::HideConsole()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

void Utilities::ShowConsole()
{
	ShowWindow(GetConsoleWindow(), SW_SHOW);
}

bool Utilities::IsConsoleVisible()
{
	return IsWindowVisible(GetConsoleWindow()) != FALSE;
}
