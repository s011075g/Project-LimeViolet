#pragma once
#include "../IRender.h"

#define GLFW_INCLUDE_VULKAN //#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "VulkanVBOManager.h"

struct QueueFamilyIndices;
struct SwapChainSupportDetails;

class VulkanRender : public IRender
{
private:
	GLFWwindow * _window;

	VkInstance _instance;
	VkDevice _device;
	VkPhysicalDevice _physicalDevice;
	VkQueue _graphicsQueue; 
	VkSurfaceKHR _surface;
	VkQueue _presentQueue;

	VkSwapchainKHR _swapChain;
	std::vector<VkImage> _swapChainImages;
	VkFormat _swapChainImageFormat;
	VkExtent2D _swapChainExtent;

	std::vector<VkImageView> _swapChainImageViews;

	VkRenderPass _renderPass;
	VkPipelineLayout _pipelineLayout;
	VkPipeline _graphicsPipeline;

	VkCommandPool _commandPool;

	std::vector<VkFramebuffer> _swapChainFramebuffers;
	std::vector<VkCommandBuffer> _commandBuffers;
	std::vector<VkSemaphore> _imageAvailableSemaphores;
	std::vector<VkSemaphore> _renderFinishedSemaphores;
	std::vector<VkFence> _inFlightFences;
	const std::vector<const char*> _validationLayers;

	size_t _currentFrame = 0;

	//TEMP
	Geometry* _triangle;
public:
	VulkanRender();
	~VulkanRender();
	HRESULT InitWindow(RECT& rc, const char*& windowTitle) override;
	HRESULT InitRenderer() override;

	void Update() override;
	void Draw() override;
	bool ShouldExit() override;
protected:
	void CleanUp() override;
	void UpdateViewMatrix() override;
	void UpdateProjectionMatrix() override;
private:
	HRESULT InitInstance();
	HRESULT CreateSurface();
	HRESULT PickPhysicalDevice();
	HRESULT CreateLogicDevice();
	HRESULT RecreateSwapChain();
		HRESULT CreateSwapChain();
		HRESULT CreateImageViews();
		HRESULT CreateRenderPass();
		HRESULT CreateGraphicsPipeLine();
		HRESULT CreateFrameBuffers();
	HRESULT CreateCommandPool();
	HRESULT CreateCommandBuffers();
	HRESULT CreateSyncObjects();


	void CleanupSwapChain();
	//Prints a list of supported extensions to the console
	static void SupportedExtensions();
	//
	bool IsDeviceSuitable(const VkPhysicalDevice device) const;

	QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice device) const;

	static bool CheckDeviceExtensionSupport(const VkPhysicalDevice device);

	SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice device) const;

	static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

	static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

	//Get the maximun value for a uint32_t 
	//Added to stop clashing with max() macro
	static constexpr uint32_t Max_uint32_t();
};



