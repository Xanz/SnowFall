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
	m_MousePolls.clear();
	m_KeyboardPolls.clear();
}

/*
===========
Sys_InitInput
===========
*/
void Sys_InitInput(void)
{
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

/*
====================
CheckKeyboardEvent

Checks if the GLFW key is active and returns the subsequent event.

Not used currently but was useful for testing.
====================
*/
void CheckKeyboardEvent(int key)
{
	if (glfwGetKey(m_Window, key) == GLFW_PRESS)
	{
		m_KeyboardPolls.push_back(keyboard_poll_t(key, true));
		Sys_QueEvent(GetTickCount(), SE_KEY, GLFWDoom_MapKey(key), true, 0, NULL);
	}
	else
	{
		m_KeyboardPolls.push_back(keyboard_poll_t(key, false));
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
	return m_KeyboardPolls.size();
}

/*
====================
Sys_PollKeyboardInputEvents
====================
*/
int Sys_ReturnKeyboardInputEvent(const int n, int &ch, bool &state)
{
	ch = GLFWDoom_MapKey(m_KeyboardPolls[n].key);
	state = m_KeyboardPolls[n].state;
	return ch;
}

void Sys_EndKeyboardInputEvents(void)
{
	m_KeyboardPolls.clear();
}

//=====================================================================================

int Sys_PollMouseInputEvents(void)
{
	return m_MousePolls.size();
}

int Sys_ReturnMouseInputEvent(const int n, int &action, int &value)
{
	if (n >= m_MousePolls.size())
	{
		return 0;
	}

	action = m_MousePolls[n].action;
	value = m_MousePolls[n].value;
	return 1;
}

void Sys_EndMouseInputEvents(void)
{
	m_MousePolls.clear();
}

unsigned char Sys_MapCharForKey(int key)
{
	return (unsigned char)key;
}
