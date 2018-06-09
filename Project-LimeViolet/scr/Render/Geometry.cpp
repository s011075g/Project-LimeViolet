#include "Geometry.h"

Geometry::Geometry(void*& vertex, std::vector<void*>& indices, std::vector<Material*>& materials)
	: _vertexBuffer(vertex), _indexBuffer(indices), _materials(materials)
{ }

Geometry::~Geometry()  //Cleans up materials
{
	for(Material* m : _materials)
		delete m;
}

void* Geometry::GetVertexBuffer() const
{
	return _vertexBuffer;
}

const std::vector<void*>& Geometry::GetIndexBuffer() const
{
	return _indexBuffer;
}
