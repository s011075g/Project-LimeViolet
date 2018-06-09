#include "VulkanRender.h"
#include <iostream>
#include <minwinbase.h>


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

HRESULT VulkanRender::InitRenderer()
{
	VkApplicationInfo appInfo;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.pEngineName = "No Engine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;
	appInfo.pNext = nullptr;
	appInfo.applicationVersion = 1;
	appInfo.engineVersion = 1;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount = 0;
	const char ** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	createInfo.enabledLayerCount = 0;

	VkResult result = vkCreateInstance(&createInfo, nullptr, &_instance);

	if (result != VK_SUCCESS)
		return E_FAIL;

	return S_OK;
}

void VulkanRender::Draw()
{ }

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
