#pragma once
typedef void* Texture;

class ITextureManager
{
public:
	ITextureManager();
	virtual ~ITextureManager();

	virtual Texture GetTexture(const char* fileLocation) = 0;
protected:
	virtual Texture LoadTexture(const char* fileLocation) = 0;
};