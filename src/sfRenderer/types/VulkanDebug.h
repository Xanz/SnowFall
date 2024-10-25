#pragma once
#include "precompiled.h"
#include <vulkan/vulkan.h>

class VulkanDebug
{
public:
    VulkanDebug();
    ~VulkanDebug();

private:
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData)
    {
        common->FatalError("validation layer: %s", pCallbackData->pMessage);
        return VK_FALSE;
    };

    VkDebugUtilsMessengerEXT debugMessenger;
};