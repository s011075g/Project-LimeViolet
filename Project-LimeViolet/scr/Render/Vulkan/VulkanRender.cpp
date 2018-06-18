#include "VulkanRender.h"
#include <iostream>
#include <set>
#include <vulkan/vulkan.hpp>
#include "../../Common/Utilities.h"
#include "VulkanShaderManager.h"

#define MAX_FRAMES_IN_FLIGHT 2

struct QueueFamilyIndices 
{
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool IsComplete() const
	{
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

//Horrible Global to this file
const std::vector<const char*> DeviceExtensions = 
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct SwapChainSupportDetails 
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

VulkanRender::VulkanRender()
	: _window(nullptr), _instance(nullptr), _device(nullptr), _physicalDevice(nullptr), _graphicsQueue(nullptr),
	  _surface(0), _presentQueue(nullptr), _swapChain(0), _swapChainImageFormat(), _renderPass(0), _pipelineLayout(0),
	  _graphicsPipeline(0), _commandPool(0), _triangle(nullptr)
{ }

VulkanRender::~VulkanRender()
{
	VulkanRender::CleanUp();
}

HRESULT VulkanRender::InitWindow(RECT& rc, const char*& windowTitle)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	_windowWidth = rc.right - rc.left;
	_windowHeight = rc.bottom - rc.top;

	_window = glfwCreateWindow(_windowWidth, _windowHeight, windowTitle, nullptr, nullptr);
	return S_OK;
}

HRESULT VulkanRender::InitRenderer()
{
	if (InitInstance() != S_OK)
		return E_FAIL;

	if (CreateSurface() != S_OK)
		return E_FAIL;

	if (PickPhysicalDevice() != S_OK)
		return E_FAIL;
	
	if (CreateLogicDevice() != S_OK)
		return E_FAIL;

	_vboManager = new VulkanVBOManager(_device, _physicalDevice);

	if (RecreateSwapChain() != S_OK)
		return E_FAIL;

	if (CreateCommandPool() != S_OK)
		return E_FAIL;

	if (CreateCommandBuffers() != S_OK)
		return E_FAIL;

	if (CreateSyncObjects() != S_OK)
		return E_FAIL;

	return S_OK;
}

void VulkanRender::Update()
{
	IRender::Update();
	glfwPollEvents();
}

void VulkanRender::Draw()
{
	vkWaitForFences(_device, 1, &_inFlightFences[_currentFrame], VK_TRUE, reinterpret_cast<uint32_t>(std::numeric_limits<uint32_t>::max));
	vkResetFences(_device, 1, &_inFlightFences[_currentFrame]);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(_device, _swapChain, reinterpret_cast<uint32_t>(std::numeric_limits<uint32_t>::max), _imageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) 
	{
		RecreateSwapChain();
		return;
	}
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		Utilities::Write("Failed to acquire swap chain image!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to acquire swap chain image!");
	}

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { _imageAvailableSemaphores[_currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &_commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { _renderFinishedSemaphores[_currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;
	if (vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _inFlightFences[_currentFrame]) != VK_SUCCESS)
	{
		Utilities::Write("Failed to submit draw command buffer!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { _swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional
	
	result = vkQueuePresentKHR(_presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) 
	{
		RecreateSwapChain();
	}
	else if (result != VK_SUCCESS) 
	{
		Utilities::Write("Failed to present swap chain image!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to present swap chain image!");
	}
	_currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

bool VulkanRender::ShouldExit()
{
	return !glfwWindowShouldClose(_window);
}

void VulkanRender::CleanUp()
{
	vkDeviceWaitIdle(_device);

	CleanupSwapChain();

	if(_triangle)
		_vboManager->DeleteVBO(_triangle);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
		vkDestroySemaphore(_device, _renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(_device, _imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(_device, _inFlightFences[i], nullptr);
	}

	vkDestroyCommandPool(_device, _commandPool, nullptr);
	
	vkDestroyDevice(_device, nullptr);
	vkDestroySurfaceKHR(_instance, _surface, nullptr);
	vkDestroyInstance(_instance, nullptr);

	glfwDestroyWindow(_window);
	glfwTerminate();
}

void VulkanRender::UpdateViewMatrix()
{ }

void VulkanRender::UpdateProjectionMatrix()
{ }

HRESULT VulkanRender::InitInstance()
{
	vk::ApplicationInfo appInfo = vk::ApplicationInfo()
		.setPApplicationName("Window")
		.setApplicationVersion(1)
		.setPEngineName("LunarG SDK")
		.setApplicationVersion(VK_MAKE_VERSION(1, 0, 0))
		.setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
		.setApiVersion(VK_API_VERSION_1_0);

	uint32_t glfwExtensionCount = 0;
	const char ** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

#if defined(DEBUG) || defined(_DEBUG)
	std::cout << "Required extensions:" << std::endl;
	for (uint32_t i = 0; i < glfwExtensionCount; i++)
		std::cout << "\t" << glfwExtensions[i] << std::endl;

	SupportedExtensions();
#endif

	VkInstanceCreateInfo instInfo = vk::InstanceCreateInfo()
		.setFlags(vk::InstanceCreateFlags())
		.setPApplicationInfo(&appInfo)
		.setEnabledExtensionCount(glfwExtensionCount)
		.setPpEnabledExtensionNames(glfwExtensions);

	VkResult result = vkCreateInstance(&instInfo, nullptr, &_instance);

	if (result != VK_SUCCESS)
	{
		Utilities::Write("Failed to create Vulkan instance!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to create Vulkan instance!");
	}

	return S_OK;
}

HRESULT VulkanRender::CreateSurface()
{
	if(glfwCreateWindowSurface(_instance, _window, nullptr, &_surface) != VK_SUCCESS)
	{
		Utilities::Write("Failed to create window surface!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to create window surface!");
	}
	return S_OK;
}

HRESULT VulkanRender::PickPhysicalDevice()
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

	return S_OK;
}

HRESULT VulkanRender::CreateLogicDevice()
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

	return S_OK;
}

HRESULT VulkanRender::RecreateSwapChain()
{
	vkDeviceWaitIdle(_device);

	if (CreateSwapChain() != S_OK)
		return E_FAIL;

	if (CreateImageViews() != S_OK)
		return E_FAIL;

	if (CreateRenderPass() != S_OK)
		return E_FAIL;

	if (CreateGraphicsPipeLine() != S_OK)
		return E_FAIL;

	if (CreateFrameBuffers() != S_OK)
		return E_FAIL;

	return S_OK;
}

HRESULT VulkanRender::CreateSwapChain()
{
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(_physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) 
		imageCount = swapChainSupport.capabilities.maxImageCount;

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = _surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = FindQueueFamilies(_physicalDevice);
	uint32_t queueFamilyIndices[] = { static_cast<uint32_t>(indices.graphicsFamily), static_cast<uint32_t>(indices.presentFamily) };

	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapChain) != VK_SUCCESS) 
	{
		Utilities::Write("Failed to create swap chain!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, nullptr);
	_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, _swapChainImages.data());

	_swapChainImageFormat = surfaceFormat.format;
	_swapChainExtent = extent;

	return S_OK;
}

HRESULT VulkanRender::CreateImageViews()
{
	_swapChainImageViews.resize(_swapChainImages.size());
	for (size_t i = 0; i < _swapChainImages.size(); i++) 
	{
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = _swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = _swapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(_device, &createInfo, nullptr, &_swapChainImageViews[i]) != VK_SUCCESS)
		{
			Utilities::Write("Failed to create image views!", Utilities::LEVEL::ERROR_LEVEL);
			throw std::runtime_error("Failed to create image views!");
		}
	}
	return S_OK;
}

HRESULT VulkanRender::CreateRenderPass()
{
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = _swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(_device, &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS) 
	{
		Utilities::Write("Failed to create render pass!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to create render pass!");
	}

	return S_OK;
}

HRESULT VulkanRender::CreateGraphicsPipeLine()
{
	auto vertex = VulkanShaderManager::CreateShader(_device, "shaders/Vulkan/vert.spv");
	auto fragment = VulkanShaderManager::CreateShader(_device, "shaders/Vulkan/frag.spv");

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertex;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragment;
	fragShaderStageInfo.pName = "main";
	
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	//VERTEX
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	
	auto bindingDescript = VulkanVBOManager::GetObjectVertexBindingDescription();
	auto attributeDescriptions = VulkanVBOManager::GetObjectVertexAttributeDescriptions();
	
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()); // Optional
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescript;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
	
	//Input Assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	//Viewport and scissors
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(_swapChainExtent.width);
	viewport.height = static_cast<float>(_swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = _swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	//Rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	//Multisampling
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	//Depth stencil
		///Exempted for now

	//Color blending (Alpha)
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	//Dynamic state
	VkDynamicState dynamicStates[] = 
	{
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};

	VkPipelineDynamicStateCreateInfo dynamicState = {};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;

	//Pipeline layout
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0; // Optional
	pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(_device, &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) 
	{
		Utilities::Write("Failed to create pipeline layout!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = _pipelineLayout;
	pipelineInfo.renderPass = _renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1; // Optional

	if (vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphicsPipeline) != VK_SUCCESS) 
	{
		Utilities::Write("Failed to create graphics pipeline!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to create graphics pipeline!");
	}

	VulkanShaderManager::FreeShader(_device, vertex);
	VulkanShaderManager::FreeShader(_device, fragment);
	return S_OK;
}

HRESULT VulkanRender::CreateFrameBuffers()
{
	_swapChainFramebuffers.resize(_swapChainImageViews.size());

	for (size_t i = 0; i < _swapChainImageViews.size(); i++) {
		VkImageView attachments[] = {
			_swapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = _renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = _swapChainExtent.width;
		framebufferInfo.height = _swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(_device, &framebufferInfo, nullptr, &_swapChainFramebuffers[i]) != VK_SUCCESS)
		{
			Utilities::Write("Failed to create framebuffer!", Utilities::LEVEL::ERROR_LEVEL);
			throw std::runtime_error("Failed to create framebuffer!");
		}
	}

	return S_OK;
}

HRESULT VulkanRender::CreateCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(_physicalDevice);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
	poolInfo.flags = 0; // Optional

	if (vkCreateCommandPool(_device, &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) 
	{
		Utilities::Write("Failed to create command pool!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to create command pool!");
	}

	return S_OK;
}

HRESULT VulkanRender::CreateCommandBuffers()
{
	_commandBuffers.resize(_swapChainFramebuffers.size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = _commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

	if (vkAllocateCommandBuffers(_device, &allocInfo, _commandBuffers.data()) != VK_SUCCESS) 
	{
		Utilities::Write("Failed to allocate command buffers!", Utilities::LEVEL::ERROR_LEVEL);
		throw std::runtime_error("Failed to allocate command buffers!");
	}

	//todo remove as following is for debug  

	std::vector<ObjectVertex> vertex(3);
	vertex[0] = ObjectVertex({ Float3(0.0f, -0.5f, 0.0f),Float2(0,0), Float3(1,1,1), Float4(0,0,0,0) });
	vertex[1] = ObjectVertex({ Float3(0.5f, 0.5f, 0.0f),Float2(0,0), Float3(0,1,0), Float4(0,0,0,0) });
	vertex[2] = ObjectVertex({Float3(-0.5f, 0.5f, 0.0f),Float2(0,0), Float3(0,0,1), Float4(0,0,0,0)});

	auto indices = std::map<unsigned short, std::vector<unsigned short>>();
	auto materials = std::vector<Material*>();
	RawGeometry* geometry = new RawGeometry(vertex, indices, materials);

	_triangle = _vboManager->VBOGeometry(geometry);
	delete geometry;

	for (size_t i = 0; i < _commandBuffers.size(); i++) 
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		//beginInfo.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS) 
		{
			Utilities::Write("Failed to begin recording command buffer!", Utilities::LEVEL::ERROR_LEVEL);
			throw std::runtime_error("Failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = _renderPass;
		renderPassInfo.framebuffer = _swapChainFramebuffers[i];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = _swapChainExtent;
		renderPassInfo.pNext = nullptr;

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline);

			VkBuffer vertexBuffers[] = { *static_cast<VkBuffer*>(_triangle->GetVertexBuffer()) };
			VkDeviceSize offsets[] = { 0 };

			vkCmdBindVertexBuffers(_commandBuffers[i], 0, 1, vertexBuffers, offsets);

			vkCmdDraw(_commandBuffers[i], 3, 1, 0, 0);

		vkCmdEndRenderPass(_commandBuffers[i]);

		if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS) 
		{
			Utilities::Write("Failed to record command buffer!", Utilities::LEVEL::ERROR_LEVEL);
			throw std::runtime_error("Failed to record command buffer!");
		}
	}

	return S_OK;
}

HRESULT VulkanRender::CreateSyncObjects()
{
	_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		if (vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(_device, &fenceInfo, nullptr, &_inFlightFences[i]) != VK_SUCCESS)
		{
			Utilities::Write("Failed to create synchronization objects for a frame!", Utilities::LEVEL::ERROR_LEVEL);
			throw std::runtime_error("Failed to create synchronization objects for a frame!");
		}

	return S_OK;
}

void VulkanRender::CleanupSwapChain()
{
	for (auto framebuffer : _swapChainFramebuffers)
		vkDestroyFramebuffer(_device, framebuffer, nullptr);

	vkFreeCommandBuffers(_device, _commandPool, static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());

	vkDestroyPipeline(_device, _graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
	vkDestroyRenderPass(_device, _renderPass, nullptr);

	for (auto imageView : _swapChainImageViews)
		vkDestroyImageView(_device, imageView, nullptr);

	vkDestroySwapchainKHR(_device, _swapChain, nullptr);
}

void VulkanRender::SupportedExtensions()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "Available extensions:" << std::endl;

	for (const auto& extension : extensions)
		std::cout << "\t" << extension.extensionName << std::endl;
}

bool VulkanRender::IsDeviceSuitable(const VkPhysicalDevice device) const
{
	QueueFamilyIndices indices = FindQueueFamilies(device);

	bool extensionsSupported = CheckDeviceExtensionSupport(device);
	bool swapChainAdequate = false;
	if (extensionsSupported)
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}
	return indices.IsComplete() && extensionsSupported && swapChainAdequate;
}

QueueFamilyIndices VulkanRender::FindQueueFamilies(const VkPhysicalDevice device) const
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

bool VulkanRender::CheckDeviceExtensionSupport(const VkPhysicalDevice device)
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

SwapChainSupportDetails VulkanRender::QuerySwapChainSupport(const VkPhysicalDevice device) const
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

VkSurfaceFormatKHR VulkanRender::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

	for (const auto& availableFormat : availableFormats)
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return availableFormat;

	return availableFormats[0];
}

VkPresentModeKHR VulkanRender::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
{
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			return availablePresentMode;
		if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
			bestMode = availablePresentMode;
	}

	return bestMode;
}

VkExtent2D VulkanRender::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
{
	if (capabilities.currentExtent.width != reinterpret_cast<uint32_t>(std::numeric_limits<uint32_t>::max))
		return capabilities.currentExtent;

	VkExtent2D actualExtent = { static_cast<uint32_t>(_windowWidth), static_cast<uint32_t>(_windowHeight) };

	actualExtent.width = max(capabilities.minImageExtent.width, min(capabilities.maxImageExtent.width, actualExtent.width));
	actualExtent.height = max(capabilities.minImageExtent.height, min(capabilities.maxImageExtent.height, actualExtent.height));

	return actualExtent;
}
