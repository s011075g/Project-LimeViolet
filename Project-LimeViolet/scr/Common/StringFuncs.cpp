#include "StringFuncs.h"
#include <fstream>
#include <sstream>
#include "scr/IO/EntityReader.h"

std::vector<std::string> StringFuncs::Split(const std::string& s, char delim)
{
	std::vector<std::string> elems;

	const char* cstr = s.c_str();
	unsigned int strLength = (unsigned int)s.length();
	unsigned int start = 0;
	unsigned int end = 0;

	while (end <= strLength) 
	{
		while (end <= strLength) 
		{
			if (cstr[end] == delim)
				break;
			end++;
		}
		elems.push_back(s.substr(start, end - start));
		start = end + 1;
		end = start;
	}
	return elems;
}

std::string StringFuncs::GetFilePath(const std::string& fileName)
{
	const char* cstr = fileName.c_str();
	auto strLength = static_cast<unsigned int>(fileName.length());
	unsigned int end = strLength - 1;

	while (end != 0) 
	{
		if (cstr[end] == '/')
			break;
		end--;
	}

	if (end == 0)
		return fileName;
	const unsigned int start = 0;
	end = end + 1;
	return fileName.substr(start, end - start);
}

bool StringFuncs::LoadTextFileWithIncludes(std::string& output, const std::string& fileName, const std::string& includeKeyword)
{
	std::ifstream file;
	file.open(fileName.c_str());

	std::string filePath = GetFilePath(fileName);
	std::stringstream ss;
	std::string line;

	if (file.is_open()) 
	{
		while (file.good()) 
		{
			getline(file, line);
			if (line.find(includeKeyword) == std::string::npos) 
				ss << line << "\n";
			else 
			{
				std::string includeFileName = Split(line, ' ')[1];
				includeFileName = includeFileName.substr(1, includeFileName.length() - 2);

				std::string toAppend;
				LoadTextFileWithIncludes(toAppend, filePath + includeFileName, includeKeyword);
				ss << toAppend.c_str() << "\n";
			}
		}
	}
	else 
	{
		Utilities::Write("Unable to load file with includes.",Utilities::ERROR_LEVEL);
		return false;
	}

	output = ss.str();
	return true;
}
