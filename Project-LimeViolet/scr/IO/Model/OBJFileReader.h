#pragma once
#include <vector>
#include <map>
#include "../../Maths/MathStructs.h"
#include "../../Common/ObjectVertex.h"
#include "../../Render/Geometry.h"

class OBJFileReader
{
private:
	struct CompareCharValues 
	{
		//Used to compare values in a map rather than memory addresses
		bool operator()(const char* left, const char* right) const;
	};
	//Used to store the Obj files data
	struct Obj 
	{
		Obj() = default;
		~Obj();
		std::vector<Float3> vertices;
		std::map<const char*, std::vector<unsigned short>, CompareCharValues> indicesVertices;
		std::vector<Float3> normals;
		std::map<const char*, std::vector<unsigned short>, CompareCharValues> indicesNormals;
		std::vector<Float2> uvs;
		std::map<const char*, std::vector<unsigned short>, CompareCharValues> indicesUvs;
		std::vector<const char*> materialPaths;
	};
	//Store a materials information
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
	//Used to store a Mtl files data
	struct Mtl 
	{
		Mtl() = default;
		std::map<const char*, MTLData, CompareCharValues> materials;
		bool isNormalMap;
	};
	//Used for packing all data so that it uses one indice
	struct Packed 
	{
		Float3 vertex;
		Float3 normal;
		Float2 uv;
		Float4 tangent;
		Packed() = default;
		Packed(Float3 vertex, Float3 normal, Float2 uv);
		bool operator<(const Packed& right) const; //Map comparison
	};

public:
	//Read Obj model file
	static Geometry* ReadFile(const char* fileLocation);
private:
	//Used to read Obj file extensions
	static Obj* LoadObj(const char* fileLocation);
	//Used to read Mtl file extensions
	static Mtl* LoadMtl(const char* fileLocation);
	//Packs the data to use only one indice list. Puts them in to a map which seperates them depending on their material
	static void PackData(Obj * obj, std::map<unsigned short, std::vector<ObjectVertex>>& outVertex, std::map<unsigned short, std::vector<unsigned short>>& outIndices, std::vector<const char*>& outMaterial);
	//Calculates tangents to map a normal map on to the objects surface
	static void CalculateTangents(std::map<unsigned short, std::vector<ObjectVertex>>& vertex, std::map<unsigned short, std::vector<unsigned short>>& indices, std::vector<const char*>& materials);
	
	//Load all mtl files
	static void LoadMtlFiles(std::vector<const char*>& paths, std::vector<Mtl*> & mtls);
	//Used to read faces from Obj files
	static void ReadFace(const char* line, const char* material, Obj*& data);
	//Fill out missing information
	static void FillData(Obj*& obj);
	//Finds the same data in a map
	static bool FindSameData(std::map<Packed, unsigned short>& map, Packed& data, unsigned short& outIndex);
};
