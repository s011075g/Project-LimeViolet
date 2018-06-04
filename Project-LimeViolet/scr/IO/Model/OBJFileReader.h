#pragma once
#include <vector>
#include <map>
#include "../../Maths/MathStructs.h"

class OBJFileReader
{
private:
	struct CompareCharValues //Used to compare values in a map
	{
		bool operator()(const char* left, const char* right) const {
			return std::strcmp(left, right) < 0;
		}
	};

	struct Obj //Used to store the Obj files data
	{
		Obj() = default;
		std::vector<Float3> vertices;
		std::map<const char*, std::vector<unsigned short>, CompareCharValues> indicesVertices;
		std::vector<Float3> normals;
		std::map<const char*, std::vector<unsigned short>, CompareCharValues> indicesNormals;
		std::vector<Float2> uvs;
		std::map<const char*, std::vector<unsigned short>, CompareCharValues> indicesUvs;
		std::vector<const char*> materialPaths;
	};

	struct Mtl //Used to store a Mtl files data
	{
		Mtl() = default;
		std::map<const char*,Float3, CompareCharValues> ambient;
		std::map<const char*, Float3, CompareCharValues> diffuse;
		std::map<const char*, Float3, CompareCharValues> specular;
		std::map<const char*, float, CompareCharValues> shine;
		std::map<const char*, float, CompareCharValues> transparency;
		std::map<const char*, const char*, CompareCharValues> fileLocationDiffuse;
		std::map<const char*, const char*, CompareCharValues> fileLocationNormal;
		std::map<const char*, const char*, CompareCharValues> fileLocationSpecular;
	};

public:
	~OBJFileReader();
	//Singleton
	static OBJFileReader * Instance();
	//Read Obj model file
	void ReadFile(const char* fileLocation);
private:
	OBJFileReader() = default;
	//Used to read Obj file extensions
	Obj* LoadObj(const char* fileLocation) const;
	//Used to read Mtl file extensions
	Mtl* LoadMtl(const char* fileLocation) const;
	//Used to read faces from Obj files
	static void ReadFace(const char* line, const char* material, Obj*& data);
};

