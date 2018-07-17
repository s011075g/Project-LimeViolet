#pragma once
#include "../../Common/Material.h"
#include <vector>

class MTLFileReader
{
public:
	static std::vector<std::pair<const char*, Material*>>* ReadFile(const char* fileLocation);
private:
	static char* ExtractString(std::string string, const int start);
};