#pragma once
#include <string>
#include <vector>

struct ModelJson
{
	std::string modelLocation;
	std::vector<std::string> materialLocations;
	std::vector<std::string> materialNames;
};

class ModelFileReader
{
public:
	static ModelJson ReadFile(const char* fileLocation);
};
