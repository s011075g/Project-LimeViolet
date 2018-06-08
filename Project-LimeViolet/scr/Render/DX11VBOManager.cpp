#include "DX11VBOManager.h"

DX11VBOManager::DX11VBOManager(ID3D11Device* device)
	: _device(device)
{ }


DX11VBOManager::~DX11VBOManager()
{
	_device = nullptr;
}

Geometry* DX11VBOManager::VBOGeometry(RawGeometry* geometry)
{
	void* vertex = static_cast<void*>(BufferData(geometry->vertex, D3D11_BIND_VERTEX_BUFFER));
	std::vector<void*> index;
	for (auto i : geometry->indices)
		index.push_back(static_cast<void*>(BufferData(i.second, D3D11_BIND_INDEX_BUFFER)));
	return new Geometry(vertex, index, geometry->materials);
}
