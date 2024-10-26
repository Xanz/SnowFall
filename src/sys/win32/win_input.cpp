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

#pragma hdrstop
#include <vector>

#include "../../idlib/precompiled.h"
#include "../sys_session_local.h"
#include "GLFW/glfw3.h"

#include "win_local.h"

std::vector<mousePoll_t> s_MousePolls;
std::vector<keyboardPoll_t> s_KeyboardPolls;


int GLFWKeyConvert(int glfwKey)
{
	switch (glfwKey)
	{
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
	case GLFW_KEY_DELETE: return K_DEL;
	case GLFW_KEY_HOME: return K_HOME;
	case GLFW_KEY_END: return K_END;
	case GLFW_KEY_PAGE_UP: return K_PGUP;
	case GLFW_KEY_PAGE_DOWN: return K_PGDN;
	case GLFW_KEY_RIGHT: return K_RIGHTARROW;
	case GLFW_KEY_LEFT: return K_LEFTARROW;
	case GLFW_KEY_DOWN: return K_DOWNARROW;
	case GLFW_KEY_UP: return K_UPARROW;
	case GLFW_KEY_LEFT_SUPER: return K_LWIN;
	case GLFW_KEY_RIGHT_SUPER: return K_RWIN;
	default: return K_NONE;
	}
}

/*
===========
Sys_ShutdownInput
===========
*/
void Sys_ShutdownInput()
{
}

/*
===========
Sys_InitInput
===========
*/
void Sys_InitInput()
{
}


void Sys_GrabMouseCursor(bool grabIt)
{
}

/*
====================
Sys_PollKeyboardInputEvents
====================
*/
int Sys_PollKeyboardInputEvents()
{
	return s_KeyboardPolls.size();
}

/*
====================
Sys_PollKeyboardInputEvents
====================
*/
int Sys_ReturnKeyboardInputEvent(const int n, int& key, bool& state)
{
	if (n >= s_KeyboardPolls.size())
	{
		return 0;
	}

	key = s_KeyboardPolls[n].key;
	state = s_KeyboardPolls[n].state;
	return 1;
}


void Sys_EndKeyboardInputEvents()
{
	s_KeyboardPolls.clear();
}

//=====================================================================================


int Sys_PollMouseInputEvents(int mouseEvents[MAX_MOUSE_EVENTS][2])
{
	int numEvents = s_MousePolls.size();
	if (numEvents > MAX_MOUSE_EVENTS)
	{
		numEvents = MAX_MOUSE_EVENTS;
	}

	for (int i = 0; i < numEvents; i++)
	{
		const mousePoll_t& mp = s_MousePolls[i];

		mouseEvents[i][0] = mp.action;
		mouseEvents[i][1] = mp.value;
	}

	s_MousePolls.clear();
	return numEvents;
}

//=====================================================================================
//	Joystick Input Handling
//=====================================================================================

void Sys_SetRumble(int device, int low, int hi)
{
	return win32.g_Joystick.SetRumble(device, low, hi);
}

int Sys_PollJoystickInputEvents(int deviceNum)
{
	return win32.g_Joystick.PollInputEvents(deviceNum);
}


int Sys_ReturnJoystickInputEvent(const int n, int& action, int& value)
{
	return win32.g_Joystick.ReturnInputEvent(n, action, value);
}


void Sys_EndJoystickInputEvents()
{
}


/*
========================
JoystickSamplingThread
========================
*/
static int threadTimeDeltas[256];
static int threadPacket[256];
static int threadCount;

void JoystickSamplingThread(void* data)
{
	static int prevTime = 0;
	static uint64 nextCheck[MAX_JOYSTICKS] = {0};
	const uint64 waitTime = 5000000; // poll every 5 seconds to see if a controller was connected
	while (1)
	{
		// hopefully we see close to 4000 usec each loop
		int now = Sys_Microseconds();
		int delta;
		if (prevTime == 0)
		{
			delta = 4000;
		}
		else
		{
			delta = now - prevTime;
		}
		prevTime = now;
		threadTimeDeltas[threadCount & 255] = delta;
		threadCount++;

		{
			XINPUT_STATE joyData[MAX_JOYSTICKS];
			bool validData[MAX_JOYSTICKS];
			for (int i = 0; i < MAX_JOYSTICKS; i++)
			{
				if (now >= nextCheck[i])
				{
					// XInputGetState might block... for a _really_ long time..
					validData[i] = XInputGetState(i, &joyData[i]) == ERROR_SUCCESS;

					// allow an immediate data poll if the input device is connected else
					// wait for some time to see if another device was reconnected.
					// Checking input state infrequently for newly connected devices prevents
					// severe slowdowns on PC, especially on WinXP64.
					if (validData[i])
					{
						nextCheck[i] = 0;
					}
					else
					{
						nextCheck[i] = now + waitTime;
					}
				}
			}

			// do this short amount of processing inside a critical section
			idScopedCriticalSection cs(win32.g_Joystick.mutexXis);

			for (int i = 0; i < MAX_JOYSTICKS; i++)
			{
				controllerState_t* cs = &win32.g_Joystick.controllers[i];

				if (!validData[i])
				{
					cs->valid = false;
					continue;
				}
				cs->valid = true;

				XINPUT_STATE& current = joyData[i];

				cs->current = current;

				// Switch from using cs->current to current to reduce chance of Load-Hit-Store on consoles

				threadPacket[threadCount & 255] = current.dwPacketNumber;
#if 0
				if ( xis.dwPacketNumber == oldXis[ inputDeviceNum ].dwPacketNumber ) {
					return numEvents;
				}
#endif
				cs->buttonBits |= current.Gamepad.wButtons;
			}
		}

		// we want this to be processed at least 250 times a second
		WaitForSingleObject(win32.g_Joystick.timer, INFINITE);
	}
}


/*
========================
idJoystickWin32::idJoystickWin32
========================
*/
idJoystickWin32::idJoystickWin32()
{
	numEvents = 0;
	memset(&events, 0, sizeof(events));
	memset(&controllers, 0, sizeof(controllers));
	memset(buttonStates, 0, sizeof(buttonStates));
	memset(joyAxis, 0, sizeof(joyAxis));
}

/*
========================
idJoystickWin32::Init
========================
*/
bool idJoystickWin32::Init()
{
	idJoystick::Init();

	// setup the timer that the high frequency thread will wait on
	// to fire every 4 msec
	timer = CreateWaitableTimer(NULL, FALSE, "JoypadTimer");
	LARGE_INTEGER dueTime;
	dueTime.QuadPart = -1;
	if (!SetWaitableTimer(timer, &dueTime, 4, NULL, NULL, FALSE))
	{
		idLib::FatalError("SetWaitableTimer for joystick failed");
	}

	// spawn the high frequency joystick reading thread
	Sys_CreateThread((xthread_t)JoystickSamplingThread, NULL, THREAD_HIGHEST, "Joystick", CORE_1A);

	return false;
}

/*
========================
idJoystickWin32::SetRumble
========================
*/
void idJoystickWin32::SetRumble(int inputDeviceNum, int rumbleLow, int rumbleHigh)
{
	if (inputDeviceNum < 0 || inputDeviceNum >= MAX_JOYSTICKS)
	{
		return;
	}
	if (!controllers[inputDeviceNum].valid)
	{
		return;
	}
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = idMath::ClampInt(0, 65535, rumbleLow);
	vibration.wRightMotorSpeed = idMath::ClampInt(0, 65535, rumbleHigh);
	DWORD err = XInputSetState(inputDeviceNum, &vibration);
	if (err != ERROR_SUCCESS)
	{
		idLib::Warning("XInputSetState error: 0x%x", err);
	}
}

/*
========================
idJoystickWin32::PostInputEvent
========================
*/
void idJoystickWin32::PostInputEvent(int inputDeviceNum, int event, int value, int range)
{
	// These events are used for GUI button presses
	if ((event >= J_ACTION1) && (event <= J_ACTION_MAX))
	{
		PushButton(inputDeviceNum, K_JOY1 + (event - J_ACTION1), value != 0);
	}
	else if (event == J_AXIS_LEFT_X)
	{
		PushButton(inputDeviceNum, K_JOY_STICK1_LEFT, (value < -range));
		PushButton(inputDeviceNum, K_JOY_STICK1_RIGHT, (value > range));
	}
	else if (event == J_AXIS_LEFT_Y)
	{
		PushButton(inputDeviceNum, K_JOY_STICK1_UP, (value < -range));
		PushButton(inputDeviceNum, K_JOY_STICK1_DOWN, (value > range));
	}
	else if (event == J_AXIS_RIGHT_X)
	{
		PushButton(inputDeviceNum, K_JOY_STICK2_LEFT, (value < -range));
		PushButton(inputDeviceNum, K_JOY_STICK2_RIGHT, (value > range));
	}
	else if (event == J_AXIS_RIGHT_Y)
	{
		PushButton(inputDeviceNum, K_JOY_STICK2_UP, (value < -range));
		PushButton(inputDeviceNum, K_JOY_STICK2_DOWN, (value > range));
	}
	else if ((event >= J_DPAD_UP) && (event <= J_DPAD_RIGHT))
	{
		PushButton(inputDeviceNum, K_JOY_DPAD_UP + (event - J_DPAD_UP), value != 0);
	}
	else if (event == J_AXIS_LEFT_TRIG)
	{
		PushButton(inputDeviceNum, K_JOY_TRIGGER1, (value > range));
	}
	else if (event == J_AXIS_RIGHT_TRIG)
	{
		PushButton(inputDeviceNum, K_JOY_TRIGGER2, (value > range));
	}
	if (event >= J_AXIS_MIN && event <= J_AXIS_MAX)
	{
		int axis = event - J_AXIS_MIN;
		int percent = (value * 16) / range;
		if (joyAxis[inputDeviceNum][axis] != percent)
		{
			joyAxis[inputDeviceNum][axis] = percent;
			Sys_QueEvent(SE_JOYSTICK, axis, percent, 0, NULL, inputDeviceNum);
		}
	}

	// These events are used for actual game input
	events[numEvents].event = event;
	events[numEvents].value = value;
	numEvents++;
}

/*
========================
idJoystickWin32::PollInputEvents
========================
*/
int idJoystickWin32::PollInputEvents(int inputDeviceNum)
{
	numEvents = 0;

	// if (!win32.activeApp)
	// {
	// 	return numEvents;
	// }

	assert(inputDeviceNum < 4);

	//	if ( inputDeviceNum > in_joystick.GetInteger() ) {
	//		return numEvents;
	//	}

	controllerState_t* cs = &controllers[inputDeviceNum];

	// grab the current packet under a critical section
	XINPUT_STATE xis;
	XINPUT_STATE old;
	int orBits;
	{
		idScopedCriticalSection crit(mutexXis);
		xis = cs->current;
		old = cs->previous;
		cs->previous = xis;
		// fetch or'd button bits
		orBits = cs->buttonBits;
		cs->buttonBits = 0;
	}
#if 0
	if ( XInputGetState( inputDeviceNum, &xis ) != ERROR_SUCCESS ) {
		return numEvents;
	}
#endif
	for (int i = 0; i < 32; i++)
	{
		int bit = 1 << i;

		if (((xis.Gamepad.wButtons | old.Gamepad.wButtons) & bit) == 0
			&& (orBits & bit))
		{
			idLib::Printf("Dropped button press on bit %i\n", i);
		}
	}

	if (session->IsSystemUIShowing())
	{
		// memset xis so the current input does not get latched if the UI is showing
		memset(&xis, 0, sizeof(XINPUT_STATE));
	}

	int joyRemap[16] = {
		J_DPAD_UP, J_DPAD_DOWN, // Up, Down
		J_DPAD_LEFT, J_DPAD_RIGHT, // Left, Right
		J_ACTION9, J_ACTION10, // Start, Back
		J_ACTION7, J_ACTION8, // Left Stick Down, Right Stick Down
		J_ACTION5, J_ACTION6, // Black, White (Left Shoulder, Right Shoulder)
		0, 0, // Unused
		J_ACTION1, J_ACTION2, // A, B
		J_ACTION3, J_ACTION4, // X, Y
	};

	// Check the digital buttons
	for (int i = 0; i < 16; i++)
	{
		int mask = (1 << i);
		if ((xis.Gamepad.wButtons & mask) != (old.Gamepad.wButtons & mask))
		{
			PostInputEvent(inputDeviceNum, joyRemap[i], (xis.Gamepad.wButtons & mask) > 0);
		}
	}

	// Check the triggers
	if (xis.Gamepad.bLeftTrigger != old.Gamepad.bLeftTrigger)
	{
		PostInputEvent(inputDeviceNum, J_AXIS_LEFT_TRIG, xis.Gamepad.bLeftTrigger * 128);
	}
	if (xis.Gamepad.bRightTrigger != old.Gamepad.bRightTrigger)
	{
		PostInputEvent(inputDeviceNum, J_AXIS_RIGHT_TRIG, xis.Gamepad.bRightTrigger * 128);
	}

	if (xis.Gamepad.sThumbLX != old.Gamepad.sThumbLX)
	{
		PostInputEvent(inputDeviceNum, J_AXIS_LEFT_X, xis.Gamepad.sThumbLX);
	}
	if (xis.Gamepad.sThumbLY != old.Gamepad.sThumbLY)
	{
		PostInputEvent(inputDeviceNum, J_AXIS_LEFT_Y, -xis.Gamepad.sThumbLY);
	}
	if (xis.Gamepad.sThumbRX != old.Gamepad.sThumbRX)
	{
		PostInputEvent(inputDeviceNum, J_AXIS_RIGHT_X, xis.Gamepad.sThumbRX);
	}
	if (xis.Gamepad.sThumbRY != old.Gamepad.sThumbRY)
	{
		PostInputEvent(inputDeviceNum, J_AXIS_RIGHT_Y, -xis.Gamepad.sThumbRY);
	}

	return numEvents;
}


/*
========================
idJoystickWin32::ReturnInputEvent
========================
*/
int idJoystickWin32::ReturnInputEvent(const int n, int& action, int& value)
{
	if ((n < 0) || (n >= MAX_JOY_EVENT))
	{
		return 0;
	}

	action = events[n].event;
	value = events[n].value;

	return 1;
}

/*
========================
idJoystickWin32::PushButton
========================
*/
void idJoystickWin32::PushButton(int inputDeviceNum, int key, bool value)
{
	// So we don't keep sending the same SE_KEY message over and over again
	if (buttonStates[inputDeviceNum][key] != value)
	{
		buttonStates[inputDeviceNum][key] = value;
		Sys_QueEvent(SE_KEY, key, value, 0, NULL, inputDeviceNum);
	}
}
