#pragma once
#include "../../Maths/MathStructs.h"
#include <string>

struct Config
{
	Int2 windowSize;
	std::string windowTitle;
};

class ConfigFileReader
{
public:
	static Config ReadFile(const char* fileLocation);
};
