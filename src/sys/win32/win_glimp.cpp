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


static void		GLW_InitExtensions( void );


// WGL_ARB_extensions_string
PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;

// WGL_EXT_swap_interval
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

// WGL_ARB_pixel_format
PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;

// WGL_ARB_pbuffer
PFNWGLCREATEPBUFFERARBPROC	wglCreatePbufferARB;
PFNWGLGETPBUFFERDCARBPROC	wglGetPbufferDCARB;
PFNWGLRELEASEPBUFFERDCARBPROC	wglReleasePbufferDCARB;
PFNWGLDESTROYPBUFFERARBPROC	wglDestroyPbufferARB;
PFNWGLQUERYPBUFFERARBPROC	wglQueryPbufferARB;

// WGL_ARB_render_texture 
PFNWGLBINDTEXIMAGEARBPROC		wglBindTexImageARB;
PFNWGLRELEASETEXIMAGEARBPROC	wglReleaseTexImageARB;
PFNWGLSETPBUFFERATTRIBARBPROC	wglSetPbufferAttribARB;



/* ARB_pixel_format */
#define WGL_NUMBER_PIXEL_FORMATS_ARB       0x2000
#define WGL_DRAW_TO_WINDOW_ARB             0x2001
#define WGL_DRAW_TO_BITMAP_ARB             0x2002
#define WGL_ACCELERATION_ARB               0x2003
#define WGL_NEED_PALETTE_ARB               0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB        0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB         0x2006
#define WGL_SWAP_METHOD_ARB                0x2007
#define WGL_NUMBER_OVERLAYS_ARB            0x2008
#define WGL_NUMBER_UNDERLAYS_ARB           0x2009
#define WGL_TRANSPARENT_ARB                0x200A
#define WGL_SHARE_DEPTH_ARB                0x200C
#define WGL_SHARE_STENCIL_ARB              0x200D
#define WGL_SHARE_ACCUM_ARB                0x200E
#define WGL_SUPPORT_GDI_ARB                0x200F
#define WGL_SUPPORT_OPENGL_ARB             0x2010
#define WGL_DOUBLE_BUFFER_ARB              0x2011
#define WGL_STEREO_ARB                     0x2012
#define WGL_PIXEL_TYPE_ARB                 0x2013
#define WGL_COLOR_BITS_ARB                 0x2014
#define WGL_RED_BITS_ARB                   0x2015
#define WGL_RED_SHIFT_ARB                  0x2016
#define WGL_GREEN_BITS_ARB                 0x2017
#define WGL_GREEN_SHIFT_ARB                0x2018
#define WGL_BLUE_BITS_ARB                  0x2019
#define WGL_BLUE_SHIFT_ARB                 0x201A
#define WGL_ALPHA_BITS_ARB                 0x201B
#define WGL_ALPHA_SHIFT_ARB                0x201C
#define WGL_ACCUM_BITS_ARB                 0x201D
#define WGL_ACCUM_RED_BITS_ARB             0x201E
#define WGL_ACCUM_GREEN_BITS_ARB           0x201F
#define WGL_ACCUM_BLUE_BITS_ARB            0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB           0x2021
#define WGL_DEPTH_BITS_ARB                 0x2022
#define WGL_STENCIL_BITS_ARB               0x2023
#define WGL_AUX_BUFFERS_ARB                0x2024
#define WGL_NO_ACCELERATION_ARB            0x2025
#define WGL_GENERIC_ACCELERATION_ARB       0x2026
#define WGL_FULL_ACCELERATION_ARB          0x2027
#define WGL_SWAP_EXCHANGE_ARB              0x2028
#define WGL_SWAP_COPY_ARB                  0x2029
#define WGL_SWAP_UNDEFINED_ARB             0x202A
#define WGL_TYPE_RGBA_ARB                  0x202B
#define WGL_TYPE_COLORINDEX_ARB            0x202C
#define WGL_TRANSPARENT_RED_VALUE_ARB      0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB    0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB     0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB    0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB    0x203B

/* ARB_multisample */
#define WGL_SAMPLE_BUFFERS_ARB             0x2041
#define WGL_SAMPLES_ARB                    0x2042

// Adds raw mouse input.
idCVar m_rawInput("m_rawinput",	"0", CVAR_BOOL,	"use raw input value : 0 : 1");


//
// function declaration
//
bool QGL_Init( const char *dllname );
void     QGL_Shutdown( void );



/*
========================
GLimp_GetOldGammaRamp
========================
*/
static void GLimp_SaveGamma( void ) {
	HDC			hDC;
	BOOL		success;

	hDC = GetDC( GetDesktopWindow() );
	success = GetDeviceGammaRamp( hDC, win32.oldHardwareGamma );
	common->DPrintf( "...getting default gamma ramp: %s\n", success ? "success" : "failed" );
	ReleaseDC( GetDesktopWindow(), hDC );
}

/*
========================
GLimp_RestoreGamma
========================
*/
static void GLimp_RestoreGamma( void ) {
	HDC hDC;
	BOOL success;

	// if we never read in a reasonable looking
	// table, don't write it out
	if ( win32.oldHardwareGamma[0][255] == 0 ) {
		return;
	}

	hDC = GetDC( GetDesktopWindow() );
	success = SetDeviceGammaRamp( hDC, win32.oldHardwareGamma );
	common->DPrintf ( "...restoring hardware gamma: %s\n", success ? "success" : "failed" );
	ReleaseDC( GetDesktopWindow(), hDC );
}


/*
========================
GLimp_SetGamma

The renderer calls this when the user adjusts r_gamma or r_brightness
========================
*/
void GLimp_SetGamma( unsigned short red[256], unsigned short green[256], unsigned short blue[256] ) {
	unsigned short table[3][256];
	int i;

	if ( !win32.hDC ) {
		return;
	}

	for ( i = 0; i < 256; i++ ) {
		table[0][i] = red[i];
		table[1][i] = green[i];
		table[2][i] = blue[i];
	}

	if ( !SetDeviceGammaRamp( win32.hDC, table ) ) {
		common->Printf( "WARNING: SetDeviceGammaRamp failed.\n" );
	}
}

/*
==================
GLW_GetWGLExtensionsWithFakeWindow
==================
*/
void GLW_CheckWGLExtensions( HDC hDC ) {
	wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)
							  GLimp_ExtensionPointer("wglGetExtensionsStringARB");
	if ( wglGetExtensionsStringARB ) {
		glConfig.wgl_extensions_string = (const char *) wglGetExtensionsStringARB(hDC);
	} else {
		glConfig.wgl_extensions_string = "";
	}

	// WGL_EXT_swap_control
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) GLimp_ExtensionPointer( "wglSwapIntervalEXT" );
	r_swapInterval.SetModified();	// force a set next frame

	// WGL_ARB_pixel_format
	wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)GLimp_ExtensionPointer("wglGetPixelFormatAttribivARB");
	wglGetPixelFormatAttribfvARB = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC)GLimp_ExtensionPointer("wglGetPixelFormatAttribfvARB");
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)GLimp_ExtensionPointer("wglChoosePixelFormatARB");

	// WGL_ARB_pbuffer
	wglCreatePbufferARB = (PFNWGLCREATEPBUFFERARBPROC)GLimp_ExtensionPointer("wglCreatePbufferARB");
	wglGetPbufferDCARB = (PFNWGLGETPBUFFERDCARBPROC)GLimp_ExtensionPointer("wglGetPbufferDCARB");
	wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC)GLimp_ExtensionPointer("wglReleasePbufferDCARB");
	wglDestroyPbufferARB = (PFNWGLDESTROYPBUFFERARBPROC)GLimp_ExtensionPointer("wglDestroyPbufferARB");
	wglQueryPbufferARB = (PFNWGLQUERYPBUFFERARBPROC)GLimp_ExtensionPointer("wglQueryPbufferARB");

	// WGL_ARB_render_texture 
	wglBindTexImageARB = (PFNWGLBINDTEXIMAGEARBPROC)GLimp_ExtensionPointer("wglBindTexImageARB");
	wglReleaseTexImageARB = (PFNWGLRELEASETEXIMAGEARBPROC)GLimp_ExtensionPointer("wglReleaseTexImageARB");
	wglSetPbufferAttribARB = (PFNWGLSETPBUFFERATTRIBARBPROC)GLimp_ExtensionPointer("wglSetPbufferAttribARB");
}

static void MouseKey_Callback(GLFWwindow* window, int button, int action, int mods)
{
	if(action == GLFW_PRESS || action == GLFW_RELEASE)
	{
		int key;
		int state = action == GLFW_PRESS ? 1 : 0;

		// UI seems to use K_MOUSE1 instead of M_ACTION1
		if(UIActive)
		{
			switch(button)
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
			switch(button)
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

		mouse_polls.Append(mouse_poll_t(key, state));
		Sys_QueEvent(GetTickCount(), SE_KEY, key, state, 0, NULL);
	}
}

static void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Backspace needs to be handled here.
	if(key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
	{
		Sys_QueEvent(GetTickCount(), SE_CHAR, K_BACKSPACE, 0, 0, NULL);
		return;
	}

	if(action == GLFW_PRESS || action == GLFW_RELEASE)
	{
		bool state = (action == GLFW_PRESS) ? 1 : 0;
		keyboard_polls.Append(keyboard_poll_t(key, state));

		Sys_QueEvent(GetTickCount(), SE_KEY, GLFWDoom_MapKey(key), state, 0, NULL);
	}
}

static void Cursor_Callback(GLFWwindow* window, double xpos, double ypos)
{
	// We use different mouse styles to allow for better mouse handling for UI.
	if(UIActive)
	{
		Sys_QueEvent( GetTickCount(), SE_MOUSE_ABS, xpos, ypos, 0, NULL );
	}
	else
	{	
		mouse_polls.Append(mouse_poll_t(M_DELTAX, xpos));
		mouse_polls.Append(mouse_poll_t(M_DELTAY, ypos));
		Sys_QueEvent( GetTickCount(), SE_MOUSE, xpos, ypos, 0, NULL );

		glfwSetCursorPos(window, 0, 0);
	}
	
}

static void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//Has to be done this way otherwise we get funky skipping.
	if(yoffset >= 1)
	{
		mouse_polls.Append(mouse_poll_t(M_DELTAZ, 1));
		Sys_QueEvent( GetTickCount(), SE_MOUSE, 1, 0, 0, NULL );
	}
	else if (yoffset <= -1)
	{
		mouse_polls.Append(mouse_poll_t(M_DELTAZ, -1));
		Sys_QueEvent( GetTickCount(), SE_MOUSE, -1, 0, 0, NULL );
	}

}

static void Character_Callback(GLFWwindow* window, unsigned int codepoint)
{
	// We convert the text to UTF-8
	char text[5] = { 0 };
    if (codepoint <= 0x7F) {
        text[0] = static_cast<char>(codepoint);
    } else if (codepoint <= 0x7FF) {
        text[0] = static_cast<char>(0xC0 | (codepoint >> 6));
        text[1] = static_cast<char>(0x80 | (codepoint & 0x3F));
    } else if (codepoint <= 0xFFFF) {
        text[0] = static_cast<char>(0xE0 | (codepoint >> 12));
        text[1] = static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        text[2] = static_cast<char>(0x80 | (codepoint & 0x3F));
    } else if (codepoint <= 0x10FFFF) {
        text[0] = static_cast<char>(0xF0 | (codepoint >> 18));
        text[1] = static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
        text[2] = static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        text[3] = static_cast<char>(0x80 | (codepoint & 0x3F));
    }

	Sys_QueEvent( GetTickCount(), SE_CHAR, (unsigned char)text[0], 0, 0, NULL );
}
/*
===================
GLimp_Init

This creates the GLFW window and inits GLEW.
The window always created in full screen.
===================
*/
bool GLimp_Init( glimpParms_t parms ) {
	common->Printf( "Initializing OpenGL subsystem\n" );

	if(!glfwInit())
	{
		common->Error( "Could not start GLFW\n" );
		return false;
	}

	// full screen window
	if(parms.fullScreen)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		//We need this to set our refresh rate correctly.
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		window = glfwCreateWindow(parms.width, parms.height, "Doom 3", glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		window = glfwCreateWindow(parms.width, parms.height, "Doom 3", NULL, NULL);
	}
	
    if (!window)
    {
        glfwTerminate();
        return false;
    }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); 

	//Disable resizing.
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

	glfwMakeContextCurrent(window);

	glewInit();

	// Raw mouse input
	if(m_rawInput.GetBool())
	{
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, 0, 0);

	//Toggle V-Sync
	if(r_swapInterval.GetInteger() == 1)
	{	
		glfwSwapInterval(1);
	}
	else
	{
		glfwSwapInterval(0);
	}

	//MSAA samples :)
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
bool GLimp_SetScreenParms( glimpParms_t parms ) {
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwSetWindowMonitor(window, monitor, 0, 0, parms.width, parms.height, parms.displayHz);
	return true;
}

/*
===================
GLimp_Shutdown

This routine does all OS specific shutdown procedures for the OpenGL
subsystem.
===================
*/
void GLimp_Shutdown( void ) {
	common->Printf( "Shutting down OpenGL subsystem\n" );

	glfwDestroyWindow(window);

	// restore gamma
	GLimp_RestoreGamma();

	// shutdown QGL subsystem
	// QGL_Shutdown();
}


/*
=====================
GLimp_SwapBuffers
=====================
*/
void GLimp_SwapBuffers( void ) {
	glfwSwapBuffers(window);
	// 
}

/*
===========================================================

SMP acceleration

===========================================================
*/

//#define	REALLOC_DC

/*
===================
GLimp_ActivateContext

===================
*/
void GLimp_ActivateContext( void ) {
	// if ( !qwglMakeCurrent( win32.hDC, win32.hGLRC ) ) {
	// 	win32.wglErrors++;
	// }
}

/*
===================
GLimp_DeactivateContext

===================
*/
void GLimp_DeactivateContext( void ) {
	glFinish();
	// if ( !qwglMakeCurrent( win32.hDC, NULL ) ) {
	// 	win32.wglErrors++;
	// }
#ifdef REALLOC_DC
	// makeCurrent NULL frees the DC, so get another
	if ( ( win32.hDC = GetDC( win32.hWnd ) ) == NULL ) {
		win32.wglErrors++;
	}
#endif

}

/*
===================
GLimp_RenderThreadWrapper

===================
*/
static void GLimp_RenderThreadWrapper( void ) {
	win32.glimpRenderThread();

	// unbind the context before we die
	// qwglMakeCurrent( win32.hDC, NULL );
}

/*
=======================
GLimp_SpawnRenderThread

Returns false if the system only has a single processor
=======================
*/
bool GLimp_SpawnRenderThread( void (*function)( void ) ) {
	SYSTEM_INFO info;

	// check number of processors
	GetSystemInfo( &info );
	if ( info.dwNumberOfProcessors < 2 ) {
		return false;
	}
	
	// create the IPC elements
	win32.renderCommandsEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	win32.renderCompletedEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	win32.renderActiveEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	win32.glimpRenderThread = function;

	win32.renderThreadHandle = CreateThread(
	   NULL,	// LPSECURITY_ATTRIBUTES lpsa,
	   0,		// DWORD cbStack,
	   (LPTHREAD_START_ROUTINE)GLimp_RenderThreadWrapper,	// LPTHREAD_START_ROUTINE lpStartAddr,
	   0,			// LPVOID lpvThreadParm,
	   0,			//   DWORD fdwCreate,
	   &win32.renderThreadId );

	if ( !win32.renderThreadHandle ) {
		common->Error( "GLimp_SpawnRenderThread: failed" );
	}

	SetThreadPriority( win32.renderThreadHandle, THREAD_PRIORITY_ABOVE_NORMAL );
#if 0
	// make sure they always run on different processors
	SetThreadAffinityMask( GetCurrentThread, 1 );
	SetThreadAffinityMask( win32.renderThreadHandle, 2 );
#endif

	return true;
}


//#define	DEBUG_PRINTS

/*
===================
GLimp_BackEndSleep

===================
*/
void *GLimp_BackEndSleep( void ) {
	void	*data;

#ifdef DEBUG_PRINTS
OutputDebugString( "-->GLimp_BackEndSleep\n" );
#endif
	ResetEvent( win32.renderActiveEvent );

	// after this, the front end can exit GLimp_FrontEndSleep
	SetEvent( win32.renderCompletedEvent );

	WaitForSingleObject( win32.renderCommandsEvent, INFINITE );

	ResetEvent( win32.renderCompletedEvent );
	ResetEvent( win32.renderCommandsEvent );

	data = win32.smpData;

	// after this, the main thread can exit GLimp_WakeRenderer
	SetEvent( win32.renderActiveEvent );

#ifdef DEBUG_PRINTS
OutputDebugString( "<--GLimp_BackEndSleep\n" );
#endif
	return data;
}

/*
===================
GLimp_FrontEndSleep

===================
*/
void GLimp_FrontEndSleep( void ) {
#ifdef DEBUG_PRINTS
OutputDebugString( "-->GLimp_FrontEndSleep\n" );
#endif
	WaitForSingleObject( win32.renderCompletedEvent, INFINITE );

#ifdef DEBUG_PRINTS
OutputDebugString( "<--GLimp_FrontEndSleep\n" );
#endif
}

volatile bool	renderThreadActive;

/*
===================
GLimp_WakeBackEnd

===================
*/
void GLimp_WakeBackEnd( void *data ) {
	int		r;

#ifdef DEBUG_PRINTS
OutputDebugString( "-->GLimp_WakeBackEnd\n" );
#endif
	win32.smpData = data;

	if ( renderThreadActive ) {
		common->FatalError( "GLimp_WakeBackEnd: already active" );
	}

	r = WaitForSingleObject( win32.renderActiveEvent, 0 );
	if ( r == WAIT_OBJECT_0 ) {
		common->FatalError( "GLimp_WakeBackEnd: already signaled" );
	}

	r = WaitForSingleObject( win32.renderCommandsEvent, 0 );
	if ( r == WAIT_OBJECT_0 ) {
		common->FatalError( "GLimp_WakeBackEnd: commands already signaled" );
	}

	// after this, the renderer can continue through GLimp_RendererSleep
	SetEvent( win32.renderCommandsEvent );

	r = WaitForSingleObject( win32.renderActiveEvent, 5000 );

	if ( r == WAIT_TIMEOUT ) {
		common->FatalError( "GLimp_WakeBackEnd: WAIT_TIMEOUT" );
	}

#ifdef DEBUG_PRINTS
OutputDebugString( "<--GLimp_WakeBackEnd\n" );
#endif
}

//===================================================================

/*
===================
GLimp_ExtensionPointer

Returns a function pointer for an OpenGL extension entry point
===================
*/
GLExtension_t GLimp_ExtensionPointer( const char *name ) {
	void	(*proc)(void);

	// proc = (GLExtension_t)glewget( name );
	proc = glfwGetProcAddress(name);

	if ( !proc ) {
		common->Printf( "Couldn't find proc address for: %s\n", name );
	}

	return proc;
}

