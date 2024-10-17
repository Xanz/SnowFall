#pragma once
#include <GLFW/glfw3.h>

#include "precompiled.h"
#include "renderer/tr_local.h"

enum eWindowState
{
  ACTIVE, // Window is updating.
  FAILED, // Failed to create the window.
};


class Window
{
public:
  Window();

  eWindowState GetState() { return m_State; };

  void Create(windowParms_t parms);

  void Update();

  void PreUpdate();

private:
  void SetState(eWindowState state) { m_State = state; }

  static void MouseKey_Callback(GLFWwindow* window, int button, int action, int mods);

  static void Cursor_Callback(GLFWwindow* window, double xpos, double ypos);

  static void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);

  static void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);

  static void Character_Callback(GLFWwindow* window, unsigned int codepoint);

  eWindowState m_State;

  GLFWwindow* m_Window;
};


