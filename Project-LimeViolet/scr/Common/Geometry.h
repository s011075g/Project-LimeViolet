#pragma once
#include <vector>
#include "Material.h"
#include <map>
#include "ObjectVertex.h"

class Geometry
{
private:
	std::map<unsigned short, std::vector<ObjectVertex>> _vertex;
	std::map<unsigned short, std::vector<unsigned short>> _indices;
	std::vector<Material*> _materials;
public:
	Geometry() = default;
	Geometry(std::map<unsigned short, std::vector<ObjectVertex>>& vertex, std::map<unsigned short, std::vector<unsigned short>>& indices, std::vector<Material*>& materials);
	~Geometry();
};