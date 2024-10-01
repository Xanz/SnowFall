#include "Renderer.h"

void Renderer::Init()
{
    glewInit();
    glEnable(GL_DEPTH_TEST);
}

/// @brief Call this at the start of the scene.
void Renderer::BeginScene()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/// @brief Called at the end of the scene.
void Renderer::EndScene()
{
    // Draw shit here.
}

void Renderer::SetViewport(int width, int height)
{
    glViewport(0, 0, width, height);
}
