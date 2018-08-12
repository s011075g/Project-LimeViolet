#include "DDSTexture.h"

DDSTexture::DDSTexture()
	: _buffer(nullptr)
{ }

DDSTexture::~DDSTexture()
{
	CleanUp();
}

bool DDSTexture::Load(const std::string& fileLocation)
{
	unsigned char header[124];
	FILE* fp;
	errno_t err = fopen_s(&fp, fileLocation.c_str(), "rb");
	if (!fp || err)
		return false;

	// Verify valid DDS file
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) 
	{
		fclose(fp);
		return false;
	}

	// Read header
	fread(&header, sizeof(header) / sizeof(header[0]), 1, fp);
	_height = *reinterpret_cast<uint32_t*>(&(header[8]));
	_width = *reinterpret_cast<uint32_t*>(&(header[12]));
	uint32_t linearSize = *reinterpret_cast<uint32_t*>(&(header[16]));
	_mipMapCount = *reinterpret_cast<uint32_t*>(&(header[24]));
	_fourCC = *reinterpret_cast<uint32_t*>(&(header[80]));

	// Allocate memory for DDS file
	uint32_t bufsize = _mipMapCount > 1 ? linearSize * 2 : linearSize;
	CleanUp();
	_buffer = static_cast<unsigned char*>(malloc(bufsize * sizeof(unsigned char)));
	fread(_buffer, 1, bufsize, fp);
	fclose(fp);

	return true;
}

uint32_t DDSTexture::GetMipMapCount() const
{
	return _mipMapCount;
}

uint32_t DDSTexture::GetFourCC() const
{
	return _fourCC;
}

uint32_t DDSTexture::GetWidth() const
{
	return _width;
}

uint32_t DDSTexture::GetHeight() const
{
	return _height;
}

const unsigned char* DDSTexture::GetBuffer() const
{
	return _buffer;
}

void DDSTexture::CleanUp()
{
	if (_buffer)
		free(_buffer);
	_buffer = nullptr;
}
