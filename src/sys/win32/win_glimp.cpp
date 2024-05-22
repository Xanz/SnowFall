/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").  

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

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
#pragma hdrstop
#include "../../idlib/precompiled.h"

#include "win_local.h"
#include "rc/doom_resource.h"
#include "../../renderer/tr_local.h"


idCVar r_useOpenGL32( "r_useOpenGL32", "2", CVAR_INTEGER, "0 = OpenGL 2.0, 1 = OpenGL 3.2 compatibility profile, 2 = OpenGL 3.2 core profile", 0, 2 );

// Adds raw mouse input.
idCVar m_rawInput("m_rawinput",	"1", CVAR_BOOL,	"use raw input value : 0 : 1");

//
// function declaration
//
bool QGL_Init( const char *dllname );
void QGL_Shutdown();


/*
========================
GLimp_TestSwapBuffers
========================
*/
void GLimp_TestSwapBuffers( const idCmdArgs &args ) {
	idLib::Printf( "GLimp_TimeSwapBuffers\n" );
	static const int MAX_FRAMES = 5;
	uint64	timestamps[MAX_FRAMES];
	glDisable( GL_SCISSOR_TEST );

	int frameMilliseconds = 16;
	for ( int swapInterval = 2 ; swapInterval >= -1 ; swapInterval-- ) {
		wglSwapIntervalEXT( swapInterval );
		for ( int i = 0 ; i < MAX_FRAMES ; i++ ) {
			if ( swapInterval == -1 ) {
				Sys_Sleep( frameMilliseconds );
			}
			if ( i & 1 ) {
				glClearColor( 0, 1, 0, 1 );
			} else {
				glClearColor( 1, 0, 0, 1 );
			}
			glClear( GL_COLOR_BUFFER_BIT );
			glfwSwapBuffers( window );
			glFinish();
			timestamps[i] = Sys_Microseconds();
		}

		idLib::Printf( "\nswapinterval %i\n", swapInterval );
		for ( int i = 1 ; i < MAX_FRAMES ; i++ ) {
			idLib::Printf( "%i microseconds\n", (int)(timestamps[i] - timestamps[i-1]) );
		}
	}
}

/*
========================
GLimp_GetOldGammaRamp
========================
*/
static void GLimp_SaveGamma() {
}

/*
========================
GLimp_RestoreGamma
========================
*/
static void GLimp_RestoreGamma() {
}


/*
========================
GLimp_SetGamma

The renderer calls this when the user adjusts r_gamma or r_brightness
========================
*/
void GLimp_SetGamma( unsigned short red[256], unsigned short green[256], unsigned short blue[256] ) {
	// unsigned short table[3][256];
	// int i;

	// if ( !win32.hDC ) {
	// 	return;
	// }

	// for ( i = 0; i < 256; i++ ) {
	// 	table[0][i] = red[i];
	// 	table[1][i] = green[i];
	// 	table[2][i] = blue[i];
	// }

	// if ( !SetDeviceGammaRamp( win32.hDC, table ) ) {
	// 	common->Printf( "WARNING: SetDeviceGammaRamp failed.\n" );
	// }
}

/*
=============================================================================

WglExtension Grabbing

This is gross -- creating a window just to get a context to get the wgl extensions

=============================================================================
*/

/*
========================
R_CheckWinExtension
========================
*/
bool R_CheckWinExtension( const char * name ) {

	if ( !strstr( glConfig.wgl_extensions_string, name ) ) {
		idLib::Printf( "X..%s not found\n", name );
		return false;
	}

	idLib::Printf( "...using %s\n", name );
	return true;
}
/*
====================
GLW_ChoosePixelFormat

Returns -1 on failure, or a pixel format
====================
*/
static int GLW_ChoosePixelFormat( const HDC hdc, const int multisamples, const bool stereo3D ) {
	FLOAT	fAttributes[] = { 0, 0 };
	int		iAttributes[] = {
		WGL_SAMPLE_BUFFERS_ARB, ( ( multisamples > 1 ) ? 1 : 0 ),
		WGL_SAMPLES_ARB, multisamples,
		WGL_DOUBLE_BUFFER_ARB, TRUE,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_RED_BITS_ARB, 8,
		WGL_BLUE_BITS_ARB, 8,
		WGL_GREEN_BITS_ARB, 8,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_STEREO_ARB, ( stereo3D ? TRUE : FALSE ),
		0, 0
	};

	int	pixelFormat;
	UINT numFormats;
	if ( !wglChoosePixelFormatARB( hdc, iAttributes, fAttributes, 1, &pixelFormat, &numFormats ) ) {
		return -1;
	}
	return pixelFormat;
}


/*
========================
GetDisplayName
========================
*/
static const char * GetDisplayName( const int deviceNum ) {
	static DISPLAY_DEVICE	device;
	device.cb = sizeof( device );
	if ( !EnumDisplayDevices(
			0,			// lpDevice
			deviceNum,
			&device,
			0 /* dwFlags */ ) ) {
		return NULL;
	}
	return device.DeviceName;
}

/*
========================
GetDeviceName
========================
*/
static idStr GetDeviceName( const int deviceNum ) {
	DISPLAY_DEVICE	device = {};
	device.cb = sizeof( device );
	if ( !EnumDisplayDevices(
			0,			// lpDevice
			deviceNum,
			&device,
			0 /* dwFlags */ ) ) {
		return false;
	}

	// get the monitor for this display
	if ( ! (device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP ) ) {
		return false;
	}

	return idStr( device.DeviceName );
}

/*
========================
GetDisplayCoordinates
========================
*/
static bool GetDisplayCoordinates( const int deviceNum, int & x, int & y, int & width, int & height, int & displayHz ) {
	idStr deviceName = GetDeviceName( deviceNum );
	if ( deviceName.Length() == 0 ) {
		return false;
	}

	DISPLAY_DEVICE	device = {};
	device.cb = sizeof( device );
	if ( !EnumDisplayDevices(
			0,			// lpDevice
			deviceNum,
			&device,
			0 /* dwFlags */ ) ) {
		return false;
	}

	DISPLAY_DEVICE	monitor;
	monitor.cb = sizeof( monitor );
	if ( !EnumDisplayDevices(
			deviceName.c_str(),
			0,
			&monitor,
			0 /* dwFlags */ ) ) {
		return false;
	}

	DEVMODE	devmode;
	devmode.dmSize = sizeof( devmode );
	if ( !EnumDisplaySettings( deviceName.c_str(),ENUM_CURRENT_SETTINGS, &devmode ) ) {
		return false;
	}

	common->Printf( "display device: %i\n", deviceNum );
	common->Printf( "  DeviceName  : %s\n", device.DeviceName );
	common->Printf( "  DeviceString: %s\n", device.DeviceString );
	common->Printf( "  StateFlags  : 0x%x\n", device.StateFlags );
	common->Printf( "  DeviceID    : %s\n", device.DeviceID );
	common->Printf( "  DeviceKey   : %s\n", device.DeviceKey );
	common->Printf( "      DeviceName  : %s\n", monitor.DeviceName );
	common->Printf( "      DeviceString: %s\n", monitor.DeviceString );
	common->Printf( "      StateFlags  : 0x%x\n", monitor.StateFlags );
	common->Printf( "      DeviceID    : %s\n", monitor.DeviceID );
	common->Printf( "      DeviceKey   : %s\n", monitor.DeviceKey );
	common->Printf( "          dmPosition.x      : %i\n", devmode.dmPosition.x );
	common->Printf( "          dmPosition.y      : %i\n", devmode.dmPosition.y );
	common->Printf( "          dmBitsPerPel      : %i\n", devmode.dmBitsPerPel );
	common->Printf( "          dmPelsWidth       : %i\n", devmode.dmPelsWidth );
	common->Printf( "          dmPelsHeight      : %i\n", devmode.dmPelsHeight );
	common->Printf( "          dmDisplayFlags    : 0x%x\n", devmode.dmDisplayFlags );
	common->Printf( "          dmDisplayFrequency: %i\n", devmode.dmDisplayFrequency );

	x = devmode.dmPosition.x;
	y = devmode.dmPosition.y;
	width = devmode.dmPelsWidth;
	height = devmode.dmPelsHeight;
	displayHz = devmode.dmDisplayFrequency;

	return true;
}

/*
====================
DMDFO
====================
*/
static const char * DMDFO( int dmDisplayFixedOutput ) {
	switch( dmDisplayFixedOutput ) {
	case DMDFO_DEFAULT: return "DMDFO_DEFAULT";
	case DMDFO_CENTER: return "DMDFO_CENTER";
	case DMDFO_STRETCH: return "DMDFO_STRETCH";
	}
	return "UNKNOWN";
}

/*
====================
PrintDevMode
====================
*/
static void PrintDevMode( DEVMODE & devmode ) {
	common->Printf( "          dmPosition.x        : %i\n", devmode.dmPosition.x );
	common->Printf( "          dmPosition.y        : %i\n", devmode.dmPosition.y );
	common->Printf( "          dmBitsPerPel        : %i\n", devmode.dmBitsPerPel );
	common->Printf( "          dmPelsWidth         : %i\n", devmode.dmPelsWidth );
	common->Printf( "          dmPelsHeight        : %i\n", devmode.dmPelsHeight );
	common->Printf( "          dmDisplayFixedOutput: %s\n", DMDFO( devmode.dmDisplayFixedOutput ) );
	common->Printf( "          dmDisplayFlags      : 0x%x\n", devmode.dmDisplayFlags );
	common->Printf( "          dmDisplayFrequency  : %i\n", devmode.dmDisplayFrequency );
}

/*
====================
DumpAllDisplayDevices
====================
*/
void DumpAllDisplayDevices() {
	common->Printf( "\n" );
	for ( int deviceNum = 0 ; ; deviceNum++ ) {
		DISPLAY_DEVICE	device = {};
		device.cb = sizeof( device );
		if ( !EnumDisplayDevices(
				0,			// lpDevice
				deviceNum,
				&device,
				0 /* dwFlags */ ) ) {
			break;
		}

		common->Printf( "display device: %i\n", deviceNum );
		common->Printf( "  DeviceName  : %s\n", device.DeviceName );
		common->Printf( "  DeviceString: %s\n", device.DeviceString );
		common->Printf( "  StateFlags  : 0x%x\n", device.StateFlags );
		common->Printf( "  DeviceID    : %s\n", device.DeviceID );
		common->Printf( "  DeviceKey   : %s\n", device.DeviceKey );

		for ( int monitorNum = 0 ; ; monitorNum++ ) {
			DISPLAY_DEVICE	monitor = {};
			monitor.cb = sizeof( monitor );
			if ( !EnumDisplayDevices(
					device.DeviceName,
					monitorNum,
					&monitor,
					0 /* dwFlags */ ) ) {
				break;
			}

			common->Printf( "      DeviceName  : %s\n", monitor.DeviceName );
			common->Printf( "      DeviceString: %s\n", monitor.DeviceString );
			common->Printf( "      StateFlags  : 0x%x\n", monitor.StateFlags );
			common->Printf( "      DeviceID    : %s\n", monitor.DeviceID );
			common->Printf( "      DeviceKey   : %s\n", monitor.DeviceKey );

			DEVMODE	currentDevmode = {};
			if ( !EnumDisplaySettings( device.DeviceName,ENUM_CURRENT_SETTINGS, &currentDevmode ) ) {
				common->Printf( "ERROR:  EnumDisplaySettings(ENUM_CURRENT_SETTINGS) failed!\n" );
			}
			common->Printf( "          -------------------\n" );
			common->Printf( "          ENUM_CURRENT_SETTINGS\n" );
			PrintDevMode( currentDevmode );

			DEVMODE	registryDevmode = {};
			if ( !EnumDisplaySettings( device.DeviceName,ENUM_REGISTRY_SETTINGS, &registryDevmode ) ) {
				common->Printf( "ERROR:  EnumDisplaySettings(ENUM_CURRENT_SETTINGS) failed!\n" );
			}
			common->Printf( "          -------------------\n" );
			common->Printf( "          ENUM_CURRENT_SETTINGS\n" );
			PrintDevMode( registryDevmode );

			for ( int modeNum = 0 ; ; modeNum++ ) {
				DEVMODE	devmode = {};

				if ( !EnumDisplaySettings( device.DeviceName,modeNum, &devmode ) ) {
					break;
				}

				if ( devmode.dmBitsPerPel != 32 ) {
					continue;
				}
				if ( devmode.dmDisplayFrequency < 60 ) {
					continue;
				}
				if ( devmode.dmPelsHeight < 720 ) {
					continue;
				}
				common->Printf( "          -------------------\n" );
				common->Printf( "          modeNum             : %i\n", modeNum );
				PrintDevMode( devmode );
			}
		}
	}
	common->Printf( "\n" );
}

/*
====================
R_GetModeListForDisplay

Returns a list of modes for resolution selection.
====================
*/
bool R_GetModeListForDisplay( const int requestedDisplayNum, idList<vidMode_t> & modeList ) {
	modeList.Clear();

	bool	verbose = false;

	for ( int displayNum = requestedDisplayNum; ; displayNum++ ) {
		DISPLAY_DEVICE	device;
		device.cb = sizeof( device );
		if ( !EnumDisplayDevices(
				0,			// lpDevice
				displayNum,
				&device,
				0 /* dwFlags */ ) ) {
			return false;
		}

		// get the monitor for this display
		if ( ! (device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP ) ) {
			continue;
		}

		DISPLAY_DEVICE	monitor;
		monitor.cb = sizeof( monitor );
		if ( !EnumDisplayDevices(
				device.DeviceName,
				0,
				&monitor,
				0 /* dwFlags */ ) ) {
			continue;
		}

		DEVMODE	devmode;
		devmode.dmSize = sizeof( devmode );

		if ( verbose ) {
			common->Printf( "display device: %i\n", displayNum );
			common->Printf( "  DeviceName  : %s\n", device.DeviceName );
			common->Printf( "  DeviceString: %s\n", device.DeviceString );
			common->Printf( "  StateFlags  : 0x%x\n", device.StateFlags );
			common->Printf( "  DeviceID    : %s\n", device.DeviceID );
			common->Printf( "  DeviceKey   : %s\n", device.DeviceKey );
			common->Printf( "      DeviceName  : %s\n", monitor.DeviceName );
			common->Printf( "      DeviceString: %s\n", monitor.DeviceString );
			common->Printf( "      StateFlags  : 0x%x\n", monitor.StateFlags );
			common->Printf( "      DeviceID    : %s\n", monitor.DeviceID );
			common->Printf( "      DeviceKey   : %s\n", monitor.DeviceKey );
		}

		for ( int modeNum = 0 ; ; modeNum++ ) {
			if ( !EnumDisplaySettings( device.DeviceName,modeNum, &devmode ) ) {
				break;
			}

			if ( devmode.dmBitsPerPel != 32 ) {
				continue;
			}
			if ( ( devmode.dmDisplayFrequency != 60 ) && ( devmode.dmDisplayFrequency != 120 ) ) {
				continue;
			}
			if ( devmode.dmPelsHeight < 720 ) {
				continue;
			}
			if ( verbose ) {
				common->Printf( "          -------------------\n" );
				common->Printf( "          modeNum             : %i\n", modeNum );
				common->Printf( "          dmPosition.x        : %i\n", devmode.dmPosition.x );
				common->Printf( "          dmPosition.y        : %i\n", devmode.dmPosition.y );
				common->Printf( "          dmBitsPerPel        : %i\n", devmode.dmBitsPerPel );
				common->Printf( "          dmPelsWidth         : %i\n", devmode.dmPelsWidth );
				common->Printf( "          dmPelsHeight        : %i\n", devmode.dmPelsHeight );
				common->Printf( "          dmDisplayFixedOutput: %s\n", DMDFO( devmode.dmDisplayFixedOutput ) );
				common->Printf( "          dmDisplayFlags      : 0x%x\n", devmode.dmDisplayFlags );
				common->Printf( "          dmDisplayFrequency  : %i\n", devmode.dmDisplayFrequency );
			}
			vidMode_t mode;
			mode.width = devmode.dmPelsWidth;
			mode.height = devmode.dmPelsHeight;
			mode.displayHz = devmode.dmDisplayFrequency;
			modeList.AddUnique( mode );
		}
		if ( modeList.Num() > 0 ) {

			class idSort_VidMode : public idSort_Quick< vidMode_t, idSort_VidMode > {
			public:
				int Compare( const vidMode_t & a, const vidMode_t & b ) const {
					int wd = a.width - b.width;
					int hd = a.height - b.height;
					int fd = a.displayHz - b.displayHz;
					return ( hd != 0 ) ? hd : ( wd != 0 ) ? wd : fd;
				}
			};

			// sort with lowest resolution first
			modeList.SortWithTemplate( idSort_VidMode() );

			return true;
		}
	}
	// Never gets here
}


/*
=======
MapKey

Map from GLFW to Doom keynums
=======
*/
int GLFWDoom_MapKey (int key) {
    switch (key) {
        case GLFW_KEY_ESCAPE: return K_ESCAPE;
        case GLFW_KEY_1: return K_1;
        case GLFW_KEY_2: return K_2;
        case GLFW_KEY_3: return K_3;
        case GLFW_KEY_4: return K_4;
        case GLFW_KEY_5: return K_5;
        case GLFW_KEY_6: return K_6;
        case GLFW_KEY_7: return K_7;
        case GLFW_KEY_8: return K_8;
        case GLFW_KEY_9: return K_9;
        case GLFW_KEY_0: return K_0;
        case GLFW_KEY_MINUS: return K_MINUS;
        case GLFW_KEY_EQUAL: return K_EQUALS;
        case GLFW_KEY_BACKSPACE: return K_BACKSPACE;
        case GLFW_KEY_TAB: return K_TAB;
        case GLFW_KEY_Q: return K_Q;
        case GLFW_KEY_W: return K_W;
        case GLFW_KEY_E: return K_E;
        case GLFW_KEY_R: return K_R;
        case GLFW_KEY_T: return K_T;
        case GLFW_KEY_Y: return K_Y;
        case GLFW_KEY_U: return K_U;
        case GLFW_KEY_I: return K_I;
        case GLFW_KEY_O: return K_O;
        case GLFW_KEY_P: return K_P;
        case GLFW_KEY_LEFT_BRACKET: return K_LBRACKET;
        case GLFW_KEY_RIGHT_BRACKET: return K_RBRACKET;
        case GLFW_KEY_ENTER: return K_ENTER;
        case GLFW_KEY_LEFT_CONTROL: return K_LCTRL;
        case GLFW_KEY_A: return K_A;
        case GLFW_KEY_S: return K_S;
        case GLFW_KEY_D: return K_D;
        case GLFW_KEY_F: return K_F;
        case GLFW_KEY_G: return K_G;
        case GLFW_KEY_H: return K_H;
        case GLFW_KEY_J: return K_J;
        case GLFW_KEY_K: return K_K;
        case GLFW_KEY_L: return K_L;
        case GLFW_KEY_SEMICOLON: return K_SEMICOLON;
        case GLFW_KEY_APOSTROPHE: return K_APOSTROPHE;
        case GLFW_KEY_GRAVE_ACCENT: return K_GRAVE;
        case GLFW_KEY_LEFT_SHIFT: return K_LSHIFT;
        case GLFW_KEY_BACKSLASH: return K_BACKSLASH;
        case GLFW_KEY_Z: return K_Z;
        case GLFW_KEY_X: return K_X;
        case GLFW_KEY_C: return K_C;
        case GLFW_KEY_V: return K_V;
        case GLFW_KEY_B: return K_B;
        case GLFW_KEY_N: return K_N;
        case GLFW_KEY_M: return K_M;
        case GLFW_KEY_COMMA: return K_COMMA;
        case GLFW_KEY_PERIOD: return K_PERIOD;
        case GLFW_KEY_SLASH: return K_SLASH;
        case GLFW_KEY_RIGHT_SHIFT: return K_RSHIFT;
        case GLFW_KEY_KP_MULTIPLY: return K_KP_STAR;
        case GLFW_KEY_LEFT_ALT: return K_LALT;
        case GLFW_KEY_SPACE: return K_SPACE;
        case GLFW_KEY_CAPS_LOCK: return K_CAPSLOCK;
        case GLFW_KEY_F1: return K_F1;
        case GLFW_KEY_F2: return K_F2;
        case GLFW_KEY_F3: return K_F3;
        case GLFW_KEY_F4: return K_F4;
        case GLFW_KEY_F5: return K_F5;
        case GLFW_KEY_F6: return K_F6;
        case GLFW_KEY_F7: return K_F7;
        case GLFW_KEY_F8: return K_F8;
        case GLFW_KEY_F9: return K_F9;
        case GLFW_KEY_F10: return K_F10;
        case GLFW_KEY_NUM_LOCK: return K_NUMLOCK;
        case GLFW_KEY_SCROLL_LOCK: return K_SCROLL;
        case GLFW_KEY_KP_7: return K_KP_7;
        case GLFW_KEY_KP_8: return K_KP_8;
        case GLFW_KEY_KP_9: return K_KP_9;
        case GLFW_KEY_KP_SUBTRACT: return K_KP_MINUS;
        case GLFW_KEY_KP_4: return K_KP_4;
        case GLFW_KEY_KP_5: return K_KP_5;
        case GLFW_KEY_KP_6: return K_KP_6;
        case GLFW_KEY_KP_ADD: return K_KP_PLUS;
        case GLFW_KEY_KP_1: return K_KP_1;
        case GLFW_KEY_KP_2: return K_KP_2;
        case GLFW_KEY_KP_3: return K_KP_3;
        case GLFW_KEY_KP_0: return K_KP_0;
        case GLFW_KEY_KP_DECIMAL: return K_KP_DOT;
        case GLFW_KEY_F11: return K_F11;
        case GLFW_KEY_F12: return K_F12;
        case GLFW_KEY_PRINT_SCREEN: return K_PRINTSCREEN;
        case GLFW_KEY_PAUSE: return K_PAUSE;
        case GLFW_KEY_INSERT: return K_INS;
        case GLFW_KEY_HOME: return K_HOME;
        case GLFW_KEY_PAGE_UP: return K_PGUP;
        case GLFW_KEY_DELETE: return K_DEL;
        case GLFW_KEY_END: return K_END;
        case GLFW_KEY_PAGE_DOWN: return K_PGDN;
        case GLFW_KEY_RIGHT: return K_RIGHTARROW;
        case GLFW_KEY_LEFT: return K_LEFTARROW;
        case GLFW_KEY_DOWN: return K_DOWNARROW;
        case GLFW_KEY_UP: return K_UPARROW;
        case GLFW_KEY_KP_ENTER: return K_KP_ENTER;
        case GLFW_KEY_RIGHT_CONTROL: return K_RCTRL;
        case GLFW_KEY_KP_DIVIDE: return K_KP_SLASH;
        case GLFW_KEY_RIGHT_ALT: return K_RALT;
        case GLFW_KEY_LEFT_SUPER: return K_LWIN;
        case GLFW_KEY_RIGHT_SUPER: return K_RWIN;
        default: return K_NONE; // Undefined mapping
    }
}


static void MouseKey_Callback(GLFWwindow* window, int button, int action, int mods)
{
	if(action == GLFW_PRESS || action == GLFW_RELEASE)
	{
		int key;
		int state = action == GLFW_PRESS ? 1 : 0;

		// UI seems to use K_MOUSE1 instead of M_ACTION1
		if(!game->IsInGame() || game->IsPDAOpen())
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
		Sys_QueEvent(SE_KEY, key, state, 0, NULL, 0);
	}
}

static void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Backspace needs to be handled here.
	if(key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
	{
		Sys_QueEvent( SE_CHAR, K_BACKSPACE, 0, 0, NULL, 0);
		return;
	}

	if(action == GLFW_PRESS || action == GLFW_RELEASE)
	{
		bool state = (action == GLFW_PRESS) ? 1 : 0;
		keyboard_polls.Append(keyboard_poll_t(key, state));

		Sys_QueEvent(SE_KEY, GLFWDoom_MapKey(key), state, 0, NULL, 0);
	}
}

static void Cursor_Callback(GLFWwindow* window, double xpos, double ypos)
{
	// We use different mouse styles to allow for better mouse handling for UI.
	if(!game->IsInGame() || game->IsPDAOpen())
	{
		Sys_QueEvent( SE_MOUSE_ABSOLUTE, xpos, ypos, 0, NULL, 0);
	}
	else
	{	
		mouse_polls.Append(mouse_poll_t(M_DELTAX, xpos));
		mouse_polls.Append(mouse_poll_t(M_DELTAY, ypos));
		
		Sys_QueEvent( SE_MOUSE, xpos, ypos, 0, NULL, 0);

		glfwSetCursorPos(window, 0, 0);
	}
}

static void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//Has to be done this way otherwise we get funky skipping.
	if(yoffset >= 1)
	{
		mouse_polls.Append(mouse_poll_t(M_DELTAZ, 1));
		Sys_QueEvent( SE_KEY, K_MWHEELUP, 1, 0, NULL, 0 );
	}
	else if (yoffset <= -1)
	{
		mouse_polls.Append(mouse_poll_t(M_DELTAZ, -1));
		Sys_QueEvent( SE_KEY, K_MWHEELDOWN, 1, 0, NULL, 0 );
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

	Sys_QueEvent(SE_CHAR, (unsigned char)text[0], 0, 0, NULL, 0);
}



/*
===================
GLimp_Init

This is the platform specific OpenGL initialization function.  It
is responsible for loading OpenGL, initializing it,
creating a window of the appropriate size, doing
fullscreen manipulations, etc.  Its overall responsibility is
to make sure that a functional OpenGL subsystem is operating
when it returns to the ref.

If there is any failure, the renderer will revert back to safe
parameters and try again.
===================
*/
bool GLimp_Init( glimpParms_t parms ) {

	// Do we even care?
	// cmdSystem->AddCommand( "testSwapBuffers", GLimp_TestSwapBuffers, CMD_FL_SYSTEM, "Times swapbuffer options" );

	common->Printf( "Initializing OpenGL subsystem with multisamples:%i stereo:%i fullscreen:%i\n", 
		parms.multiSamples, parms.stereo, parms.fullScreen );


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
		//Really bad!
        glfwTerminate();
		common->Error( "Could not start GLFW\n" );
        return false;
    }

	glConfig.isFullscreen = parms.fullScreen;
	glConfig.isStereoPixelFormat = parms.stereo;
	glConfig.nativeScreenWidth = parms.width;
	glConfig.nativeScreenHeight = parms.height;
	glConfig.multisamples = parms.multiSamples;

	glConfig.pixelAspect = 1.0f;	// FIXME: some monitor modes may be distorted
									// should side-by-side stereo modes be consider aspect 0.5?


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

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


	GLenum glewResult = glewInit();
	if( GLEW_OK != glewResult )
	{
		// glewInit failed, something is seriously wrong
		common->Printf( "GLimp_Init() - GLEW could not load OpenGL subsystem: %s", glewGetErrorString( glewResult ) );
	}
	else
	{
		common->Printf( "Using GLEW %s\n", glewGetString( GLEW_VERSION ) );
	}


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
	// Optionally ChangeDisplaySettings to get a different fullscreen resolution.
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwSetWindowMonitor(window, monitor, 0, 0, parms.width, parms.height, parms.displayHz);

	glConfig.isFullscreen = parms.fullScreen;
	glConfig.pixelAspect = 1.0f;	// FIXME: some monitor modes may be distorted

	glConfig.isFullscreen = parms.fullScreen;
	glConfig.nativeScreenWidth = parms.width;
	glConfig.nativeScreenHeight = parms.height;

	return true;
}

/*
===================
GLimp_Shutdown

This routine does all OS specific shutdown procedures for the OpenGL
subsystem.
===================
*/
void GLimp_Shutdown() {
	common->Printf( "Shutting down OpenGL subsystem\n" );

	glfwTerminate();

	// restore gamma
	GLimp_RestoreGamma();
}

/*
=====================
GLimp_SwapBuffers
=====================
*/
void GLimp_SwapBuffers() {
	glfwSwapBuffers(window);
}

/*
===========================================================

SMP acceleration

===========================================================
*/

/*
===================
GLimp_ActivateContext
===================
*/
void GLimp_ActivateContext() {
	glfwMakeContextCurrent(window);
}

/*
===================
GLimp_DeactivateContext
===================
*/
void GLimp_DeactivateContext() {
	glFinish();
}

/*
===================
GLimp_RenderThreadWrapper
===================
*/
static void GLimp_RenderThreadWrapper() {
	win32.glimpRenderThread();
}

/*
=======================
GLimp_SpawnRenderThread

Returns false if the system only has a single processor
=======================
*/
bool GLimp_SpawnRenderThread( void (*function)() ) {
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
void *GLimp_BackEndSleep() {
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
void GLimp_FrontEndSleep() {
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
