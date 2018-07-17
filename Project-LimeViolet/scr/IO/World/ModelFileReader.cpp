#include "ModelFileReader.h"
#include "../JsonFileReader.h"

ModelJson ModelFileReader::ReadFile(const char* fileLocation)
{
	Json::Value root = JsonFileReader::ReadFile(fileLocation);
	ModelJson model;
	model.modelLocation = root["Model"]["Location"].asString();
	model.materialLocations = JsonFileReader::ReadStringArray(root["Model"]["Materials"]["Location"]);
	model.materialNames = JsonFileReader::ReadStringArray(root["Model"]["Materials"]["Names"]);
	return model;
}
