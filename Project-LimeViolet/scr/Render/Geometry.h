#pragma once
#include <vector>
#include "RawGeometry.h"

class Geometry //todo remove materials from geomtry
{
private:
	void* _vertexBuffer;
	std::vector<void*> _indexBuffer;
public:
	Geometry(void*& vertex, std::vector<void*>& indices);
	~Geometry();

	void* GetVertexBuffer() const;
	const std::vector<void*>& GetIndexBuffer() const;
};
