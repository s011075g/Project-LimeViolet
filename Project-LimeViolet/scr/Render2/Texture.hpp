#pragma once
#include "RenderDevice.h"
#include "ArrayBitmap.h"
#include "DDSTexture.h"

class Texture
{
	RenderDevice* _device;
	uint32_t _texId;
	uint32_t _width;
	uint32_t _height;
	bool _compressed;
	bool _mipmaps;
public:
	inline Texture(RenderDevice& device, const ArrayBitmap& texData, enum RenderDevice::PixelFormat internalPixelFormat, bool generateMipmaps, bool shouldCompress);
	inline Texture(RenderDevice& device, const DDSTexture& ddsTexture);
	inline ~Texture();
	inline uint32_t GetID() const;
	inline uint32_t GetWidth() const;
	inline uint32_t GetHeight() const;
	inline bool IsCompressed() const;
	inline bool HasMipmaps() const;
private:
	Texture(const Texture& other) = delete;
	void operator=(const Texture& other) = delete;
};

inline Texture::Texture(RenderDevice& device, const ArrayBitmap& texData, enum RenderDevice::PixelFormat internalPixelFormat, bool generateMipmaps, bool shouldCompress)
	:	_device(&device), _texId(_device->CreateTexture2D(texData.GetWidth(), texData.GetHeight(), texData.GetPixelArray(), RenderDevice::FORMAT_RGBA, internalPixelFormat, generateMipmaps, shouldCompress)),
		_width(texData.GetWidth()), _height(texData.GetHeight()), _compressed(shouldCompress), _mipmaps(generateMipmaps)
{ }

inline Texture::Texture(RenderDevice& device, const DDSTexture& ddsTexture)
	: _device(&device), _texId(_device->CreateDDSTexture2D(ddsTexture.GetWidth(), ddsTexture.GetHeight(), ddsTexture.GetBuffer(), ddsTexture.GetFourCC(), ddsTexture.GetMipMapCount())),
		_width(ddsTexture.GetWidth()), _height(ddsTexture.GetHeight()), _compressed(true), _mipmaps(ddsTexture.GetMipMapCount() > 1)
{ }

inline Texture::~Texture()
{
	_texId = _device->ReleaseTexture2D(_texId);
}

inline uint32_t Texture::GetID() const
{
	return _texId;
}

inline uint32_t Texture::GetWidth() const
{
	return _width;
}

inline uint32_t Texture::GetHeight() const
{
	return _height;
}

inline bool Texture::IsCompressed() const
{
	return _compressed;
}

inline bool Texture::HasMipmaps() const
{
	return _mipmaps;
}
