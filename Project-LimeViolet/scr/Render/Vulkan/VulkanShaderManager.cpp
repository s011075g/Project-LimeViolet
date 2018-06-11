#include "VulkanShaderManager.h"
#include <fstream>
#include "../../Common/Utilities.h"

VulkanShaderManager::VulkanShaderManager()
{ }

VulkanShaderManager::~VulkanShaderManager()
{ }

VkShaderModule VulkanShaderManager::CreateShader(const VkDevice device, const char* fileLocation)
{
	auto file = ReadFile(fileLocation);
	return CreateShaderModule(device, file);
}

void VulkanShaderManager::FreeShader(const VkDevice device, const VkShaderModule shader)
{
	vkDestroyShaderModule(device, shader, nullptr);
}

std::vector<char> VulkanShaderManager::ReadFile(const char* fileLocation)
{
	std::ifstream file(fileLocation, std::ios::ate | std::ios::binary);
	if (!file.good())
	{
		Utilities::Write((std::string("Unable to open file: ") + std::string(fileLocation)).c_str(), Utilities::LEVEL::WARNING_LEVEL);
		return {};
	}

	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();
	return buffer;
}

VkShaderModule VulkanShaderManager::CreateShaderModule(const VkDevice device, const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) 
	{
		Utilities::Write("Failed to create shader module!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to create shader module!");
	}

	return shaderModule;
}
