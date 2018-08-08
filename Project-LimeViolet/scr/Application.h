#pragma once
#include "IO/World/ConfigFileReader.h"
#include "Render/IRender.h"
#include "ECS/ECS.h"

class Application
{
private:
	Config & _config;
	IRender*& _render;
	ECS _ecs;
	SystemList _mainSystems;
	SystemList _renderPipeline;
public:
	Application(Config & config, IRender*& render);
	~Application();

	int Start() const;
	int GameLoop();
	void CleanUp();

	void SetMainSystems(SystemList& systems);
	void SetRenderPipeline(SystemList& systems);

	ECS& GetECS();
};

