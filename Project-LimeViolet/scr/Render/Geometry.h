#pragma once
#include <vector>
#include "RawGeometry.h"

class Geometry //todo remove materials from geomtry
{
private:
	void* _vertexBuffer;
	std::vector<std::pair<void*, int>> _indexBuffer;
public:
	Geometry(void*& vertex, std::vector<std::pair<void*, int>>& indices);
	~Geometry();

	void* GetVertexBuffer() const;
	const std::vector<std::pair<void*, int>>& GetIndexBuffer() const;
};
