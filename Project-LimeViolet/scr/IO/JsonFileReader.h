#pragma once
#include <json/json.h>
#include "../Maths/MathStructs.h"

class JsonFileReader
{
public:
	static Json::Value ReadFile(const char* fileLocation);
	
	//Write functions

	static Json::Value WriteFloat2(const Float2& value);
	static Json::Value WriteFloat3(const Float3& value);
	static Json::Value WriteFloat4(const Float4& value);
	//Read functions

	static Int2 ReadInt2(Json::Value value);
	static Float2 ReadFloat2(Json::Value value);
	static Float3 ReadFloat3(Json::Value value);
	static Float4 ReadFloat4(Json::Value value);
};

