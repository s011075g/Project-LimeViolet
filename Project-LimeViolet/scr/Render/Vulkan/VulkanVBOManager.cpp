#include "VulkanVBOManager.h"
#include "../../Common/Utilities.h"

VulkanVBOManager::VulkanVBOManager(const VkDevice device, const VkPhysicalDevice physicalDevice)
	: _device(device), _physicalDevice(physicalDevice)
{ }

VulkanVBOManager::~VulkanVBOManager()
{ }

Geometry* VulkanVBOManager::VBOGeometry(RawGeometry* geometry)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = 8;// sizeof(geometry->vertex[0]) * geometry->vertex.size();
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferInfo.flags = 0;
	bufferInfo.pNext = nullptr;
	bufferInfo.queueFamilyIndexCount = 0u;
	bufferInfo.pQueueFamilyIndices = nullptr;

	VkBuffer vertexBuffer;

	if (vkCreateBuffer(_device, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
	{
		Utilities::Write("Failed to create vertex buffer!", Utilities::LEVEL::WARNING_LEVEL);
		return nullptr;
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(_device, vertexBuffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	VkBuffer buffer;

	if (vkAllocateMemory(_device, &allocInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		vkDestroyBuffer(_device, vertexBuffer, nullptr);
		Utilities::Write("Failed to allocate vertex buffer memory!", Utilities::LEVEL::WARNING_LEVEL);
		return nullptr;
	}

	vkBindBufferMemory(_device, vertexBuffer, buffer, 0);


	void* data;

	vkMapMemory(_device, buffer, 0, bufferInfo.size, 0, &data);
	memcpy(data, geometry->vertex.data(), static_cast<size_t>(bufferInfo.size));
	vkUnmapMemory(_device, buffer);

	std::vector<void*> indices {};

	VkBuffer* vb = new VkBuffer(vertexBuffer);
	_vertexBufferMemory[vb] = new VkBuffer(buffer);
	void* vertex = static_cast<void*>(&vb);

	return new Geometry(vertex, indices, geometry->materials); //TODO FINISH UP 
}

void VulkanVBOManager::DeleteVBO(Geometry*& geometry)
{
	//Clean up after swapchain has been cleaned up
	vkDestroyBuffer(_device, *static_cast<VkBuffer*>(geometry->GetVertexBuffer()), nullptr);
	vkFreeMemory(_device, *_vertexBufferMemory[static_cast<VkBuffer*>(geometry->GetVertexBuffer())], nullptr);
}

uint32_t VulkanVBOManager::FindMemoryType(const uint32_t typeFilter, VkMemoryPropertyFlags properties) const
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;

	throw std::runtime_error("Failed to find suitable memory type!");
}

VkVertexInputBindingDescription VulkanVBOManager::GetObjectVertexBindingDescription()
{
	VkVertexInputBindingDescription bindingDescription;
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(ObjectVertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 4> VulkanVBOManager::GetObjectVertexAttributeDescriptions()
{
	std::array<VkVertexInputAttributeDescription, 4> attribute{};

	attribute[0].binding = 0;
	attribute[0].location = 0;
	attribute[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attribute[0].offset = offsetof(ObjectVertex, vertex);

	attribute[1].binding = 0;
	attribute[1].location = 1;
	attribute[1].format = VK_FORMAT_R32G32_SFLOAT;
	attribute[1].offset = offsetof(ObjectVertex, uv);

	attribute[2].binding = 0;
	attribute[2].location = 2;
	attribute[2].format = VK_FORMAT_R32G32B32_SFLOAT;
	attribute[2].offset = offsetof(ObjectVertex, normal);

	attribute[3].binding = 0;
	attribute[3].location = 3;
	attribute[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attribute[3].offset = offsetof(ObjectVertex, tangent);

	return attribute;
}