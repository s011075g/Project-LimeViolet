#include "RawGeometry.h"

RawGeometry::RawGeometry(std::vector<ObjectVertex>& vertex, std::map<unsigned short, std::vector<unsigned short>>& indices, std::vector<RawMaterial*>& materials)
	: vertex(vertex), indices(indices), materials(materials)
{ }
