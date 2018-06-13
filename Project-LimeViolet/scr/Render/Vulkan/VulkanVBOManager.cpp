#include "VulkanVBOManager.h"

VulkanVBOManager::VulkanVBOManager(const VkDevice device)
	: _device(device)
{ }

VulkanVBOManager::~VulkanVBOManager()
{ }

Geometry* VulkanVBOManager::VBOGeometry(RawGeometry* geometry)
{

}

std::array<VkVertexInputAttributeDescription, 4> VulkanVBOManager::GetObjectVertexAttributeDescriptions()
{
	std::array<VkVertexInputAttributeDescription, 4> attribute{};

	attribute[0].binding = 0;
	attribute[0].location = 0;
	attribute[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attribute[0].offset = offsetof(ObjectVertex, vertex);

	attribute[0].binding = 0;
	attribute[0].location = 1;
	attribute[0].format = VK_FORMAT_R32G32_SFLOAT;
	attribute[0].offset = offsetof(ObjectVertex, uv);

	attribute[0].binding = 0;
	attribute[0].location = 2;
	attribute[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attribute[0].offset = offsetof(ObjectVertex, normal);

	attribute[0].binding = 0;
	attribute[0].location = 0;
	attribute[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attribute[0].offset = offsetof(ObjectVertex, tangent);

	return attribute;
}

VkVertexInputBindingDescription VulkanVBOManager::GetObjectVertexBindingDescription()
{
	VkVertexInputBindingDescription bindingDescription;
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(ObjectVertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}
