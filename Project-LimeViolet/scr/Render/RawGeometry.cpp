#include "RawGeometry.h"

RawGeometry::RawGeometry(std::vector<ObjectVertex>& vertex, std::vector<std::vector<unsigned short>>& indices)
	: vertex(vertex), indices(indices)
{ }
