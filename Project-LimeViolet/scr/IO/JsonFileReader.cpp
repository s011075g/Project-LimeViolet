#include "JsonFileReader.h"
#include <fstream>

Json::Value JsonFileReader::ReadFile(const char* fileLocation)
{
	std::ifstream stream(fileLocation, std::ifstream::binary);
	if (!stream.good())
		throw std::exception(std::string("Cannot open file " + std::string(fileLocation)).c_str());
	Json::Value value;
	stream >> value;
	stream.close();
	return value;
}

Json::Value JsonFileReader::WriteFloat2(const Float2& value)
{
	Json::Value result(Json::arrayValue);
	result.append(value.x);
	result.append(value.y);
	return result;
}

Json::Value JsonFileReader::WriteFloat3(const Float3& value)
{
	Json::Value result(Json::arrayValue);
	result.append(value.x);
	result.append(value.y);
	result.append(value.z);
	return result;
}

Json::Value JsonFileReader::WriteFloat4(const Float4& value)
{
	Json::Value result(Json::arrayValue);
	result.append(value.x);
	result.append(value.y);
	result.append(value.z);
	result.append(value.w);
	return result;
}

Int2 JsonFileReader::ReadInt2(Json::Value value)
{
	return Int2(value[0].asInt(), value[1].asInt());
}

Float2 JsonFileReader::ReadFloat2(Json::Value value)
{
	return Float2(value[0].asFloat(), value[1].asFloat());
}

Float3 JsonFileReader::ReadFloat3(Json::Value value)
{
	return Float3(value[0].asFloat(), value[1].asFloat(), value[2].asFloat());
}

Float4 JsonFileReader::ReadFloat4(Json::Value value)
{
	return Float4(value[0].asFloat(), value[1].asFloat(), value[2].asFloat(), value[3].asFloat());
}
