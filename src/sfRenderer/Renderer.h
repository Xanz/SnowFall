#pragma once
#include "precompiled.h"
#include "vulkan/vulkan.h"
#include "types/VulkanInstance.h"
#include "types/VulkanDebug.h"

class Renderer
{
public:
  void Init();

private:
  std::unique_ptr<VulkanInstance> m_Instance;
  std::unique_ptr<VulkanDebug> m_Debugger;
};

extern Renderer s_Renderer;
