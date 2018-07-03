#include "MTLFileReader.h"
#include <fstream>
#include <string>
#include <algorithm>

std::vector<std::pair<const char*, Material*>>* MTLFileReader::ReadFile(const char* fileLocation)
{
	std::ifstream stream; //Opens file in to a stream
	stream.open(fileLocation, std::ios_base::in);
	if (!stream.good())
		return nullptr;
	std::vector<std::pair<const char*, Material*>>* data = new std::vector<std::pair<const char*, Material*>>();
	try //Encase any errors happen while reading the file, it can safely close it.
	{
		std::string line;
		Float3 value;
		while (getline(stream, line))
		{
			if (line.empty())
				continue;
			const char c = line[0];
			if (c == '#' || c == '\n' || c == ' ')
				continue;
			const char* ptr = line.c_str();
			if (sscanf_s(ptr, "Kd %f %f %f", &value.x, &value.y, &value.z) == 3)
				(*data)[data->size() - 1].second->diffuseColor = Color4(value, 1.0f);
			else if (sscanf_s(ptr, "Ks %f %f %f", &value.x, &value.y, &value.z) == 3)
				(*data)[data->size() - 1].second->specularColor = Color3(value);
			else if (sscanf_s(ptr, "Ns %f", &value.x) == 1)
				(*data)[data->size() - 1].second->specularPower = value.x;
			else if (sscanf_s(ptr, "d %f", &value.x) == 1)
				(*data)[data->size() - 1].second->diffuseColor.a = value.x;
			else if (line.substr(0, 6).compare("newmtl") == 0) 
				data->push_back(std::pair<const char*, Material*>(ExtractString(line, 7), new Material()));
			else if (c == 'm') //This first compare is to decrease the amount of substrings needed
			{
				if (line.substr(0, 6).compare("map_Kd") == 0)
					(*data)[data->size() - 1].second->diffuseTexture = ExtractString(line, 8);
				else if (line.substr(0, 6).compare("map_Ks") == 0)
					(*data)[data->size() - 1].second->specularTexture = ExtractString(line, 8);
				else if (line.substr(0, 8).compare("map_bump") == 0)
					(*data)[data->size() - 1].second->normalTexture = ExtractString(line, 10);
				else if (line.substr(0, 7).compare("map_occ") == 0) //todo find out correct MTL value
					(*data)[data->size() - 1].second->occlusionTexture = ExtractString(line, 9);
			}
		}
	}
	catch (std::exception e)
	{
		delete data; //clean up
		stream.close();
		throw;
	}
	stream.close();
	return data;
}

char* MTLFileReader::ExtractString(std::string string, const int start)
{
	char* path = new char[string.substr(start).size() + 1]();
	strcpy_s(path, string.substr(start).size() + 1, string.substr(start).c_str());
	return path;
}
