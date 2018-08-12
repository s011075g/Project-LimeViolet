#pragma once
#include <string>
#include <vector>

namespace StringFuncs
{
	static std::vector<std::string> Split(const std::string& s, char delim);
	static std::string GetFilePath(const std::string& fileName);
	static bool LoadTextFileWithIncludes(std::string& output, const std::string& fileName, const std::string& includeKeyword);
};

