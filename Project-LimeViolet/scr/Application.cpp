#include "Application.h"
#include "Common/Utilities.h"

Application::Application(Config & config, IRender*& render)
	: _config(config), _render(render)
{ }

Application::~Application()
{
	CleanUp();
}

int Application::Start() const
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
	uint32_t fps = 0;
	float fpsTimeCounter = 0.0f;
	float updateTimer = 1.0f;
	float lastTime = GetTickCount() / 1000.0f; //Converts the given milliseconds to seconds
	const float frameTime = 1.0f / 60.0f;
	while(_render->ShouldExit())
	{
		float currentTime = GetTickCount() / 1000.0f;
		float passedTime = currentTime - lastTime;
		lastTime = currentTime;

		fpsTimeCounter += passedTime;
		updateTimer += passedTime;

		if (fpsTimeCounter >= 1.0)
		{
			float msPerFrame = -1.0f;
			if (fps != 0)
				msPerFrame = 1000.0f / static_cast<float>(fps);
			Utilities::Write(std::string(std::to_string(msPerFrame) + " ms (" + std::to_string(fps) + " fps)").c_str());
			fpsTimeCounter = 0.0f;
			fps = 0;
		}

		bool shouldRender = false;
		while (updateTimer >= frameTime)
		{
			_render->Update();
			_ecs.UpdateSystems(_mainSystems, frameTime);
			updateTimer -= frameTime;
			shouldRender = true;
		}
		if (shouldRender)
		{
			_render->DrawStart();
			_ecs.UpdateSystems(_renderPipeline, frameTime);
			_render->DrawEnd();
			fps++;
		}
		else
			Sleep(1);
	}
	return 0;
}

void Application::CleanUp()
{
	//Clean up
	delete _render;
}

void Application::SetMainSystems(SystemList& systems)
{
	_mainSystems = systems;
}

void Application::SetRenderPipeline(SystemList& systems)
{
	_renderPipeline = systems;
}
