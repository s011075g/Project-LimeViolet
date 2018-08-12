#include "Utilities.h"
#include <Windows.h>
#include <iostream>

void Utilities::Write(const char* ptr, const LEVEL level)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), level);
	std::cout << ptr << std::endl;
	if (level == LEVEL::EXTREME_LEVEL)
	{
		ShowConsole();
		MessageBox(nullptr, L"Error - Check console", L"Error", MB_OK);
	}
}

void Utilities::Write(const char* ptr)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7u);
	std::cout << ptr << std::endl;
}
#if defined(DEBUG) || defined(_DEBUG)
//Stream for the console
FILE* stream = nullptr;
#endif
void Utilities::CreateConsole()
{
#if defined(DEBUG) || defined(_DEBUG)
	if (stream)
		return;
	AllocConsole();
	freopen_s(&stream, "CONOUT$", "w", stdout);
#endif
}

void Utilities::CloseConsole()
{
#if defined(DEBUG) || defined(_DEBUG)
	if (!stream)
		return;
	fclose(stream);
#endif
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

bool Utilities::IsDebug()
{
#if defined(DEBUG) || defined(_DEBUG)
	return true;
#else
	return false;
#endif
}
