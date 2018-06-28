#pragma once
#include <vector> 
#include <map>
#include "../common/Color.h"
#include "../common/ObjectVertex.h"

struct RawMaterial //Holds raw values for a material
{
	void* diffuseTexturePath;
	Color4 diffuseColor;
	void* specularTexturePath;
	Color3 specularColor;
	float specularPower;
	void* normalTexturePath;
	void* occlusionTexturePath;
};

//Holds raw values for geometry
struct RawGeometry
{
	std::vector<ObjectVertex> vertex;
	std::map<unsigned short, std::vector<unsigned short>> indices;
	std::vector<RawMaterial*> materials; //TODO make a material manager to hold all the materials so that if objects use the same material, it can be reused to save memory

	RawGeometry(std::vector<ObjectVertex>& vertex, std::map<unsigned short, std::vector<unsigned short>>& indices, std::vector<RawMaterial*>& materials);
	~RawGeometry() = default;
};