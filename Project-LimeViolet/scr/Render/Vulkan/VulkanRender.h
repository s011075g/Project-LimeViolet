#pragma once
#include "../IRender.h"

#define GLFW_INCLUDE_VULKAN //#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

struct QueueFamilyIndices;

class VulkanRender : public IRender
{
private:
	GLFWwindow * _window;

	VkInstance _instance;
	VkDevice _device;
	VkPhysicalDevice _physicalDevice;
	VkQueue _graphicsQueue;

	const std::vector<const char*> _validationLayers;
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
	HRESULT PickPhysicalDevice();
	HRESULT CreateLogicDevice();

	//Prints a list of supported extensions to the console
	static void SupportedExtensions();
	//
	static bool IsDeviceSuitable(const VkPhysicalDevice device);

	static QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice device);
};

