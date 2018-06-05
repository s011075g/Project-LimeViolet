#pragma once
#include <vector> 
#include <map>
#include "../Common/ObjectVertex.h"
#include "../Common/Material.h"

class Geometry //Geometry current holds raw model data, future plan to make it hold pointers to VBO data
{
private:
	std::map<unsigned short, std::vector<ObjectVertex>> _vertex;
	std::map<unsigned short, std::vector<unsigned short>> _indices;
	std::vector<Material*> _materials; //TODO make a material manager to hold all the materials so that if objects use the same material, it can be reused to save memory
public:
	Geometry() = default;
	Geometry(std::map<unsigned short, std::vector<ObjectVertex>>& vertex, std::map<unsigned short, std::vector<unsigned short>>& indices, std::vector<Material*>& materials);
	~Geometry();
};
