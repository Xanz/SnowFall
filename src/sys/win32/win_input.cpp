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

#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "win_local.h"
#include "framework/Session_local.h"

#define DINPUT_BUFFERSIZE 256

#define CHAR_FIRSTREPEAT 200
#define CHAR_REPEAT 100

typedef struct MYDATA
{
	LONG lX;	   // X axis goes here
	LONG lY;	   // Y axis goes here
	LONG lZ;	   // Z axis goes here
	BYTE bButtonA; // One button goes here
	BYTE bButtonB; // Another button goes here
	BYTE bButtonC; // Another button goes here
	BYTE bButtonD; // Another button goes here
} MYDATA;

static DIOBJECTDATAFORMAT rgodf[] = {
	{
		&GUID_XAxis,
		FIELD_OFFSET(MYDATA, lX),
		DIDFT_AXIS | DIDFT_ANYINSTANCE,
		0,
	},
	{
		&GUID_YAxis,
		FIELD_OFFSET(MYDATA, lY),
		DIDFT_AXIS | DIDFT_ANYINSTANCE,
		0,
	},
	{
		&GUID_ZAxis,
		FIELD_OFFSET(MYDATA, lZ),
		0x80000000 | DIDFT_AXIS | DIDFT_ANYINSTANCE,
		0,
	},
	{
		0,
		FIELD_OFFSET(MYDATA, bButtonA),
		DIDFT_BUTTON | DIDFT_ANYINSTANCE,
		0,
	},
	{
		0,
		FIELD_OFFSET(MYDATA, bButtonB),
		DIDFT_BUTTON | DIDFT_ANYINSTANCE,
		0,
	},
	{
		0,
		FIELD_OFFSET(MYDATA, bButtonC),
		0x80000000 | DIDFT_BUTTON | DIDFT_ANYINSTANCE,
		0,
	},
	{
		0,
		FIELD_OFFSET(MYDATA, bButtonD),
		0x80000000 | DIDFT_BUTTON | DIDFT_ANYINSTANCE,
		0,
	},
};

//==========================================================================

static const unsigned char s_scantokey[256] = {
	//  0            1       2          3          4       5            6         7
	//  8            9       A          B          C       D            E         F
	0, 27, '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '-', '=', K_BACKSPACE, 9, // 0
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '[', ']', K_ENTER, K_CTRL, 'a', 's', // 1
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'\'', '`', K_SHIFT, '\\', 'z', 'x', 'c', 'v', // 2
	'b', 'n', 'm', ',', '.', '/', K_SHIFT, K_KP_STAR,
	K_ALT, ' ', K_CAPSLOCK, K_F1, K_F2, K_F3, K_F4, K_F5, // 3
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, K_SCROLL, K_HOME,
	K_UPARROW, K_PGUP, K_KP_MINUS, K_LEFTARROW, K_KP_5, K_RIGHTARROW, K_KP_PLUS, K_END, // 4
	K_DOWNARROW, K_PGDN, K_INS, K_DEL, 0, 0, 0, K_F11,
	K_F12, 0, 0, K_LWIN, K_RWIN, K_MENU, 0, 0, // 5
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, // 6
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, // 7
							// shifted
	0, 27, '!', '@', '#', '$', '%', '^',
	'&', '*', '(', ')', '_', '+', K_BACKSPACE, 9, // 0
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '[', ']', K_ENTER, K_CTRL, 'a', 's', // 1
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'\'', '~', K_SHIFT, '\\', 'z', 'x', 'c', 'v', // 2
	'b', 'n', 'm', ',', '.', '/', K_SHIFT, K_KP_STAR,
	K_ALT, ' ', K_CAPSLOCK, K_F1, K_F2, K_F3, K_F4, K_F5, // 3
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, K_SCROLL, K_HOME,
	K_UPARROW, K_PGUP, K_KP_MINUS, K_LEFTARROW, K_KP_5, K_RIGHTARROW, K_KP_PLUS, K_END, // 4
	K_DOWNARROW, K_PGDN, K_INS, K_DEL, 0, 0, 0, K_F11,
	K_F12, 0, 0, K_LWIN, K_RWIN, K_MENU, 0, 0, // 5
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, // 6
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0 // 7
};

static const unsigned char s_scantokey_german[256] = {
	//  0            1       2          3          4       5            6         7
	//  8            9       A          B          C       D            E         F
	0, 27, '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '?', '\'', K_BACKSPACE, 9, // 0
	'q', 'w', 'e', 'r', 't', 'z', 'u', 'i',
	'o', 'p', '=', '+', K_ENTER, K_CTRL, 'a', 's', // 1
	'd', 'f', 'g', 'h', 'j', 'k', 'l', '[',
	']', '`', K_SHIFT, '#', 'y', 'x', 'c', 'v', // 2
	'b', 'n', 'm', ',', '.', '-', K_SHIFT, K_KP_STAR,
	K_ALT, ' ', K_CAPSLOCK, K_F1, K_F2, K_F3, K_F4, K_F5, // 3
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, K_SCROLL, K_HOME,
	K_UPARROW, K_PGUP, K_KP_MINUS, K_LEFTARROW, K_KP_5, K_RIGHTARROW, K_KP_PLUS, K_END, // 4
	K_DOWNARROW, K_PGDN, K_INS, K_DEL, 0, 0, '<', K_F11,
	K_F12, 0, 0, K_LWIN, K_RWIN, K_MENU, 0, 0, // 5
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, // 6
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, // 7
							// shifted
	0, 27, '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '?', '\'', K_BACKSPACE, 9, // 0
	'q', 'w', 'e', 'r', 't', 'z', 'u', 'i',
	'o', 'p', '=', '+', K_ENTER, K_CTRL, 'a', 's', // 1
	'd', 'f', 'g', 'h', 'j', 'k', 'l', '[',
	']', '`', K_SHIFT, '#', 'y', 'x', 'c', 'v', // 2
	'b', 'n', 'm', ',', '.', '-', K_SHIFT, K_KP_STAR,
	K_ALT, ' ', K_CAPSLOCK, K_F1, K_F2, K_F3, K_F4, K_F5, // 3
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, K_SCROLL, K_HOME,
	K_UPARROW, K_PGUP, K_KP_MINUS, K_LEFTARROW, K_KP_5, K_RIGHTARROW, K_KP_PLUS, K_END, // 4
	K_DOWNARROW, K_PGDN, K_INS, K_DEL, 0, 0, '<', K_F11,
	K_F12, 0, 0, K_LWIN, K_RWIN, K_MENU, 0, 0, // 5
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, // 6
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0 // 7
};

static const unsigned char s_scantokey_french[256] = {
	//  0            1       2          3          4       5            6         7
	//  8            9       A          B          C       D            E         F
	0, 27, '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', ')', '=', K_BACKSPACE, 9, // 0
	'a', 'z', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '^', '$', K_ENTER, K_CTRL, 'q', 's', // 1
	'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm',
	'�', '`', K_SHIFT, '*', 'w', 'x', 'c', 'v', // 2
	'b', 'n', ',', ';', ':', '!', K_SHIFT, K_KP_STAR,
	K_ALT, ' ', K_CAPSLOCK, K_F1, K_F2, K_F3, K_F4, K_F5, // 3
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, K_SCROLL, K_HOME,
	K_UPARROW, K_PGUP, K_KP_MINUS, K_LEFTARROW, K_KP_5, K_RIGHTARROW, K_KP_PLUS, K_END, // 4
	K_DOWNARROW, K_PGDN, K_INS, K_DEL, 0, 0, '<', K_F11,
	K_F12, 0, 0, K_LWIN, K_RWIN, K_MENU, 0, 0, // 5
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, // 6
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, // 7
							// shifted
	0, 27, '&', '�', '\"', '\'', '(', '-',
	'�', '_', '�', '�', '�', '+', K_BACKSPACE, 9, // 0
	'a', 'z', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '^', '$', K_ENTER, K_CTRL, 'q', 's', // 1
	'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm',
	'�', 0, K_SHIFT, '*', 'w', 'x', 'c', 'v', // 2
	'b', 'n', ',', ';', ':', '!', K_SHIFT, K_KP_STAR,
	K_ALT, ' ', K_CAPSLOCK, K_F1, K_F2, K_F3, K_F4, K_F5, // 3
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, K_SCROLL, K_HOME,
	K_UPARROW, K_PGUP, K_KP_MINUS, K_LEFTARROW, K_KP_5, K_RIGHTARROW, K_KP_PLUS, K_END, // 4
	K_DOWNARROW, K_PGDN, K_INS, K_DEL, 0, 0, '<', K_F11,
	K_F12, 0, 0, K_LWIN, K_RWIN, K_MENU, 0, 0, // 5
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, // 6
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0 // 7
};

static const unsigned char s_scantokey_spanish[256] = {
	//  0            1       2          3          4       5            6         7
	//  8            9       A          B          C       D            E         F
	0, 27, '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '\'', '�', K_BACKSPACE, 9, // 0
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '`', '+', K_ENTER, K_CTRL, 'a', 's', // 1
	'd', 'f', 'g', 'h', 'j', 'k', 'l', '�',
	'�', '�', K_SHIFT, '�', 'z', 'x', 'c', 'v', // 2
	'b', 'n', 'm', ',', '.', '-', K_SHIFT, K_KP_STAR,
	K_ALT, ' ', K_CAPSLOCK, K_F1, K_F2, K_F3, K_F4, K_F5, // 3
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, K_SCROLL, K_HOME,
	K_UPARROW, K_PGUP, K_KP_MINUS, K_LEFTARROW, K_KP_5, K_RIGHTARROW, K_KP_PLUS, K_END, // 4
	K_DOWNARROW, K_PGDN, K_INS, K_DEL, 0, 0, '<', K_F11,
	K_F12, 0, 0, K_LWIN, K_RWIN, K_MENU, 0, 0, // 5
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, // 6
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, // 7
							// shifted
	0, 27, '!', '\"', '�', '$', '%', '&',
	'/', '(', ')', '=', '?', '�', K_BACKSPACE, 9, // 0
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '^', '*', K_ENTER, K_CTRL, 'a', 's', // 1
	'd', 'f', 'g', 'h', 'j', 'k', 'l', '�',
	'�', '�', K_SHIFT, '�', 'z', 'x', 'c', 'v', // 2
	'b', 'n', 'm', ',', '.', '-', K_SHIFT, K_KP_STAR,
	K_ALT, ' ', K_CAPSLOCK, K_F1, K_F2, K_F3, K_F4, K_F5, // 3
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, K_SCROLL, K_HOME,
	K_UPARROW, K_PGUP, K_KP_MINUS, K_LEFTARROW, K_KP_5, K_RIGHTARROW, K_KP_PLUS, K_END, // 4
	K_DOWNARROW, K_PGDN, K_INS, K_DEL, 0, 0, '<', K_F11,
	K_F12, 0, 0, K_LWIN, K_RWIN, K_MENU, 0, 0, // 5
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, // 6
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0 // 7
};

static const unsigned char s_scantokey_italian[256] = {
	//  0            1       2          3          4       5            6         7
	//  8            9       A          B          C       D            E         F
	0, 27, '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '\'', '�', K_BACKSPACE, 9, // 0
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '�', '+', K_ENTER, K_CTRL, 'a', 's', // 1
	'd', 'f', 'g', 'h', 'j', 'k', 'l', '�',
	'�', '\\', K_SHIFT, '�', 'z', 'x', 'c', 'v', // 2
	'b', 'n', 'm', ',', '.', '-', K_SHIFT, K_KP_STAR,
	K_ALT, ' ', K_CAPSLOCK, K_F1, K_F2, K_F3, K_F4, K_F5, // 3
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, K_SCROLL, K_HOME,
	K_UPARROW, K_PGUP, K_KP_MINUS, K_LEFTARROW, K_KP_5, K_RIGHTARROW, K_KP_PLUS, K_END, // 4
	K_DOWNARROW, K_PGDN, K_INS, K_DEL, 0, 0, '<', K_F11,
	K_F12, 0, 0, K_LWIN, K_RWIN, K_MENU, 0, 0, // 5
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, // 6
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, // 7
							// shifted
	0, 27, '!', '\"', '�', '$', '%', '&',
	'/', '(', ')', '=', '?', '^', K_BACKSPACE, 9, // 0
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '�', '*', K_ENTER, K_CTRL, 'a', 's', // 1
	'd', 'f', 'g', 'h', 'j', 'k', 'l', '�',
	'�', '|', K_SHIFT, '�', 'z', 'x', 'c', 'v', // 2
	'b', 'n', 'm', ',', '.', '-', K_SHIFT, K_KP_STAR,
	K_ALT, ' ', K_CAPSLOCK, K_F1, K_F2, K_F3, K_F4, K_F5, // 3
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, K_SCROLL, K_HOME,
	K_UPARROW, K_PGUP, K_KP_MINUS, K_LEFTARROW, K_KP_5, K_RIGHTARROW, K_KP_PLUS, K_END, // 4
	K_DOWNARROW, K_PGDN, K_INS, K_DEL, 0, 0, '<', K_F11,
	K_F12, 0, 0, K_LWIN, K_RWIN, K_MENU, 0, 0, // 5
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, // 6
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0 // 7

};

static const unsigned char *keyScanTable = s_scantokey;

// this should be part of the scantables and the scan tables should be 512 bytes
// (256 scan codes, shifted and unshifted).  Changing everything to use 512 byte
// scan tables now might introduce bugs in tested code.  Since we only need to fix
// the right-alt case for non-US keyboards, we're just using a special-case table
// for it.  Eventually, the tables above should be fixed to handle all possible
// scan codes instead of just the first 128.
static unsigned char rightAltKey = K_ALT;

#define NUM_OBJECTS (sizeof(rgodf) / sizeof(rgodf[0]))

static DIDATAFORMAT df = {
	sizeof(DIDATAFORMAT),		// this structure
	sizeof(DIOBJECTDATAFORMAT), // size of object data format
	DIDF_RELAXIS,				// absolute axis coordinates
	sizeof(MYDATA),				// device data size
	NUM_OBJECTS,				// number of objects
	rgodf,						// and here they are
};

/*
============================================================

DIRECT INPUT KEYBOARD CONTROL

============================================================
*/

bool IN_StartupKeyboard(void)
{

	common->Printf("keyboard: DirectInput initialized.\n");
	return true;
}

// This function converts a GLFW scan code to its ASCII representation if possible
int scanCodeToAscii(int scanCode)
{
	const char *keyName = glfwGetKeyName(GLFW_KEY_UNKNOWN, scanCode);
	if (keyName && keyName[0] != '\0' && keyName[1] == '\0')
	{
		return static_cast<int>(keyName[0]);
	}
	return -1; // Return -1 if the scan code does not correspond to a printable ASCII character
}

/*
=======
MapKey

Map from GLFW to Doom keynums
=======
*/
int GLFWDoom_MapKey(int key)
{

	int scanCode = glfwGetKeyScancode(key);

	int convertedKey = scanCodeToAscii(scanCode);

	if (convertedKey != -1)
	{
		return convertedKey;
	}

	// Special keys need to be converted here.
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		return K_ESCAPE;
	case GLFW_KEY_LEFT_SHIFT:
		return K_SHIFT;
	case GLFW_KEY_LEFT_ALT:
		return K_ALT;
	case GLFW_KEY_RIGHT_ALT:
		return K_RIGHT_ALT;
	case GLFW_KEY_LEFT_CONTROL:
		return K_CTRL;
	case GLFW_KEY_LEFT:
		return K_LEFTARROW;
	case GLFW_KEY_RIGHT:
		return K_RIGHTARROW;
	case GLFW_KEY_UP:
		return K_UPARROW;
	case GLFW_KEY_DOWN:
		return K_DOWNARROW;
	}

	return 0;
}

/*
==========================
IN_DeactivateKeyboard
==========================
*/
void IN_DeactivateKeyboard(void)
{
	// if (!win32.g_pKeyboard) {
	// 	return;
	// }
	// win32.g_pKeyboard->Unacquire( );
}

/*
============================================================

DIRECT INPUT MOUSE CONTROL

============================================================
*/

/*
========================
IN_InitDirectInput
========================
*/

void IN_InitDirectInput(void)
{
}

/*
========================
IN_InitDIMouse
========================
*/
bool IN_InitDIMouse(void)
{

	return true;
}

/*
==========================
IN_ActivateMouse
==========================
*/
void IN_ActivateMouse(void)
{

	// if ( !win32.in_mouse.GetBool() || win32.mouseGrabbed || !win32.g_pMouse ) {
	// 	return;
	// }

	// win32.mouseGrabbed = true;

	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// for ( i = 0; i < 10; i++ ) {
	// if ( ::ShowCursor( false ) < 0 ) {
	// 		break;
	// 	}
	// }

	// we may fail to reacquire if the window has been recreated
	// hr = win32.g_pMouse->Acquire();
	// if (FAILED(hr)) {
	// 	return;
	// }

	// set the cooperativity level.
	// hr = win32.g_pMouse->SetCooperativeLevel( win32.hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
}

/*
==========================
IN_DeactivateMouse
==========================
*/
void IN_DeactivateMouse(void)
{
}

/*
==========================
IN_DeactivateMouseIfWindowed
==========================
*/
void IN_DeactivateMouseIfWindowed(void)
{
}

/*
============================================================

  MOUSE CONTROL

============================================================
*/

/*
===========
Sys_ShutdownInput
===========
*/
void Sys_ShutdownInput(void)
{
	mouse_polls.Clear();
	keyboard_polls.Clear();
	IN_DeactivateMouse();
	IN_DeactivateKeyboard();
}

/*
===========
Sys_InitInput
===========
*/
void Sys_InitInput(void)
{
	keyboard_polls.SetGranularity(64);
	mouse_polls.SetGranularity(64);
	common->Printf("\n------- Input Initialization -------\n");
	common->Printf("------------------------------------\n");
}

/*
===========
Sys_InitScanTable
===========
*/
void Sys_InitScanTable(void)
{
	idStr lang = cvarSystem->GetCVarString("sys_lang");
	if (lang.Length() == 0)
	{
		lang = "english";
	}
	if (lang.Icmp("english") == 0)
	{
		keyScanTable = s_scantokey;
		// the only reason that english right alt binds as K_ALT is so that
		// users who were using right-alt before the patch don't suddenly find
		// that only left-alt is working.
		rightAltKey = K_ALT;
	}
	else if (lang.Icmp("spanish") == 0)
	{
		keyScanTable = s_scantokey_spanish;
		rightAltKey = K_RIGHT_ALT;
	}
	else if (lang.Icmp("french") == 0)
	{
		keyScanTable = s_scantokey_french;
		rightAltKey = K_RIGHT_ALT;
	}
	else if (lang.Icmp("german") == 0)
	{
		keyScanTable = s_scantokey_german;
		rightAltKey = K_RIGHT_ALT;
	}
	else if (lang.Icmp("italian") == 0)
	{
		keyScanTable = s_scantokey_italian;
		rightAltKey = K_RIGHT_ALT;
	}
}

/*
==================
Sys_GetScanTable
==================
*/
const unsigned char *Sys_GetScanTable(void)
{
	return keyScanTable;
}

/*
===============
Sys_GetConsoleKey
===============
*/
unsigned char Sys_GetConsoleKey(bool shifted)
{
	return keyScanTable[41 + (shifted ? 128 : 0)];
}

/*
==================
IN_Frame

Called every frame, even if not generating commands
==================
*/
void IN_Frame(void)
{

	bool shouldGrab = true;
	// Needed to be set up here.
	bool menuActive = (sessLocal.GetActiveMenu() != NULL);

	UIActive = menuActive;
}

void Sys_GrabMouseCursor(bool grabIt)
{
#ifndef ID_DEDICATED
	// win32.mouseReleased = !grabIt;
	if (!grabIt)
	{
		// release it right now
		IN_Frame();
	}
#endif
}

//=====================================================================================

static int diFetch;
static byte toggleFetch[2][256];

/*
====================
CheckKeyboardEvent

Checks if the GLFW key is active and returns the subsequent event.

Not used currently but was useful for testing.
====================
*/
void CheckKeyboardEvent(int key)
{
	if (glfwGetKey(window, key) == GLFW_PRESS)
	{
		keyboard_polls.Append(keyboard_poll_t(key, true));
		Sys_QueEvent(GetTickCount(), SE_KEY, GLFWDoom_MapKey(key), true, 0, NULL);
	}
	else
	{
		keyboard_polls.Append(keyboard_poll_t(key, false));
		Sys_QueEvent(GetTickCount(), SE_KEY, GLFWDoom_MapKey(key), false, 0, NULL);
	}
}

/*
====================
Sys_PollKeyboardInputEvents
====================
*/
int Sys_PollKeyboardInputEvents(void)
{
	// return the keyboard events.
	return keyboard_polls.Num();
}

/*
====================
Sys_PollKeyboardInputEvents
====================
*/
int Sys_ReturnKeyboardInputEvent(const int n, int &ch, bool &state)
{
	ch = GLFWDoom_MapKey(keyboard_polls[n].key);
	state = keyboard_polls[n].state;
	return ch;
}

void Sys_EndKeyboardInputEvents(void)
{
	keyboard_polls.SetNum(0, false);
}

//=====================================================================================

int Sys_PollMouseInputEvents(void)
{
	return mouse_polls.Num();
}

int Sys_ReturnMouseInputEvent(const int n, int &action, int &value)
{
	if (n >= mouse_polls.Num())
	{
		return 0;
	}

	action = mouse_polls[n].action;
	value = mouse_polls[n].value;
	return 1;
}

void Sys_EndMouseInputEvents(void)
{
	mouse_polls.SetNum(0, false);
}

unsigned char Sys_MapCharForKey(int key)
{
	return (unsigned char)key;
}
