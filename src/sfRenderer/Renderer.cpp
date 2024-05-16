#include "Renderer.h"

void Renderer::Init()
{
    glewInit();
    glewExperimental = GL_TRUE;
    glEnable(GL_DEPTH_TEST);
}

/// @brief Call this at the start of the scene.
void Renderer::BeginScene()
{
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/// @brief Called at the end of the scene.
void Renderer::EndScene()
{
    // Doom 3 renderer commands processed here.
    renderSystem->EndFrame(NULL, NULL);

    
}

void Renderer::SetViewport(int posX, int posY, int width, int height)
{
    glViewport(0, 0, width, height);
}
