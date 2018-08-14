#pragma once
#include "RenderDevice.h"
#include "../Common/Color.h"
#include "Shader.hpp"

class RenderContext
{
public:
	inline RenderContext(RenderDevice& device);
	inline void Clear(bool shouldClearColor, bool shouldClearDepth, bool shouldClearStencil, const Color4& color, uint32_t stencil);
	inline void Clear(const Color4& color, bool shouldClearDepth = false);
	inline void Draw(Shader& shader);
private:
	RenderDevice & _device;
};

inline RenderContext::RenderContext(RenderDevice& device)
	: _device(device)
{ }

inline void RenderContext::Clear(bool shouldClearColor, bool shouldClearDepth, bool shouldClearStencil, const Color4& color, uint32_t stencil)
{
}
