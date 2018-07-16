#pragma once
#include <string>
#include <vector>

struct ModelJson
{
	std::string modelLocation;
	std::vector<std::string> materials; 
};

class ModelFileReader
{
public:
	static ModelJson ReadFile(const char* fileLocation);
};
