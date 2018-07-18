#pragma once
#include "../../Maths/MathStructs.h"
#include <string>

struct Config
{
	Int2 windowSize;
	std::string windowTitle;

	std::string gameScene;
};

class ConfigFileReader
{
public:
	static Config ReadFile(const char* fileLocation);
};
