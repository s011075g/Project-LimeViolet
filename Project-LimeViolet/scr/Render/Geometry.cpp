#include "Geometry.h"

Geometry::Geometry(void*& vertex, std::vector<std::pair<void*, int>>& indices)
	: _vertexBuffer(vertex), _indexBuffer(indices)
{ }

Geometry::~Geometry()  //Cleans up materials
{}

void* Geometry::GetVertexBuffer() const
{
	return _vertexBuffer;
}

const std::vector<std::pair<void*, int>>& Geometry::GetIndexBuffer() const
{
	return _indexBuffer;
}
