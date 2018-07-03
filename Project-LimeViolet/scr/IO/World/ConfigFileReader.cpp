#include "ConfigFileReader.h"
#include "../JsonFileReader.h"
Config ConfigFileReader::ReadFile(const char* fileLocation)
{
	Json::Value root = JsonFileReader::ReadFile(fileLocation);
	Config configuration;
	configuration.windowSize = JsonFileReader::ReadInt2(root["Window"]["Size"]);
	configuration.windowTitle = root["Window"]["Title"].asString();
	return configuration;
}
