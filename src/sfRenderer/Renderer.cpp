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

/*
==================
GL_CheckErrors
==================
*/
void GL_CheckErrorsTwo(void)
{
    int err;
    char s[64];
    int i;

    // check for up to 10 errors pending
    for (i = 0; i < 10; i++)
    {
        err = glGetError();
        if (err == GL_NO_ERROR)
        {
            return;
        }
        switch (err)
        {
        case GL_INVALID_ENUM:
            strcpy(s, "GL_INVALID_ENUM");
            break;
        case GL_INVALID_VALUE:
            strcpy(s, "GL_INVALID_VALUE");
            break;
        case GL_INVALID_OPERATION:
            strcpy(s, "GL_INVALID_OPERATION");
            break;
        case GL_STACK_OVERFLOW:
            strcpy(s, "GL_STACK_OVERFLOW");
            break;
        case GL_STACK_UNDERFLOW:
            strcpy(s, "GL_STACK_UNDERFLOW");
            break;
        case GL_OUT_OF_MEMORY:
            strcpy(s, "GL_OUT_OF_MEMORY");
            break;
        default:
            idStr::snPrintf(s, sizeof(s), "%i", err);
            break;
        }

        // if (!r_ignoreGLErrors.GetBool())
        // {
        common->Printf("GL_CheckErrors: %s\n", s);
        // }
    }
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
