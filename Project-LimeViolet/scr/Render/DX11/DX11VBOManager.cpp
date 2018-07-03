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
		index.push_back(static_cast<void*>(BufferData(i, D3D11_BIND_INDEX_BUFFER)));
	return new Geometry(vertex, index);
}

void DX11VBOManager::DeleteVBO(Geometry*& geometry)
{
	static_cast<ID3D11Buffer*>(geometry->GetVertexBuffer())->Release();
	for (auto buffer : geometry->GetIndexBuffer())
		static_cast<ID3D11Buffer*>(buffer)->Release();
	delete geometry;
	geometry = nullptr;
}
