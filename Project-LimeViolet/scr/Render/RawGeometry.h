#pragma once
#include <vector> 
#include <map>
#include "../Common/ObjectVertex.h"
#include "../Common/Material.h"
struct RawGeometry
{
public:
	std::vector<ObjectVertex> vertex;
	std::map<unsigned short, std::vector<unsigned short>> indices;
	std::vector<Material*> materials; //TODO make a material manager to hold all the materials so that if objects use the same material, it can be reused to save memory

	RawGeometry(std::vector<ObjectVertex>& vertex, std::map<unsigned short, std::vector<unsigned short>>& indices, std::vector<Material*>& materials);
	~RawGeometry() = default;
};