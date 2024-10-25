#include "Renderer.h"

#include <string>

Renderer s_Renderer;

void Renderer::Init()
{
    // grab the instance
    m_Instance = std::make_unique<VulkanInstance>("SFEditor");
    m_Debugger = std::make_unique<VulkanDebug>();
}
