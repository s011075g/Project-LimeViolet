#pragma once
#include "../IVBOManager.h"
#include <array>
#include <vulkan/vulkan_core.h>
#include <map>
class VulkanVBOManager : public IVBOManager
{
private:
	const VkDevice& _device;
	const VkPhysicalDevice& _physicalDevice;
	std::map<VkBuffer*, VkDeviceMemory*> _vertexBufferMemory;
public:
	VulkanVBOManager(const VkDevice device, const VkPhysicalDevice physicalDevice);
	~VulkanVBOManager();
	Geometry* VBOGeometry(RawGeometry* geometry) override;
	void DeleteVBO(Geometry*& geometry) override;

	//ObjectVertex
	static VkVertexInputBindingDescription GetObjectVertexBindingDescription();
	static std::array<VkVertexInputAttributeDescription, 4> GetObjectVertexAttributeDescriptions();

	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
};
