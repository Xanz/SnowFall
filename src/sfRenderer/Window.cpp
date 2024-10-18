#include "Window.h"
#include "GLFW/glfw3.h"
#include "sys/win32/win_local.h"
#include "sys/win32/win_input.h"

idCVar m_rawInput("m_rawinput", "1", CVAR_BOOL, "use raw input value : 0 : 1", 0, 1);

idCVar r_customWidth("r_customWidth", "1280", CVAR_RENDERER | CVAR_ARCHIVE | CVAR_INTEGER,
                     "custom screen width. set r_vidMode to -1 to activate");
idCVar r_customHeight("r_customHeight", "720", CVAR_RENDERER | CVAR_ARCHIVE | CVAR_INTEGER,
                      "custom screen height. set r_vidMode to -1 to activate");
idCVar r_windowX("r_windowX", "0", CVAR_RENDERER | CVAR_ARCHIVE | CVAR_INTEGER, "Non-fullscreen parameter");
idCVar r_windowY("r_windowY", "0", CVAR_RENDERER | CVAR_ARCHIVE | CVAR_INTEGER, "Non-fullscreen parameter");
idCVar r_windowWidth("r_windowWidth", "1280", CVAR_RENDERER | CVAR_ARCHIVE | CVAR_INTEGER, "Non-fullscreen parameter");
idCVar r_windowHeight("r_windowHeight", "720", CVAR_RENDERER | CVAR_ARCHIVE | CVAR_INTEGER, "Non-fullscreen parameter");

Window::Window()
{
    SetState(FAILED);
}

bool Window::Create()
{
    common->Printf("Creating GLFW Window.\n");

    if (!glfwInit())
    {
        common->Error("Could not start GLFW.\n");
        SetState(FAILED);
        return false;
    }

    windowParms_t parms = SetupParms();

    // full screen window
    if (parms.fullScreen)
    {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        // We need this to set our refresh rate correctly.
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        m_Window = glfwCreateWindow(mode->width, mode->height, "SnowFall Engine", NULL, NULL);
    }
    else
    {
        m_Window = glfwCreateWindow(parms.width, parms.height, "SnowFall Engine", NULL, NULL);
    }

    if (!m_Window)
    {
        glfwTerminate();
        SetState(FAILED);
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Disable resizing.
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // MSAA samples :)
    glfwWindowHint(GLFW_SAMPLES, r_multiSamples.GetInteger());
    glfwMakeContextCurrent(m_Window);


    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Raw mouse input
    if (m_rawInput.GetBool())
    {
        glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    glfwSetCursorPos(m_Window, parms.width / 2, parms.height / 2);

    // Toggle V-Sync
    if (r_swapInterval.GetInteger() == 1)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }

    glfwSetMouseButtonCallback(m_Window, MouseKey_Callback);
    glfwSetKeyCallback(m_Window, Key_Callback);
    glfwSetCursorPosCallback(m_Window, Cursor_Callback);
    glfwSetScrollCallback(m_Window, Scroll_Callback);
    glfwSetCharCallback(m_Window, Character_Callback);
    // glConfig.global_vao

    glConfig.isFullscreen = parms.fullScreen;
    glConfig.isStereoPixelFormat = parms.stereo;
    glConfig.nativeScreenWidth = parms.width;
    glConfig.nativeScreenHeight = parms.height;
    glConfig.multisamples = parms.multiSamples;

    glConfig.pixelAspect = 1.0f;

    return true;
}

void Window::PreUpdate()
{
    glfwPollEvents();
}

void Window::Update()
{
    glfwSwapBuffers(m_Window);
}

void Window::MouseKey_Callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_RELEASE)
    {
        int key;
        int state = action == GLFW_PRESS ? 1 : 0;

        // UI seems to use K_MOUSE1 instead of M_ACTION1
        if (game->Shell_IsActive())
        {
            switch (button)
            {
            case GLFW_MOUSE_BUTTON_LEFT:
                key = K_MOUSE1;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                key = K_MOUSE2;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                key = K_MOUSE3;
                break;
            case GLFW_MOUSE_BUTTON_4:
                key = K_MOUSE4;
                break;
            case GLFW_MOUSE_BUTTON_5:
                key = K_MOUSE5;
                break;
            }
        }
        else
        {
            switch (button)
            {
            case GLFW_MOUSE_BUTTON_LEFT:
                key = M_ACTION1;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                key = M_ACTION2;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                key = M_ACTION3;
                break;
            case GLFW_MOUSE_BUTTON_4:
                key = M_ACTION4;
                break;
            case GLFW_MOUSE_BUTTON_5:
                key = M_ACTION5;
                break;
            }
        }

        s_MousePolls.emplace_back(mousePoll_t(key, state));
        Sys_QueEvent(SE_KEY, key, state, 0, NULL, 0);
    }
}

void Window::Cursor_Callback(GLFWwindow* window, double xpos, double ypos)
{
    s_MousePolls.emplace_back(mousePoll_t(M_DELTAX, xpos));
    s_MousePolls.emplace_back(mousePoll_t(M_DELTAY, ypos));
    Sys_QueEvent(SE_MOUSE, xpos, ypos, 0, NULL, 0);

    glfwSetCursorPos(window, 0, 0);
}

void Window::Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Backspace needs to be handled here.
    if (key == GLFW_KEY_BACKSPACE && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        Sys_QueEvent(SE_CHAR, K_BACKSPACE, 0, 0, NULL, 0);
        return;
    }

    if (action == GLFW_PRESS || action == GLFW_RELEASE)
    {
        bool state = (action == GLFW_PRESS) ? 1 : 0;

        int keyValue = GLFWKeyConvert(key);

        s_KeyboardPolls.emplace_back(keyboardPoll_t(keyValue, state));

        Sys_QueEvent(SE_KEY, keyValue, state, 0, NULL, 0);
    }
}

void Window::Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Has to be done this way otherwise we get funky skipping.
    if (yoffset >= 1)
    {
        s_MousePolls.emplace_back(mousePoll_t(M_DELTAZ, yoffset));
        // Sys_QueEvent(SE_MOUSE, 1, 0, 0, NULL, 0);
        Sys_QueEvent(SE_KEY, K_MWHEELUP, yoffset, 0, NULL, 0);

        // I hate this but we have to reset the scroll straight after.
        s_MousePolls.emplace_back(mousePoll_t(M_DELTAZ, 0));
        // Sys_QueEvent(SE_MOUSE, 1, 0, 0, NULL, 0);
        Sys_QueEvent(SE_KEY, K_MWHEELUP, 0, 0, NULL, 0);
    }
    else if (yoffset <= -1)
    {
        s_MousePolls.emplace_back(mousePoll_t(M_DELTAZ, yoffset));
        // Sys_QueEvent(SE_MOUSE, -1, 0, 0, NULL, 0);
        Sys_QueEvent(SE_KEY, K_MWHEELDOWN, yoffset, 0, NULL, 0);

        // I hate this but we have to reset the scroll straight after.
        s_MousePolls.emplace_back(mousePoll_t(M_DELTAZ, 0));
        // Sys_QueEvent(SE_MOUSE, -1, 0, 0, NULL, 0);
        Sys_QueEvent(SE_KEY, K_MWHEELDOWN, 0, 0, NULL, 0);
    }
}

void Window::Character_Callback(GLFWwindow* window, unsigned int codepoint)
{
    // We convert the text to UTF-8
    char text[5] = {0};
    if (codepoint <= 0x7F)
    {
        text[0] = static_cast<char>(codepoint);
    }
    else if (codepoint <= 0x7FF)
    {
        text[0] = static_cast<char>(0xC0 | (codepoint >> 6));
        text[1] = static_cast<char>(0x80 | (codepoint & 0x3F));
    }
    else if (codepoint <= 0xFFFF)
    {
        text[0] = static_cast<char>(0xE0 | (codepoint >> 12));
        text[1] = static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        text[2] = static_cast<char>(0x80 | (codepoint & 0x3F));
    }
    else if (codepoint <= 0x10FFFF)
    {
        text[0] = static_cast<char>(0xF0 | (codepoint >> 18));
        text[1] = static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
        text[2] = static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        text[3] = static_cast<char>(0x80 | (codepoint & 0x3F));
    }

    Sys_QueEvent(SE_CHAR, (unsigned char)text[0], 0, 0, NULL, 0);
}

windowParms_t Window::SetupParms()
{
    windowParms_t parms = windowParms_t();

    // for (int i = 0; i < 3; i++)
    // {
    if (r_fullscreen.GetInteger() <= 0)
    {
        // use explicit position / size for window
        parms.x = r_windowX.GetInteger();
        parms.y = r_windowY.GetInteger();
        parms.width = r_windowWidth.GetInteger();
        parms.height = r_windowHeight.GetInteger();
        // may still be -1 to force a borderless window
        parms.fullScreen = r_fullscreen.GetInteger();
        parms.displayHz = 0; // ignored
    }
    else
    {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        parms.x = 0; // ignored
        parms.y = 0; // ignored
        parms.fullScreen = r_fullscreen.GetInteger();

        // set the parameters we are trying
        if (r_vidMode.GetInteger() == 0)
        {
            // try forcing a specific mode, even if it isn't on the list
            parms.width = r_customWidth.GetInteger();
            parms.height = r_customHeight.GetInteger();
            parms.displayHz = r_displayRefresh.GetInteger();
        }
        else
        {
            parms.width = mode->width;
            parms.height = mode->height;
            parms.displayHz = mode->refreshRate;
        }
    }

    parms.multiSamples = r_multiSamples.GetInteger();

    parms.stereo = false;
    // }

    return parms;
}
