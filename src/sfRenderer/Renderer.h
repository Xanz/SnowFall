#pragma once
#include "../idlib/precompiled.h"

class Renderer
{
public:
    static void Init();

    static void BeginScene();

    static void EndScene();

    static void SetViewport(int posX, int posY, int width, int height);
};
