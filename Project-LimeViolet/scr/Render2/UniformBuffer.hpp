#pragma once
#include "RenderDevice.h"

class UniformBuffer
{
	RenderDevice* _device;
	uint32_t _deviceId;
	uintptr_t _size;
public:
	inline UniformBuffer(RenderDevice& device, uintptr_t dataSize, enum RenderDevice::BufferUsage usage, const void* data = nullptr);
	inline ~UniformBuffer();

	inline void Update(const void* data, uintptr_t dataSize);
	inline void Update(const void* data);

	inline uint32_t GetID();
private:
	UniformBuffer(const UniformBuffer& other) = delete;
	void operator=(const UniformBuffer& other) = delete;
};

inline UniformBuffer::UniformBuffer(RenderDevice& device, uintptr_t dataSize, RenderDevice::BufferUsage usage, const void* data)
	: _device(&device), _deviceId(_device->CreateUniformBuffer(data, dataSize, usage)), _size(dataSize)
{ }

inline UniformBuffer::~UniformBuffer()
{
	_deviceId = _device->ReleaseUniformBuffer(_deviceId);
}

inline void UniformBuffer::Update(const void* data, uintptr_t dataSize)
{
	_device->UpdateUniformBuffer(_deviceId, data, dataSize);
}

inline void UniformBuffer::Update(const void* data)
{
	Update(data, _size);
}

inline uint32_t UniformBuffer::GetID()
{
	return _deviceId;
}
