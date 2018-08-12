#pragma once
#include "RenderDevice.h"

class Sampler
{
	RenderDevice * _device;
	uint32_t _deviceId;
public:
	inline Sampler(RenderDevice& device, enum RenderDevice::SamplerFilter minFilter = RenderDevice::FILTER_MIPMAP_LINEAR,
		enum RenderDevice::SamplerFilter magFilter = RenderDevice::FILTER_LINEAR, enum RenderDevice::SamplerWrapMode wrapU = RenderDevice::WRAP_CLAMP,
		enum RenderDevice::SamplerWrapMode wrapV = RenderDevice::WRAP_CLAMP, float anisotropy = 0.0f);
	inline ~Sampler();
	inline uint32_t GetID() const;
private:
	Sampler(const Sampler& other) = delete;
	void operator=(const Sampler& other) = delete;
};

inline Sampler::Sampler(RenderDevice& device, RenderDevice::SamplerFilter minFilter, RenderDevice::SamplerFilter magFilter, 
							RenderDevice::SamplerWrapMode wrapU, RenderDevice::SamplerWrapMode wrapV, float anisotropy)
	: _device(&device), _deviceId(_device->CreateSampler(minFilter, magFilter, wrapU, wrapV, anisotropy))
{ }

inline Sampler::~Sampler()
{
	_deviceId = _device->ReleaseSampler(_deviceId);
}

inline uint32_t Sampler::GetID() const
{
	return _deviceId;
}
