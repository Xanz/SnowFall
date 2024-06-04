#pragma once

#include "../idlib/precompiled.h"

enum sfGUIType
{

} typedef sfGUIType_t;

enum sfGUIState
{
    MAIN_MENU,
    LOADING,
    HUD,
    CONSOLE,
} typedef sfGUIState_t;

class sfGUIManager
{
public:
    sfGUIState_t GetCurrentState() { return m_State; };

    void SetState(sfGUIState_t state);

    void SetLoadingScreen(const char *mapName);

    void AdvanceLoading(float frameTime, int bytesNumber);

    void Update(float frameTime);

    bool IsActive();

private:
    sfGUIState_t m_State;
    idUserInterface *m_LoadScreen;

    void Draw(idUserInterface *userInterface, float frameTime);
};

extern sfGUIManager guiManager;