/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
/*
** WIN_GLIMP.C
**
** This file contains ALL Win32 specific stuff having to do with the
** OpenGL refresh.  When a port is being made the following functions
** must be implemented by the port:
**
** GLimp_SwapBuffers
** GLimp_Init
** GLimp_Shutdown
** GLimp_SetGamma
**
** Note that the GLW_xxx functions are Windows specific GL-subsystem
** related functions that are relevant ONLY to win_glimp.c
*/
#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "win_local.h"
#include "rc/AFEditor_resource.h"
#include "rc/doom_resource.h"
#include "../../renderer/tr_local.h"
#include <GLFW/glfw3.h>
#include <vector>

// Adds raw mouse input.
idCVar m_rawInput("m_rawinput", "1", CVAR_BOOL, "use raw input value : 0 : 1", 0, 1);

/*
========================
GLimp_GetOldGammaRamp
========================
*/
static void GLimp_SaveGamma(void)
{
}

/*
========================
GLimp_RestoreGamma
========================
*/
static void GLimp_RestoreGamma(void)
{
}

/*
========================
GLimp_SetGamma

The renderer calls this when the user adjusts r_gamma or r_brightness
========================
*/
void GLimp_SetGamma(unsigned short red[256], unsigned short green[256], unsigned short blue[256])
{
}

static void MouseKey_Callback(GLFWwindow *window, int button, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_RELEASE)
	{
		int key;
		int state = action == GLFW_PRESS ? 1 : 0;

		// UI seems to use K_MOUSE1 instead of M_ACTION1
		if (UIActive)
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

		m_MousePolls.push_back(mouse_poll_t(key, state));
		Sys_QueEvent(GetTickCount(), SE_KEY, key, state, 0, NULL);
	}
}

static void Key_Callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	// Backspace needs to be handled here.
	if (key == GLFW_KEY_BACKSPACE && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		Sys_QueEvent(GetTickCount(), SE_CHAR, K_BACKSPACE, 0, 0, NULL);
		return;
	}

	if (action == GLFW_PRESS || action == GLFW_RELEASE)
	{
		bool state = (action == GLFW_PRESS) ? 1 : 0;
		m_KeyboardPolls.push_back(keyboard_poll_t(key, state));

		Sys_QueEvent(GetTickCount(), SE_KEY, GLFWDoom_MapKey(key), state, 0, NULL);
	}
}

static void Cursor_Callback(GLFWwindow *window, double xpos, double ypos)
{
	// We use different mouse styles to allow for better mouse handling for UI.
	if (UIActive)
	{
		Sys_QueEvent(GetTickCount(), SE_MOUSE_ABS, xpos, ypos, 0, NULL);
	}
	else
	{
		m_MousePolls.push_back(mouse_poll_t(M_DELTAX, xpos));
		m_MousePolls.push_back(mouse_poll_t(M_DELTAY, ypos));
		Sys_QueEvent(GetTickCount(), SE_MOUSE, xpos, ypos, 0, NULL);

		glfwSetCursorPos(window, 0, 0);
	}
}

static void Scroll_Callback(GLFWwindow *window, double xoffset, double yoffset)
{
	// Has to be done this way otherwise we get funky skipping.
	if (yoffset >= 1)
	{
		m_MousePolls.push_back(mouse_poll_t(M_DELTAZ, 1));
		Sys_QueEvent(GetTickCount(), SE_MOUSE, 1, 0, 0, NULL);
		Sys_QueEvent(GetTickCount(), SE_KEY, K_MWHEELUP, 1, 0, NULL);
	}
	else if (yoffset <= -1)
	{
		m_MousePolls.push_back(mouse_poll_t(M_DELTAZ, -1));
		Sys_QueEvent(GetTickCount(), SE_MOUSE, -1, 0, 0, NULL);
		Sys_QueEvent(GetTickCount(), SE_KEY, K_MWHEELDOWN, 1, 0, NULL);
	}
}

static void Character_Callback(GLFWwindow *window, unsigned int codepoint)
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

	Sys_QueEvent(GetTickCount(), SE_CHAR, (unsigned char)text[0], 0, 0, NULL);
}
/*
===================
GLimp_Init

This creates the GLFW window and inits GLEW.
The window always created in full screen.
===================
*/
bool GLimp_Init(glimpParms_t parms)
{
	common->Printf("Initializing OpenGL subsystem\n");

	if (!glfwInit())
	{
		common->Error("Could not start GLFW\n");
		return false;
	}

	// full screen window
	if (parms.fullScreen)
	{
		const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		// We need this to set our refresh rate correctly.
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		window = glfwCreateWindow(mode->width, mode->height, "SnowFall Engine", NULL, NULL);
		glConfig.vidWidth = mode->width;
		glConfig.vidHeight = mode->height;
	}
	else
	{
		window = glfwCreateWindow(parms.width, parms.height, "SnowFall Engine", NULL, NULL);
	}

	if (!window)
	{
		glfwTerminate();
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Disable resizing.
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwMakeContextCurrent(window);

	glewInit();

	// Raw mouse input
	if (m_rawInput.GetBool())
	{
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, parms.width / 2, parms.height / 2);

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

	glfwSetMouseButtonCallback(window, MouseKey_Callback);
	glfwSetKeyCallback(window, Key_Callback);
	glfwSetCursorPosCallback(window, Cursor_Callback);
	glfwSetScrollCallback(window, Scroll_Callback);
	glfwSetCharCallback(window, Character_Callback);

	return true;
}

/*
===================
GLimp_SetScreenParms

Sets up the screen based on passed parms..
===================
*/
bool GLimp_SetScreenParms(glimpParms_t parms)
{
	// GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	// const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	// glfwSetWindowMonitor(window, monitor, 0, 0, parms.width, parms.height, parms.displayHz);
	return true;
}

/*
===================
GLimp_Shutdown

This routine does all OS specific shutdown procedures for the OpenGL
subsystem.
===================
*/
void GLimp_Shutdown(void)
{
	common->Printf("Shutting down OpenGL subsystem\n");

	glfwDestroyWindow(window);

	// restore gamma
	GLimp_RestoreGamma();
}

/*
=====================
GLimp_SwapBuffers
=====================
*/
void GLimp_SwapBuffers(void)
{
	glfwSwapBuffers(window);
}

/*
===================
GLimp_ActivateContext
===================
*/
void GLimp_ActivateContext(void)
{
}

/*
===================
GLimp_DeactivateContext
===================
*/
void GLimp_DeactivateContext(void)
{
	glFinish();
}

/*
===================
GLimp_RenderThreadWrapper
===================
*/
static void GLimp_RenderThreadWrapper(void)
{
	win32.glimpRenderThread();
}

/*
=======================
GLimp_SpawnRenderThread

Returns false if the system only has a single processor
=======================
*/
bool GLimp_SpawnRenderThread(void (*function)(void))
{
	SYSTEM_INFO info;

	// check number of processors
	GetSystemInfo(&info);
	if (info.dwNumberOfProcessors < 2)
	{
		return false;
	}

	// create the IPC elements
	win32.renderCommandsEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	win32.renderCompletedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	win32.renderActiveEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	win32.glimpRenderThread = function;

	win32.renderThreadHandle = CreateThread(
		NULL,											   // LPSECURITY_ATTRIBUTES lpsa,
		0,												   // DWORD cbStack,
		(LPTHREAD_START_ROUTINE)GLimp_RenderThreadWrapper, // LPTHREAD_START_ROUTINE lpStartAddr,
		0,												   // LPVOID lpvThreadParm,
		0,												   //   DWORD fdwCreate,
		&win32.renderThreadId);

	if (!win32.renderThreadHandle)
	{
		common->Error("GLimp_SpawnRenderThread: failed");
	}

	SetThreadPriority(win32.renderThreadHandle, THREAD_PRIORITY_ABOVE_NORMAL);
#if 0
	// make sure they always run on different processors
	SetThreadAffinityMask( GetCurrentThread, 1 );
	SetThreadAffinityMask( win32.renderThreadHandle, 2 );
#endif

	return true;
}

// #define	DEBUG_PRINTS

/*
===================
GLimp_BackEndSleep

===================
*/
void *GLimp_BackEndSleep(void)
{
	void *data;

#ifdef DEBUG_PRINTS
	OutputDebugString("-->GLimp_BackEndSleep\n");
#endif
	ResetEvent(win32.renderActiveEvent);

	// after this, the front end can exit GLimp_FrontEndSleep
	SetEvent(win32.renderCompletedEvent);

	WaitForSingleObject(win32.renderCommandsEvent, INFINITE);

	ResetEvent(win32.renderCompletedEvent);
	ResetEvent(win32.renderCommandsEvent);

	data = win32.smpData;

	// after this, the main thread can exit GLimp_WakeRenderer
	SetEvent(win32.renderActiveEvent);

#ifdef DEBUG_PRINTS
	OutputDebugString("<--GLimp_BackEndSleep\n");
#endif
	return data;
}

/*
===================
GLimp_FrontEndSleep

===================
*/
void GLimp_FrontEndSleep(void)
{
#ifdef DEBUG_PRINTS
	OutputDebugString("-->GLimp_FrontEndSleep\n");
#endif
	WaitForSingleObject(win32.renderCompletedEvent, INFINITE);

#ifdef DEBUG_PRINTS
	OutputDebugString("<--GLimp_FrontEndSleep\n");
#endif
}

volatile bool renderThreadActive;

/*
===================
GLimp_WakeBackEnd

===================
*/
void GLimp_WakeBackEnd(void *data)
{
	int r;

#ifdef DEBUG_PRINTS
	OutputDebugString("-->GLimp_WakeBackEnd\n");
#endif
	win32.smpData = data;

	if (renderThreadActive)
	{
		common->FatalError("GLimp_WakeBackEnd: already active");
	}

	r = WaitForSingleObject(win32.renderActiveEvent, 0);
	if (r == WAIT_OBJECT_0)
	{
		common->FatalError("GLimp_WakeBackEnd: already signaled");
	}

	r = WaitForSingleObject(win32.renderCommandsEvent, 0);
	if (r == WAIT_OBJECT_0)
	{
		common->FatalError("GLimp_WakeBackEnd: commands already signaled");
	}

	// after this, the renderer can continue through GLimp_RendererSleep
	SetEvent(win32.renderCommandsEvent);

	r = WaitForSingleObject(win32.renderActiveEvent, 5000);

	if (r == WAIT_TIMEOUT)
	{
		common->FatalError("GLimp_WakeBackEnd: WAIT_TIMEOUT");
	}

#ifdef DEBUG_PRINTS
	OutputDebugString("<--GLimp_WakeBackEnd\n");
#endif
}

//===================================================================
