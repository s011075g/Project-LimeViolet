#include "MATFileReader.h"
#include "../JsonFileReader.h"
std::vector<std::pair<const char*, Material*>>* MATFileReader::ReadFile(const char* fileLocation)
{
	std::vector<std::pair<const char*, Material*>>* data = new std::vector<std::pair<const char*, Material*>>();
	Json::Value root = JsonFileReader::ReadFile(fileLocation);

	int count = root["Count"].asInt();
	for(int i = 1; i <= count; i++)
		data->push_back(ReadMaterial(root["Materials"][std::to_string(i)]));

	return data;
}

std::pair<const char*, Material*> MATFileReader::ReadMaterial(Json::Value value)
{
	std::pair<const char*, Material*> result = std::pair<const char*, Material*>();
	result.first = value["Name"].asString().c_str();
	result.second = new Material();
	result.second->diffuseTexture = (texture)value["DiffuseTexture"].asString().c_str();
	result.second->diffuseColor = Color4(JsonFileReader::ReadFloat4(value["DiffuseColor"]));
	result.second->specularTexture = (texture)value["SpecularTexture"].asString().c_str();
	result.second->specularColor = Color3(JsonFileReader::ReadFloat3(value["SpecularColor"]));
	result.second->specularPower = value["SpecularPower"].asFloat();
	result.second->normalTexture = (texture)value["NormalTexture"].asString().c_str();
	result.second->occlusionTexture = (texture)value["OcclusionTexture"].asString().c_str();

	return result;
}
