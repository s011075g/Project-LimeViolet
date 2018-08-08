#pragma once
#include "IO/World/ConfigFileReader.h"

class Application
{
private:
	Config *& _config;
public:
	Application(Config *& config);
	~Application();

	void Start();
	void GameLoop();
	void CleanUp();
};

