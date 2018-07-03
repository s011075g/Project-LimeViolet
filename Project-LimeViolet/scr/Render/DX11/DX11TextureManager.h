#pragma once
#include "../ITextureManager.h"
#include <d3d11.h>
#include <string>
#include <map>

class DX11TextureManager : public ITextureManager
{
private:
	ID3D11Device * _device;
	std::map<std::string, texture> _loadedImages;
public:
	DX11TextureManager(ID3D11Device * device);
	~DX11TextureManager();
	texture GetTexture(const char* fileLocation) override;
	void UnloadTexture(const char* fileLocation) override;
protected:
	texture LoadTexture(std::string fileLocation) const;
};

