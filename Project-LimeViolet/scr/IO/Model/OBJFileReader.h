#pragma once
#include <vector>
#include <map>
#include "../../Maths/MathStructs.h"
#include "../../Common/ObjectVertex.h"

class OBJFileReader
{
private:
	struct CompareCharValues //Used to compare values in a map
	{
		bool operator()(const char* left, const char* right) const;
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
	struct MTLData
	{
		Float3 ambient;
		Float3 diffuse;
		Float3 specular;
		float specularPower;
		float transparency;
		void* fileLocationDiffuse;
		void* fileLocationNormal;
		void* fileLocationSpecular;
	};

	struct Mtl //Used to store a Mtl files data
	{
		Mtl() = default;
		std::map<const char*, MTLData, CompareCharValues> materials;
	};

	struct Packed //Used for packing all data so that it uses one indice
	{
		Float3 vertex;
		Float3 normal;
		Float2 uv;
		Float4 tangent;
		Packed() = default;
		Packed(Float3 vertex, Float3 normal, Float2 uv);
		bool operator<(const Packed& right) const; //Map compare
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
	//Packs the data to use only one indice list. Puts them in to a map which seperates them depending on their material
	static void PackData(Obj * obj, std::map<unsigned short, std::vector<ObjectVertex>>& outVertex, std::map<unsigned short, std::vector<unsigned short>>& outIndices, std::vector<const char*>& outMaterial);
	//Calculates tangents to map a normal map on to the objects surface
	static void CalculateTangents(std::map<unsigned short, std::vector<ObjectVertex>>& vertex, std::map<unsigned short, std::vector<unsigned short>>& indices, std::vector<const char*> materials);
	
	//Used to read faces from Obj files
	static void ReadFace(const char* line, const char* material, Obj*& data);
	//Fill out missing information
	static void FillData(Obj*& obj);
	//Finds the same data in a map
	static bool FindSameData(std::map<Packed, unsigned short>& map, Packed& data, unsigned short& outIndex);
};

