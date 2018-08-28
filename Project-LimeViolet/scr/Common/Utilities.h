#pragma once
namespace Utilities
{
	enum LEVEL : unsigned short
	{
		//White
		LEVEL_NORMAL = 15u, 
		//Yellow
		LEVEL_WARNING = 14u,
		//Red
		LEVEL_ERROR = 12u, 
		//Red background
		LEVEL_EXTREME = 203u
	};
	
	//Used to write to console. Level shows the message in a different colour.
	//Extreme level forces the console to show, and send a message box informing the user to check the console.
	void Write(const char* ptr, const LEVEL level);
	//Used to write to console. Colour will be default.
	void Write(const char* ptr);
	//Creates a console
	void CreateConsole();
	//Close console
	void CloseConsole();
	//Causes the console to pause by requesting input.
	void PauseConsole();
	//Hide the console window.
	void HideConsole();
	//Show the console window.
	void ShowConsole();
	//Querry if the console window is showing.
	bool IsConsoleVisible();
	//Check if game is in debug mode
	bool IsDebug();
};