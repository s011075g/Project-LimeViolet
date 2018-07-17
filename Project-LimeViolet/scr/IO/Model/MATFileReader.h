#pragma once
#include "../../Common/Material.h"
#include <vector>
#include <json/json.h>
class MATFileReader
{
public:
	static std::vector<std::pair<const char*, Material*>>* ReadFile(const char* fileLocation);
private:
	static std::pair<const char*, Material*> ReadMaterial(Json::Value value);
};