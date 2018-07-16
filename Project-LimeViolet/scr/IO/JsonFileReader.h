#pragma once
#include <json/json.h>
#include "../Maths/MathStructs.h"

class JsonFileReader
{
public:
	//File functions

	static Json::Value ReadFile(const char* fileLocation);
	
	//Write functions

	static Json::Value WriteInt2(const Int2& value);
	static Json::Value WriteFloat2(const Float2& value);
	static Json::Value WriteFloat3(const Float3& value);
	static Json::Value WriteFloat4(const Float4& value);
	static Json::Value ReadStringArray(std::vector<std::string>& value);
	//Read functions

	static Int2 ReadInt2(Json::Value value);
	static Float2 ReadFloat2(Json::Value value);
	static Float3 ReadFloat3(Json::Value value);
	static Float4 ReadFloat4(Json::Value value);
	static std::vector<std::string> ReadStringArray(Json::Value value);
};

