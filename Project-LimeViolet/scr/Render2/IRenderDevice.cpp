#include "IRenderDevice.h"
#include <cstdint>
#include <sdl/SDL_vulkan.h>
#include <vulkan/vulkan.h>

bool IRenderDevice::isInitialized = false;

bool IRenderDevice::GlobalInit()
{
	if (isInitialized)
		return isInitialized;
	int32_t major = 3;
	int32_t minor = 2;
	isInitialized = true;
	SDL_Vulkan_GetVkGetInstanceProcAddr();

	return isInitialized;
}

IRenderDevice::IRenderDevice(SDLWindow*& window)
{ }

IRenderDevice::~IRenderDevice()
{ }
