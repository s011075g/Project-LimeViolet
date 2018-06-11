#pragma once
#include "../IShaderManager.h"
#include <vector>
#include <vulkan/vulkan_core.h>

class VulkanShaderManager : public IShaderManager 
{
	VulkanShaderManager();
public:
	~VulkanShaderManager();
	static VkShaderModule CreateShader(const VkDevice device, const char* fileLocation);
	static void FreeShader(const VkDevice, const VkShaderModule shader);
private:
	static std::vector<char> ReadFile(const char* fileLocation);
	static VkShaderModule CreateShaderModule(const VkDevice device, const std::vector<char>& code);
};
