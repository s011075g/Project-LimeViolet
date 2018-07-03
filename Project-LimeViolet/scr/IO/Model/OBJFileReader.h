#pragma once
#include <vector>
#include <map>
#include "../../Maths/MathStructs.h"
#include "../../Common/ObjectVertex.h"
#include "../../Render/RawGeometry.h"

class OBJFileReader
{
private:
	
	//Used to store the Obj files data
	struct Obj 
	{
		Obj() = default;
		~Obj();
		std::vector<Float3> vertices;
		std::vector<std::vector<unsigned short>> indicesVertices;
		std::vector<Float3> normals;
		std::vector<std::vector<unsigned short>> indicesNormals;
		std::vector<Float2> uvs;
		std::vector<std::vector<unsigned short>> indicesUvs;
		std::vector<const char*> materialPaths;
		int materialCount;
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
	static RawGeometry* ReadFile(const char* fileLocation, const bool calculateTangents = false);
private:
	//Used to read Obj file extensions
	static Obj* LoadObj(const char* fileLocation);
	
	//Packs the data to use only one indice list. Puts them in to a map which seperates them depending on their material
	static void PackData(Obj * obj, std::vector<ObjectVertex>& outVertex, std::vector<std::vector<unsigned short>>& outIndices);
	//Calculates tangents to map a normal map on to the objects surface
	static void CalculateTangents(std::vector<ObjectVertex>& vertex, std::vector<std::vector<unsigned short>>& indices, const size_t materialCount);
	//Used to read faces from Obj files
	static void ReadFace(const char* line, const int materialValue, Obj*& data);
	//Fill out missing information
	static void FillData(Obj*& obj);
	//Finds the same data in a map
	static bool FindSameData(std::map<Packed, unsigned short>& map, Packed& data, unsigned short& outIndex);
};
