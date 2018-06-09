#pragma once
#include "../IRender.h"

#define GLFW_INCLUDE_VULKAN //#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>


class VulkanRender : public IRender
{
private:
	GLFWwindow * _window;

	VkInstance _instance;

	const std::vector<const char*> _validationLayers;
public:
	VulkanRender();
	~VulkanRender();
	HRESULT InitWindow(RECT& rc) override;
	HRESULT InitRenderer() override;
	void Draw() override;
protected:
	void CleanUp() override;
	void UpdateViewMatrix() override;
	void UpdateProjectionMatrix() override;
};

