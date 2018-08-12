#pragma once
#include <cstdint>
#include <string>

class DDSTexture
{
	unsigned char* _buffer;
	uint32_t _height;
	uint32_t _width;
	uint32_t _mipMapCount;
	uint32_t _fourCC;
public:
	DDSTexture();
	virtual ~DDSTexture();

	bool Load(const std::string& fileLocation);
	inline uint32_t GetMipMapCount() const;
	inline uint32_t GetFourCC() const;
	inline uint32_t GetWidth() const;
	inline uint32_t GetHeight() const;
	inline const unsigned char* GetBuffer() const;
private:
	void CleanUp();

	DDSTexture(const DDSTexture& other) = delete;
	void operator=(const DDSTexture& other) = delete;
};
