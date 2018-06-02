#pragma once

namespace Utilities
{
	enum LEVEL : unsigned short
	{
		NORMAL_LEVEL = 15u, WARNING_LEVEL = 14u, ERROR_LEVEL = 12u, EXTREME_LEVEL = 203u
	};

	void Write(const char* ptr, LEVEL level);
	void PauseConsole();

	void HideConsole();
	void ShowConsole();
	bool IsConsoleVisible();
};

