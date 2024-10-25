#pragma once
#include <GLFW/glfw3.h>

#include "precompiled.h"
#include "renderer/tr_local.h"


extern idCVar r_windowX;
extern idCVar r_windowY;
extern idCVar r_windowWidth;
extern idCVar r_windowHeight;
extern idCVar r_vidMode; // video mode number
extern idCVar r_displayRefresh; // optional display refresh rate option for vid mode
extern idCVar r_fullscreen; // 0 = windowed, 1 = full screen
extern idCVar r_multiSamples; // number of antialiasing samples

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

  void Update();

  void PreUpdate();

  bool Create();

  ~Window();

private:
  void SetState(eWindowState state) { m_State = state; }

  static void MouseKey_Callback(GLFWwindow* window, int button, int action, int mods);

  static void Cursor_Callback(GLFWwindow* window, double xpos, double ypos);

  static void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);

  static void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);

  static void Character_Callback(GLFWwindow* window, unsigned int codepoint);

  windowParms_t SetupParms();

  eWindowState m_State;

  GLFWwindow* m_Window;
};


