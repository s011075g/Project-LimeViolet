#pragma once
#include <cstdint>
#include <string>

class ArrayBitmap
{
public:
	ArrayBitmap(int32_t width = 1, int32_t height = 1);
	ArrayBitmap(int32_t width, int32_t height, int32_t* pixels);
	ArrayBitmap(int32_t width, int32_t height, int32_t* pixels, int32_t offsetX, int32_t offsetY, int32_t rowOffset);
	ArrayBitmap(const std::string& fileLocation);
	virtual ~ArrayBitmap();

	bool Load(const std::string& fileLocation);
	bool Save(const std::string& fileLocation);
	void Clear(int32_t color);

	inline int32_t GetWidth() const;
	inline int32_t GetHeight() const;
	inline int32_t Get(int32_t x, int32_t y) const;
	inline void Set(int32_t x, int32_t y, int32_t pixel);
	inline int32_t* GetPixelArray() const;
private:
	int32_t _width{};
	int32_t _height{};
	int32_t* _pixels{};
	inline uintptr_t GetPixelsSize() const;
};

