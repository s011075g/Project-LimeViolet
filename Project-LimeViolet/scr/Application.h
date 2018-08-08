#pragma once
#include "IO/World/ConfigFileReader.h"
#include "Render/IRender.h"

class Application
{
private:
	Config & _config;
	IRender*& _render;
public:
	Application(Config & config, IRender*& render);
	~Application();

	int Start();
	int GameLoop();
	void CleanUp();
};

