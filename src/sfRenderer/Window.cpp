#include "Window.h"
#include "GLFW/glfw3.h"
#include "sys/win32/win_local.h"
#include "sys/win32/win_input.h"

idCVar m_rawInput("m_rawinput", "1", CVAR_BOOL, "use raw input value : 0 : 1", 0, 1);

Window::Window()
{
    SetState(FAILED);
}

void Window::Create(windowParms_t parms)
{
    common->Printf("Initializing OpenGL subsystem\n");

    if (!glfwInit())
    {
        common->Error("Could not start GLFW\n");
        SetState(FAILED);
    }

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
        // glConfig.vidWidth = mode->width;
        // glConfig.vidHeight = mode->height;
    }
    else
    {
        m_Window = glfwCreateWindow(parms.width, parms.height, "SnowFall Engine", NULL, NULL);
    }

    if (!m_Window)
    {
        glfwTerminate();
        SetState(FAILED);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Disable resizing.
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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

    // MSAA samples :)
    glfwWindowHint(GLFW_SAMPLES, r_multiSamples.GetInteger());

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
    // if (key == GLFW_KEY_BACKSPACE && (action == GLFW_PRESS || action == GLFW_REPEAT))
    // {
    //     Sys_QueEvent(SE_CHAR, K_BACKSPACE, 0, 0, NULL, 0);
    //     return;
    // }

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
