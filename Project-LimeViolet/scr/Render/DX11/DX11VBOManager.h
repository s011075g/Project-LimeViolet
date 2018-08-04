#pragma once
#include "../IVBOManager.h"
#include <d3d11.h>

class DX11VBOManager : public IVBOManager
{
private:
	ID3D11Device * _device;
public:
	DX11VBOManager(ID3D11Device* device);
	~DX11VBOManager();
	Geometry* VBOGeometry(RawGeometry* geometry) override;
	void DeleteVBO(Geometry*& geometry) override;
private:
	template<class T>
	inline ID3D11Buffer* BufferData(std::vector<T> data, const UINT bindFlag);
};

template <class T>
inline ID3D11Buffer* DX11VBOManager::BufferData(std::vector<T> data, const UINT bindFlag)
{
	D3D11_BUFFER_DESC buffer_desc;
	ZeroMemory(&buffer_desc, sizeof(buffer_desc));
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.ByteWidth = static_cast<unsigned int>(sizeof(T) * data.size());
	buffer_desc.BindFlags = bindFlag;
	buffer_desc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = &data[0];
	ID3D11Buffer* pBuffer(nullptr);
	_device->CreateBuffer(&buffer_desc, &initData, &pBuffer);
	return pBuffer;
}