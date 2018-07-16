#include "ModelFileReader.h"
#include "JsonFileReader.h"

ModelJson ModelFileReader::ReadFile(const char* fileLocation)
{
	Json::Value root = JsonFileReader::ReadFile(fileLocation);
	ModelJson model;
	return model;
}
