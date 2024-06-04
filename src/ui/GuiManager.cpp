#include "GuiManager.h"

sfGUIManager guiManager;

/// @brief Sets the GUI manager state.
/// @param state
void sfGUIManager::SetState(sfGUIState_t state)
{
    // We can run any specific state inits here.
    switch (state)
    {
    case sfGUIState::MAIN_MENU:
        break;
    case sfGUIState::LOADING:
        break;
    case sfGUIState::HUD:
        break;
    case sfGUIState::CONSOLE:
        break;
    }

    // Always update the state regardless of whether there is an init stage.
    m_State = state;
}

void sfGUIManager::SetLoadingScreen(const char *mapName)
{
    idStr stripped = mapName;
    stripped.StripFileExtension();
    stripped.StripPath();

    char guiMap[MAX_STRING_CHARS];
    strncpy(guiMap, va("guis/map/%s.gui", stripped.c_str()), MAX_STRING_CHARS);

    // give the gamecode a chance to override
    game->GetMapLoadingGUI(guiMap);

    if (uiManager->CheckGui(guiMap))
    {
        m_LoadScreen = uiManager->FindGui(guiMap, true, false, true);
    }
    else
    {
        m_LoadScreen = uiManager->FindGui("guis/map/loading.gui", true, false, true);
    }

    m_LoadScreen->SetStateFloat("map_loading", 0.0f);

    SetState(sfGUIState_t::LOADING);
}

/// @brief Advances the loading screen by the frametime and the number of bytes loaded.
/// @param frameTime
/// @param bytesNumber
void sfGUIManager::AdvanceLoading(float frameTime, int bytesNumber)
{
    if (m_LoadScreen && bytesNumber)
    {
        float n = fileSystem->GetReadCount();
        float pct = (n / bytesNumber);
        m_LoadScreen->SetStateFloat("map_loading", pct);
        m_LoadScreen->StateChanged(frameTime);
    }
}

/// @brief This updates and redraws any GUI's
/// @param frameTime
void sfGUIManager::Update(float frameTime)
{
    switch (m_State)
    {
    case sfGUIState::MAIN_MENU:
        break;
    case sfGUIState::LOADING:
        Draw(m_LoadScreen, frameTime);
        break;
    case sfGUIState::HUD:
        break;
    case sfGUIState::CONSOLE:
        break;
    }
}

/// @brief Returns if a GUI is active.
/// @return
bool sfGUIManager::IsActive()
{
    if (m_State == sfGUIState::HUD)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/// @brief Redraws the interface.
/// @param userInterface
/// @param frameTime
void sfGUIManager::Draw(idUserInterface *userInterface, float frameTime)
{
    if (userInterface)
    {
        userInterface->Redraw(frameTime);
    }
    else
    {
        common->Warning("Could not draw user interface!");
    }
}
