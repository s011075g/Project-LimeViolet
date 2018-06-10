#include "VulkanRender.h"
#include <iostream>

VulkanRender::VulkanRender()
	: _window(nullptr), _instance(nullptr), _validationLayers({"VK_LAYER_LUNARG_standard_validation"})
{ }

VulkanRender::~VulkanRender()
{
	CleanUp();
}

HRESULT VulkanRender::InitWindow(RECT& rc)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	_window = glfwCreateWindow(rc.right - rc.left, rc.bottom - rc.top, "Vulkan window", nullptr, nullptr);
	return S_OK;
}

#include <vulkan/vulkan.hpp>

HRESULT VulkanRender::InitRenderer()
{
	vk::ApplicationInfo appInfo = vk::ApplicationInfo()
        .setPApplicationName("Vulkan C++ Windowed Program Template")
        .setApplicationVersion(1)
        .setPEngineName("LunarG SDK")
        .setEngineVersion(1)
        .setApiVersion(VK_API_VERSION_1_0);
	/*appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.pEngineName = "No Engine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_1;
	appInfo.pNext = nullptr;*/

	uint32_t glfwExtensionCount = 0;
	const char ** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::cout << "Required extensions:" << std::endl;
	for (int i = 0; i < glfwExtensionCount; i++)
		std::cout << "\t" << glfwExtensions[i] << std::endl;

	SupportedExtensions();

	VkInstanceCreateInfo instInfo = vk::InstanceCreateInfo()
		.setFlags(vk::InstanceCreateFlags())
		.setPApplicationInfo(&appInfo)
		.setEnabledExtensionCount(glfwExtensionCount)
		.setPpEnabledExtensionNames(glfwExtensions);
		/*.setEnabledLayerCount(static_cast<uint32_t>(layers.size()))
		.setPpEnabledLayerNames(layers.data());*/
	/*createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;*/

	/*createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	createInfo.enabledLayerCount = 0;*/

	VkResult result = vkCreateInstance(&instInfo, nullptr, &_instance);

	if (result != VK_SUCCESS)
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
	vkDestroyInstance(_instance, nullptr);

	glfwDestroyWindow(_window);
	glfwTerminate();
}

void VulkanRender::UpdateViewMatrix()
{ }

void VulkanRender::UpdateProjectionMatrix()
{ }

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
