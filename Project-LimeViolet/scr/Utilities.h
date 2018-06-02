#pragma once

namespace Utilities
{
	enum LEVEL : unsigned short
	{
		NORMAL_LEVEL = 15u, WARNING_LEVEL = 14u, ERROR_LEVEL = 12u, EXTREME_LEVEL = 203u
	};

	//Used to write to console. Level shows the message in a different colour.
	void Write(const char* ptr, LEVEL level);
	//Used to write to console. Colour will be default.
	void Write(const char* ptr);
	//Causes the console to pause by requesting input.
	void PauseConsole();
	//Hide the console window.
	void HideConsole();
	//Show the console window.
	void ShowConsole();
	//Querry if the console window is showing.
	bool IsConsoleVisible();
};

