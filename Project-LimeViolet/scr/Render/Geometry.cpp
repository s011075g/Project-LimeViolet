#include "Geometry.h"

Geometry::Geometry(std::map<unsigned short, std::vector<ObjectVertex>>& vertex,
	std::map<unsigned short, std::vector<unsigned short>>& indices, std::vector<Material*>& materials)
	: _vertex(vertex), _indices(indices), _materials(materials)
{ }

Geometry::~Geometry() //Cleans up materials
{
	for (Material* m : _materials) 
		delete m;
}
