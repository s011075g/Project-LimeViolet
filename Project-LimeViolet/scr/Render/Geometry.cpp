#include "Geometry.h"

Geometry::Geometry(void*& vertex, std::vector<void*>& indices, std::vector<RawMaterial*>& materials)
	: _vertexBuffer(vertex), _indexBuffer(indices), _materials(materials)
{ }

Geometry::~Geometry()  //Cleans up materials
{
	for(RawMaterial* m : _materials)
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

const std::vector<RawMaterial*> Geometry::GetMaterials() const
{
	return _materials;
}
