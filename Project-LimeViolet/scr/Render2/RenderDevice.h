#pragma once
#include "../SDL/sdlWindow.h"
#include <sdl/SDL_vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vector>
class RenderDevice
{
	//Vulkan varibles
	VkInstance _instance;
	VkSurfaceKHR _surface;
	VkPhysicalDevice _physicalDevice;
	VkDevice _device;
	VkQueue _graphicsQueue;
	VkQueue _presentQueue;
	VkCommandPool _commandPool;

	std::vector<VkCommandBuffer> _commandBuffers;

	VkSwapchainKHR _swapChain;
	std::vector<VkImage> _swapChainImages;
	std::vector<VkFramebuffer> _swapChainFramebuffers;
	std::vector<VkImageView> _swapChainImageViews;
	VkFormat _swapChainImageFormat;
	VkExtent2D _swapChainExtent;

	VkRenderPass _renderPass;
	//
	SDLWindow & _window;
	//Bool used to keep check to see if the object has been destroyed
	std::vector<std::pair<VkSampler, bool>> _samplers;
	std::vector<std::tuple<VkPipeline, VkPipelineLayout, bool>> _shaders;
	std::vector<std::tuple<VkImageView, VkImage, VkDeviceMemory, bool>> _images;

public:
	enum SamplerFilter
	{
		FILTER_NEAREST = VK_FILTER_NEAREST,
		FILTER_LINEAR = VK_FILTER_LINEAR,
		FILTER_MIPMAP_NEAREST = VK_SAMPLER_MIPMAP_MODE_NEAREST,
		FILTER_MIPMAP_LINEAR = VK_SAMPLER_MIPMAP_MODE_LINEAR,
	};
	enum SamplerWrapMode
	{
		WRAP_CLAMP = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		WRAP_REPEAT = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		WRAP_CLAMP_MIRROR = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE,
		WRAP_REPEAT_MIRROR = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT
	};
	enum PixelFormat
	{
		FORMAT_R = VK_FORMAT_R8_UNORM,
		FORMAT_RG = VK_FORMAT_R8G8_UNORM,
		FORMAT_RGB = VK_FORMAT_R8G8B8_UNORM,
		FORMAT_RGBA = VK_FORMAT_R8G8B8A8_UNORM,
		FORMAT_DEPTH = VK_FORMAT_R8_UNORM,
		FORMAT_DEPTH_AND_STENCIL = VK_FORMAT_R8G8_UNORM,
	};
	enum BufferUsage
	{
		USAGE_STAGE_VERTEX = VK_SHADER_STAGE_VERTEX_BIT,
		USAGE_STAGE_PIXEL = VK_SHADER_STAGE_FRAGMENT_BIT
	};
private:
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};
	struct QueueFamilyIndices
	{
		int graphicsFamily = -1;
		int presentFamily = -1;

		bool IsComplete() const
		{
			return graphicsFamily >= 0 && presentFamily >= 0;
		}
	};
public:
	
	RenderDevice(SDLWindow& window);
	virtual ~RenderDevice();

	uint32_t CreateSampler(enum SamplerFilter minFilter, enum SamplerFilter magFilter, enum SamplerWrapMode wrapU, enum SamplerWrapMode wrapV, float anisotropy);
	uint32_t ReleaseSampler(uint32_t sampler);

	uint32_t CreateShaderProgram(const std::string& shaderText);
	void SetShaderUniformBuffer(uint32_t shader, const std::string& uniformBufferName, uint32_t buffer);
	void SetShaderSampler(uint32_t shader, const std::string& samplerName, uint32_t texture, uint32_t sampler, uint32_t unit);
	uint32_t ReleaseShaderProgram(uint32_t shader);

	uint32_t CreateTexture2D(uint32_t width, uint32_t height, void* data, enum PixelFormat dataFormat, enum PixelFormat internalFormat, bool generateMipmaps, bool compress);
	uint32_t CreateDDSTexture2D(uint32_t width, uint32_t height, const unsigned char* buffer, uint32_t fourCC, uint32_t mipMapCount);
	uint32_t ReleaseTexture2D(uint32_t texture2D);

	uint32_t CreateUniformBuffer(const void* data, uintptr_t dataSize, enum BufferUsage usage);
	void UpdateUniformBuffer(uint32_t buffer, const void* data, uintptr_t dataSize);
	uint32_t ReleaseUniformBuffer(uint32_t buffer);
private:
	void CreateVKInstance(SDLWindow& window);
	void CreateVKSurface(SDLWindow& window);
	void CreateVKPhysicalDevice();
		bool IsDeviceSuitable(const VkPhysicalDevice device) const;
		static bool CheckDeviceExtensionSupport(const VkPhysicalDevice device);
	void CreateVKDevice();
	void RecreateVkSwapChain();
		void CreateVkSwapChain();
		void CreateVkImageViews();
		void CreateVkRenderPass();
		void CreateVkFrameBuffers();
	void CreateVkCommandPool();

	void CleanUpSwapChain();

	VkPipelineShaderStageCreateInfo CreateVkPipelineShaderStageInfo(std::string shader, VkShaderStageFlagBits flags) const;
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) const;
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) const;
	VkImageView CreateImageView(VkImage image, VkFormat format) const;
	static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<struct VkSurfaceFormatKHR>& availableFormats);
	static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
	SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice device) const;
	QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice device) const;

	VkCommandBuffer BeginSingleTimeCommands() const;
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer) const;

	RenderDevice(const RenderDevice& other) = delete;
	void operator=(const RenderDevice& other) = delete;
};
