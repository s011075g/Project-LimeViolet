#include "VulkanRender.h"
#include <iostream>
#include "../../Common/Utilities.h"

struct QueueFamilyIndices 
{
	int graphicsFamily = -1;

	bool IsComplete() const
	{
		return graphicsFamily >= 0;
	}
};

VulkanRender::VulkanRender()
	: _window(nullptr), _instance(nullptr), _physicalDevice(nullptr)
{ }

VulkanRender::~VulkanRender()
{
	CleanUp();
}

HRESULT VulkanRender::InitWindow(RECT& rc, const char*& windowTitle)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	_window = glfwCreateWindow(rc.right - rc.left, rc.bottom - rc.top, windowTitle, nullptr, nullptr);
	return S_OK;
}

#include <vulkan/vulkan.hpp>

HRESULT VulkanRender::InitRenderer()
{
	if (InitInstance() != S_OK)
		return E_FAIL;

	if (PickPhysicalDevice() != S_OK)
		return E_FAIL;
	
	if (CreateLogicDevice() != S_OK)
		return E_FAIL;

	return S_OK;
}

void VulkanRender::Update()
{
	IRender::Update();
	glfwPollEvents();
}

void VulkanRender::Draw()
{ }

bool VulkanRender::ShouldExit()
{
	return !glfwWindowShouldClose(_window);
}

void VulkanRender::CleanUp()
{
	vkDestroyDevice(_device, nullptr);
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
		Utilities::Write("Failed to create Vulkan instance!", Utilities::LEVEL::EXTREME_LEVEL);
		throw std::runtime_error("Failed to create Vulkan instance!");
	}

	return S_OK;
}

HRESULT VulkanRender::PickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
	if (deviceCount == 0) 
	{
		Utilities::Write("Failed to find Vulkan support!", Utilities::LEVEL::EXTREME_LEVEL);
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
		Utilities::Write("Failed to find a suitable GPU!", Utilities::LEVEL::EXTREME_LEVEL);
		throw std::runtime_error("Failed to find a suitable GPU!");
	}

	return S_OK;
}

HRESULT VulkanRender::CreateLogicDevice()
{
	QueueFamilyIndices indices = FindQueueFamilies(_physicalDevice);

	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = 0;
	createInfo.enabledLayerCount = 0;

	if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device) != VK_SUCCESS)
	{
		Utilities::Write("Failed to create logic device!", Utilities::LEVEL::EXTREME_LEVEL);
		throw std::runtime_error("Failed to create logical device!");
	}

	vkGetDeviceQueue(_device, indices.graphicsFamily, 0, &_graphicsQueue);

	return S_OK;
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

bool VulkanRender::IsDeviceSuitable(const VkPhysicalDevice device)
{
	QueueFamilyIndices indices = FindQueueFamilies(device);

	return indices.IsComplete();
}

QueueFamilyIndices VulkanRender::FindQueueFamilies(const VkPhysicalDevice device)
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
		if (indices.IsComplete())
			break;
		i++;
	}

	return indices;
}
