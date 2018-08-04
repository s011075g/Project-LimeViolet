#pragma once
#include <vector> 
#include "../Common/ObjectVertex.h"

//Holds raw values for geometry
struct RawGeometry
{
	std::vector<ObjectVertex> vertex;
	std::vector<std::vector<unsigned short>> indices;

	RawGeometry(std::vector<ObjectVertex>& vertex, std::vector<std::vector<unsigned short>>& indices);
	~RawGeometry() = default;
};