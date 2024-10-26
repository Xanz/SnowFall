#pragma once
#include "vulkan/vulkan.h"
#include <vector>

class VulkanInstance
{
public:
    VulkanInstance(const char *applicationName);

    ~VulkanInstance();

private:
    bool CheckValidationSupport();

    std::vector<const char *> GetRequiredExtensions();

    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData);

    const std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"};

    VkInstance m_Instance;
};