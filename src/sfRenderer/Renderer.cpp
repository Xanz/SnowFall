#include "Renderer.h"

void Renderer::Init()
{
  glewInit();
  glewExperimental = GL_TRUE;
}
