#pragma once

#include "RenderDevice.h"
#include "UniformBuffer.hpp"
#include "Texture.hpp"
#include "Sampler.hpp"

class Shader
{
	RenderDevice * _device;
	uint32_t _deviceId;
public:
	inline Shader(RenderDevice& device, const std::string& text);
	inline ~Shader();
	inline void SetUniformBuffer(const std::string& name, UniformBuffer& buffer);
	inline void SetSampler(const std::string& name, Texture& texture, Sampler& sampler, uint32_t unit);
	inline uint32_t GetID() const;
private:
	Shader(const Shader& other) = delete;
	void operator=(const Shader& other) = delete;
};

inline Shader::Shader(RenderDevice& device, const std::string& text)
	: _device(&device), _deviceId(_device->CreateShaderProgram(text))
{ }

inline Shader::~Shader()
{
	_deviceId = _device->ReleaseShaderProgram(_deviceId);
}

inline void Shader::SetUniformBuffer(const std::string& name, UniformBuffer& buffer)
{
	_device->SetShaderUniformBuffer(_deviceId, name, buffer.GetID());
}

inline void Shader::SetSampler(const std::string& name, Texture& texture, Sampler& sampler, uint32_t unit)
{
	_device->SetShaderSampler(_deviceId, name, texture.GetID(), sampler.GetID(), unit);
}

inline uint32_t Shader::GetID() const
{
	return _deviceId;
}
