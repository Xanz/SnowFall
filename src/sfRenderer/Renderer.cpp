#include "Renderer.h"

#include <string>

void Renderer::Init()
{
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    common->FatalError("Could not init graphics API!");
  }
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
}
