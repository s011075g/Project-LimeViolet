#pragma once
#include "../Common/Texture.h"

class ITextureManager
{
public:
	ITextureManager();
	virtual ~ITextureManager();

	virtual Texture GetTexture(const char* fileLocation) = 0;
	virtual void UnloadTexture(const char* fileLocation) = 0;
};