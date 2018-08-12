#include "ArrayBitmap.h"
#include <cassert>
#include <stb/stb_image.h>

ArrayBitmap::ArrayBitmap(int32_t width, int32_t height)
	: _width(width), _height(height)
{
	assert(width > 0 && height > 0);
	_pixels = static_cast<int32_t*>(malloc(GetPixelsSize()));
}

ArrayBitmap::ArrayBitmap(int32_t width, int32_t height, int32_t* pixels)
	: _width(width), _height(height)
{
	assert(width > 0 && height > 0);
	assert(pixels != nullptr);
	uintptr_t size = GetPixelsSize();
	pixels = static_cast<int32_t*>(malloc(size));
	memcpy_s(pixels, size, pixels, size);
}

ArrayBitmap::ArrayBitmap(int32_t width, int32_t height, int32_t* pixels, int32_t offsetX, int32_t offsetY, int32_t rowOffset)
	: _width(width), _height(height)
{
	assert(width > 0 && height > 0);
	assert(pixels != nullptr);
	assert(offsetX > 0 && offsetY > 0 && rowOffset > 0);
	uintptr_t size = GetPixelsSize();
	pixels = static_cast<int32_t*>(malloc(size));
	int32_t* pixelsSrc = pixels + offsetY + offsetX * rowOffset;
	for (uintptr_t i = 0; i < static_cast<uintptr_t>(_height); i++, pixels += _width, pixelsSrc += rowOffset)
		memcpy_s(pixels, static_cast<uintptr_t>(_width), pixelsSrc, static_cast<uintptr_t>(_width));
}

ArrayBitmap::ArrayBitmap(const std::string& fileLocation)
{
	Load(fileLocation);
}

ArrayBitmap::~ArrayBitmap()
{
	if (_pixels)
		free(static_cast<void*>(_pixels));
	_pixels = nullptr;
}

bool ArrayBitmap::Load(const std::string& fileLocation)
{
	int32_t texWidth, texHeight, bytesPerPixel;
	uint8_t* data = stbi_load(fileLocation.c_str(), &texWidth, &texHeight, &bytesPerPixel, 4);
	if (!data)
		return false;

	if (texWidth == _width && texHeight == _height)
		memcpy(_pixels, data, GetPixelsSize());
	else {
		_width = texWidth;
		_height = texHeight;
		if(_pixels)
			free(static_cast<void*>(_pixels));
		_pixels = static_cast<int32_t*>(malloc(GetPixelsSize()));
		memcpy(_pixels, data, GetPixelsSize());
	}

	stbi_image_free(data);
	return true;
}

bool ArrayBitmap::Save(const std::string& fileLocation)
{
	//todo 
	return false;
}

void ArrayBitmap::Clear(int32_t color)
{
	memset(_pixels, color, GetPixelsSize());
}

int32_t ArrayBitmap::GetWidth() const
{
	return _width;
}

int32_t ArrayBitmap::GetHeight() const
{
	return _height;
}

int32_t ArrayBitmap::Get(int32_t x, int32_t y) const
{
	assert(x >= 0 && x < _width);
	assert(y >= 0 && y < _height);
	return _pixels[x + y * _width];
}

void ArrayBitmap::Set(int32_t x, int32_t y, int32_t pixel)
{
	assert(x >= 0 && x < _width);
	assert(y >= 0 && y < _height);
	_pixels[x + y * _width] = pixel;
}

int32_t* ArrayBitmap::GetPixelArray() const
{
	return _pixels;
}

uintptr_t ArrayBitmap::GetPixelsSize() const
{
	return static_cast<uintptr_t>(_width * _height) * sizeof(_pixels[0]);
}
