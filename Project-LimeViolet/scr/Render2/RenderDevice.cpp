#include "RenderDevice.h"
#include <cstdint>
#include <sdl/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>
#include "../Application.h"
#include <set>

struct QueueFamilyIndices
{
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool IsComplete() const
	{
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

RenderDevice::RenderDevice(SDLWindow& window)
{
	CreateVKInstance(window);
	CreateVKSurface(window);
	CreateVKPhysicalDevice();
	CreateVKDevice();



	//(vkGetInstanceProcAddr*)(SDL_Vulkan_GetVkGetInstanceProcAddr())(nullptr, vkCreateBuffer);
}

RenderDevice::~RenderDevice()
{
	vkDestroyCommandPool(_device, _commandPool, nullptr);

	vkDestroyDevice(_device, nullptr);
	vkDestroySurfaceKHR(_instance, _surface, nullptr);
	vkDestroyInstance(_instance, nullptr);
}

uint32_t RenderDevice::CreateSampler(SamplerFilter minFilter, SamplerFilter magFilter, SamplerWrapMode wrapU, SamplerWrapMode wrapV, float anisotropy)
{
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.minFilter = static_cast<VkFilter>(minFilter);
	samplerInfo.magFilter = static_cast<VkFilter>(magFilter);
	samplerInfo.addressModeU = static_cast<VkSamplerAddressMode>(wrapU);
	samplerInfo.addressModeV = static_cast<VkSamplerAddressMode>(wrapV);
	samplerInfo.anisotropyEnable = anisotropy == 1.0f ? VK_FALSE : VK_TRUE;
	samplerInfo.maxAnisotropy = anisotropy;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;
	VkSampler sampler;

	if(vkCreateSampler(_device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
	{
		Utilities::Write("Failed to create texture sampler!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to create texture sampler!");
	}
	_samplers.emplace_back(sampler, true);
	return _samplers.size();
}

uint32_t RenderDevice::ReleaseSampler(uint32_t sampler)
{
	if (sampler != 0 && _samplers[sampler-1].second)
	{
		vkDestroySampler(_device, _samplers[sampler-1].first, nullptr);
		_samplers[sampler-1].second = false;
	}
	return 0;
}

uint32_t RenderDevice::CreateShaderProgram(const std::string& shaderText)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = shaderText.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderText.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		Utilities::Write("Failed to create shader module!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to create shader module!");
	}

	_shaders.emplace_back(shaderModule, true);
	return _shaders.size();
}

uint32_t RenderDevice::ReleaseShaderProgram(uint32_t shader)
{
	if (shader != 0 && _shaders[shader - 1].second)
	{
		vkDestroyShaderModule(_device, _shaders[shader - 1].first, nullptr);
		_shaders[shader - 1].second = false;
	}
	return 0;
}

uint32_t RenderDevice::CreateTexture2D(uint32_t width, uint32_t height, void* data, PixelFormat dataFormat, PixelFormat internalFormat, bool generateMipmaps, bool compress)
{
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(width * height * 4, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data2;
	vkMapMemory(_device, stagingBufferMemory, 0, width * height * 4, 0, &data2);
	memcpy_s(data2, width * height * 4, data, width * height * 4);
	vkUnmapMemory(_device, stagingBufferMemory);

	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = static_cast<VkFormat>(dataFormat);
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkImage image;
	VkDeviceMemory imageMemory;

	if (vkCreateImage(_device, &imageInfo, nullptr, &image) != VK_SUCCESS) 
	{
		Utilities::Write("Failed to create image!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(_device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
	{
		Utilities::Write("Failed to allocate image memory!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to allocate image memory!");
	}

	vkBindImageMemory(_device, image, imageMemory, 0);

	TransitionImageLayout(image, static_cast<VkFormat>(dataFormat), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(stagingBuffer, image, width, height);
	TransitionImageLayout(image, static_cast<VkFormat>(dataFormat), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(_device, stagingBuffer, nullptr);
	vkFreeMemory(_device, stagingBufferMemory, nullptr);
	
	_images.emplace_back(image, imageMemory, true);
	return _images.size();
}

#define MAKEFOURCC(a, b, c, d)                              \
                ((uint32_t)(uint8_t)(a) | ((uint32_t)(uint8_t)(b) << 8) |       \
				((uint32_t)(uint8_t)(c) << 16) | ((uint32_t)(uint8_t)(d) << 24 ))

#define MAKEFOURCCDXT(a) MAKEFOURCC('D', 'X', 'T', a)

#define FOURCC_DXT1 MAKEFOURCCDXT('1')
#define FOURCC_DXT2 MAKEFOURCCDXT('2')
#define FOURCC_DXT3 MAKEFOURCCDXT('3')
#define FOURCC_DXT4 MAKEFOURCCDXT('4')
#define FOURCC_DXT5 MAKEFOURCCDXT('5')

//Very unsure if this will work - a lot of testing required 
uint32_t RenderDevice::CreateDDSTexture2D(uint32_t width, uint32_t height, const unsigned char* buffer, uint32_t fourCC, uint32_t mipMapCount)
{
	VkFormat format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
		break;
	//case FOURCC_DXT3:
	//	format = VK_FORMAT_BC3_UNORM_BLOCK; //Unsure
	//	break;
	//case FOURCC_DXT5:
	//	format = VK_FORMAT_BC5_UNORM_BLOCK; //Unsure
	//	break;
	default:
		Utilities::Write("Invalid DDS compression format", Utilities::WARNING_LEVEL);
		return 0;
	}
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(width * height * 4, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data2;
	vkMapMemory(_device, stagingBufferMemory, 0, width * height * 4, 0, &data2);
	memcpy_s(data2, width * height * 4, buffer, width * height * 4);
	vkUnmapMemory(_device, stagingBufferMemory);

	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkImage image;
	VkDeviceMemory imageMemory;

	if (vkCreateImage(_device, &imageInfo, nullptr, &image) != VK_SUCCESS)
	{
		Utilities::Write("Failed to create image!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(_device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
	{
		Utilities::Write("Failed to allocate image memory!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to allocate image memory!");
	}

	vkBindImageMemory(_device, image, imageMemory, 0);

	TransitionImageLayout(image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(stagingBuffer, image, width, height);
	TransitionImageLayout(image, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(_device, stagingBuffer, nullptr);
	vkFreeMemory(_device, stagingBufferMemory, nullptr);

	_images.emplace_back(image, imageMemory, true);
	return _images.size();
}

uint32_t RenderDevice::ReleaseTexture2D(uint32_t texture2D)
{
	if (texture2D != 0 && std::get<2>(_images[texture2D - 1]))
	{
		vkDestroyImage(_device, std::get<0>(_images[texture2D - 1]), nullptr);
		vkFreeMemory(_device, std::get<1>(_images[texture2D - 1]), nullptr);
		std::get<2>(_images[texture2D - 1]) = true;
	}
	return 0;
}

uint32_t RenderDevice::CreateUniformBuffer(const void* data, uintptr_t dataSize, BufferUsage usage)
{
	//CreateBuffer(dataSize, );
}

void RenderDevice::CreateVKInstance(SDLWindow& window)
{
	vk::ApplicationInfo appInfo = vk::ApplicationInfo()
		.setPApplicationName(window.GetTitle().c_str())
		.setApplicationVersion(1)
		.setPEngineName("Lime Violet")
		.setApplicationVersion(VK_MAKE_VERSION(1, 0, 0))
		.setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
		.setApiVersion(VK_API_VERSION_1_0);

	const char** extensions = nullptr;
	uint32_t extensionCount;

	SDL_bool sdlResult = SDL_Vulkan_GetInstanceExtensions(window.GetWindowHandle(), &extensionCount, extensions);
	if (sdlResult == SDL_FALSE)
	{
		//throw
	}

	if (Utilities::IsDebug())
	{
		for (uint32_t i = 0; i < extensionCount; i++)
			Utilities::Write(extensions[i]);
	}

	VkInstanceCreateInfo instanceInfo = vk::InstanceCreateInfo()
		.setFlags(vk::InstanceCreateFlags())
		.setPApplicationInfo(&appInfo)
		.setEnabledExtensionCount(extensionCount)
		.setPpEnabledExtensionNames(extensions);

	VkResult vkResult = vkCreateInstance(&instanceInfo, nullptr, &_instance);

	if (vkResult != VK_SUCCESS)
	{
		Utilities::Write("Failed to create Vulkan instance!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to create Vulkan instance!");
	}
}

void RenderDevice::CreateVKSurface(SDLWindow& window)
{
	if(SDL_Vulkan_CreateSurface(window.GetWindowHandle(), _instance, &_surface) != SDL_TRUE)
	{
		Utilities::Write("Failed to create window surface!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to create window surface!");
	}
}

void RenderDevice::CreateVKPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
	if (deviceCount == 0)
	{
		Utilities::Write("Failed to find Vulkan support!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to find Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

	for (const auto& device : devices)
		if (IsDeviceSuitable(device))
		{
			_physicalDevice = device;
			break;
		}

	if (!_physicalDevice)
	{
		Utilities::Write("Failed to find a suitable GPU!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to find a suitable GPU!");
	}
}

bool RenderDevice::IsDeviceSuitable(const VkPhysicalDevice device) const
{
	QueueFamilyIndices indices = FindQueueFamilies(device);

	bool extensionsSupported = CheckDeviceExtensionSupport(device);
	bool swapChainAdequate = false;
	if (extensionsSupported)
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.IsComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

RenderDevice::QueueFamilyIndices RenderDevice::FindQueueFamilies(const VkPhysicalDevice device) const
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.graphicsFamily = i;

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface, &presentSupport);

		if (queueFamily.queueCount > 0 && presentSupport)
			indices.presentFamily = i;

		if (indices.IsComplete())
			break;
		i++;
	}

	return indices;
}

const std::vector<const char*> DeviceExtensions =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

bool RenderDevice::CheckDeviceExtensionSupport(const VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(DeviceExtensions.begin(), DeviceExtensions.end());

	for (const auto& extension : availableExtensions)
		requiredExtensions.erase(extension.extensionName);

	return requiredExtensions.empty();
}

RenderDevice::SwapChainSupportDetails RenderDevice::QuerySwapChainSupport(const VkPhysicalDevice device) const
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

void RenderDevice::CreateVKDevice()
{
	QueueFamilyIndices indices = FindQueueFamilies(_physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

	float queuePriority = 1.0f;
	for (int queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size());
	createInfo.ppEnabledExtensionNames = DeviceExtensions.data();

	createInfo.enabledLayerCount = 0;

	if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS)
	{
		Utilities::Write("Failed to create logic device!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to create logical device!");
	}

	vkGetDeviceQueue(_device, indices.graphicsFamily, 0, &_graphicsQueue);
	vkGetDeviceQueue(_device, indices.presentFamily, 0, &_presentQueue);
}

void RenderDevice::CreateCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(_physicalDevice);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;

	if (vkCreateCommandPool(_device, &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) 
	{
		Utilities::Write("Failed to create graphics command pool!", Utilities::ERROR_LEVEL);
		throw std::runtime_error("Failed to create graphics command pool!");
	}
}

void RenderDevice::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(_device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(_device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(_device, buffer, bufferMemory, 0);
}

uint32_t RenderDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	Utilities::Write("Failed to find suitable memory type!", Utilities::LEVEL::ERROR_LEVEL);
	throw std::runtime_error("Failed to find suitable memory type!");
}

void RenderDevice::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else {
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier( commandBuffer, sourceStage, destinationStage,
							0, 0, nullptr, 0, nullptr, 1, &barrier );

	EndSingleTimeCommands(commandBuffer);
}

void RenderDevice::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = { width, height, 1 };

	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
	EndSingleTimeCommands(commandBuffer);
}

VkCommandBuffer RenderDevice::BeginSingleTimeCommands()
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = _commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(_device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void RenderDevice::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(_graphicsQueue);

	vkFreeCommandBuffers(_device, _commandPool, 1, &commandBuffer);
}
