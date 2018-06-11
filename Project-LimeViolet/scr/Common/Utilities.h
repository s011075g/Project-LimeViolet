#pragma once
namespace Utilities
{
	enum LEVEL : unsigned short
	{
		//White
		NORMAL_LEVEL = 15u, 
		//Yellow
		WARNING_LEVEL = 14u,
		//Red
		ERROR_LEVEL = 12u, 
		//Red background
		EXTREME_LEVEL = 203u
	};
	
	//Used to write to console. Level shows the message in a different colour.
	//Extreme level forces the console to show, and send a message box informing the user to check the console.
	static void Write(const char* ptr, const LEVEL level);
	//Used to write to console. Colour will be default.
	static void Write(const char* ptr);
	//Creates a console
	static void CreateConsole();
	//Close console
	static void CloseConsole();
	//Causes the console to pause by requesting input.
	static void PauseConsole();
	//Hide the console window.
	static void HideConsole();
	//Show the console window.
	static void ShowConsole();
	//Querry if the console window is showing.
	static bool IsConsoleVisible();
};