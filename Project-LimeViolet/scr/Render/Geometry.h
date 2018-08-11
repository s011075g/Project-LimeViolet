#pragma once
#include <vector>

class Geometry
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
