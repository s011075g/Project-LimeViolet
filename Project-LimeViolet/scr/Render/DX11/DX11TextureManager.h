#pragma once
#include "../ITextureManager.h"
#include <d3d11.h>
#include <string>
#include <map>

class DX11TextureManager : public ITextureManager
{
private:
	ID3D11Device * _device;
	std::map<std::string, Texture> _loadedImages;
public:
	DX11TextureManager(ID3D11Device * device);
	~DX11TextureManager();
	Texture GetTexture(const char* fileLocation) override;
	void UnloadTexture(const char* fileLocation) override;
protected:
	Texture LoadTexture(std::string fileLocation) const;
};

