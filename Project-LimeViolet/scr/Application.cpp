#include "Application.h"
#include "Common/Utilities.h"

Application::Application(Config *& config, IRender*& render)
	: _config(config), _render(render)
{ }

Application::~Application()
{
	CleanUp();
}

int Application::Start()
{
	RECT rc = { 0, 0, _config.windowSize.x, _config.windowSize.y };
	const char* windowTitle = _config.windowTitle.c_str();
	if (FAILED(_render->InitWindow(rc, windowTitle)))
	{
		Utilities::Write("FAILED: Created Window", Utilities::LEVEL::EXTREME_LEVEL);
		Utilities::CloseConsole();
		return -1;
	}
	else
		Utilities::Write("SUCCESS: Created Window", Utilities::LEVEL::NORMAL_LEVEL);

	if (FAILED(_render->InitRenderer()))
	{
		Utilities::Write("FAILED: Render Init", Utilities::LEVEL::EXTREME_LEVEL);
		Utilities::CloseConsole();
		return -1;
	}
	else
		Utilities::Write("SUCCESS: Render Init", Utilities::LEVEL::NORMAL_LEVEL);
	return 0;
}

int Application::GameLoop()
{
	//Run gameloop
	return 0;
}

void Application::CleanUp()
{
	//Clean up
	delete _config;
	delete _render;
}
