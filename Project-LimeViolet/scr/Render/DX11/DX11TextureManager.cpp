#include "DX11TextureManager.h"
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>

DX11TextureManager::DX11TextureManager(ID3D11Device* device)
	: _device(device)
{ }

DX11TextureManager::~DX11TextureManager()
{
	for (auto i : _loadedImages)
		if (i.second) static_cast<ID3D11ShaderResourceView*>(i.second)->Release();
}

Texture DX11TextureManager::GetTexture(const char* fileLocation)
{
	std::string path(fileLocation);
	if (path.empty()) 
		return nullptr;
	Texture result = _loadedImages[path];
	if (result)
		return result;
	return _loadedImages[path] = LoadTexture(path);
}

void DX11TextureManager::UnloadTexture(const char* fileLocation)
{
	std::string path(fileLocation);
	if(_loadedImages[path])
		static_cast<ID3D11ShaderResourceView*>(_loadedImages[path])->Release();
	_loadedImages[path] = nullptr;
}

Texture DX11TextureManager::LoadTexture(std::string fileLocation)
{
	ID3D11ShaderResourceView* result(nullptr);
	std::wstring file(fileLocation.begin(), fileLocation.end());
	file = file.substr(file.size() - 3);
	HRESULT hr;
	if (file.compare(L"dds") == 0 || file.compare(L"DDS") == 0)
		hr = DirectX::CreateDDSTextureFromFile(_device, file.c_str(), nullptr, &result);
	else
		hr = DirectX::CreateWICTextureFromFile(_device, file.c_str(), nullptr, &result);
	if FAILED(hr)
		return nullptr;
	return result;
}
