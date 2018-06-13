#pragma once
#include "../IVBOManager.h"
#include <array>
#include <vulkan/vulkan_core.h>

class VulkanVBOManager : public IVBOManager
{
private:
	const VkDevice& _device;
public:
	VulkanVBOManager(const VkDevice device);
	~VulkanVBOManager();
	Geometry* VBOGeometry(RawGeometry* geometry) override;
	void DeleteVBO(Geometry*& geometry) override;

	//ObjectVertex
	static VkVertexInputBindingDescription GetObjectVertexBindingDescription();
	static std::array<VkVertexInputAttributeDescription, 4> GetObjectVertexAttributeDescriptions();
};
