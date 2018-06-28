#pragma once
#include <vector>
#include "RawGeometry.h"

class Geometry //todo remove materials from geomtry
{
private:
	void* _vertexBuffer;
	std::vector<void*> _indexBuffer;
	std::vector<RawMaterial*> _materials; //TODO make a material manager to hold all the materials so that if objects use the same material, it can be reused to save memory
public:
	Geometry(void*& vertex, std::vector<void*>& indices, std::vector<RawMaterial*>& materials);
	~Geometry();

	void* GetVertexBuffer() const;
	const std::vector<void*>& GetIndexBuffer() const;
	const std::vector<RawMaterial*> GetMaterials() const;
};
