#include "Geometry.h"

Geometry::Geometry(void*& vertex, std::vector<void*>& indices)
	: _vertexBuffer(vertex), _indexBuffer(indices)
{ }

Geometry::~Geometry()  //Cleans up materials
{}

void* Geometry::GetVertexBuffer() const
{
	return _vertexBuffer;
}

const std::vector<void*>& Geometry::GetIndexBuffer() const
{
	return _indexBuffer;
}
