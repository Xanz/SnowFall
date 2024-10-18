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


idCVar r_useOpenGL32("r_useOpenGL32", "1", CVAR_INTEGER,
                     "0 = OpenGL 2.0, 1 = OpenGL 3.2 compatibility profile, 2 = OpenGL 3.2 core profile", 0, 2);

//
// function declaration
//
bool QGL_Init(const char* dllname);
void QGL_Shutdown();


/*
========================
GLimp_TestSwapBuffers
========================
*/
void GLimp_TestSwapBuffers(const idCmdArgs& args)
{
	idLib::Printf("GLimp_TimeSwapBuffers\n");
	static const int MAX_FRAMES = 5;
	uint64 timestamps[MAX_FRAMES];
	glDisable(GL_SCISSOR_TEST);

	int frameMilliseconds = 16;
	for (int swapInterval = 2; swapInterval >= -1; swapInterval--)
	{
		for (int i = 0; i < MAX_FRAMES; i++)
		{
			if (swapInterval == -1)
			{
				Sys_Sleep(frameMilliseconds);
			}
			if (i & 1)
			{
				glClearColor(0, 1, 0, 1);
			}
			else
			{
				glClearColor(1, 0, 0, 1);
			}
			glClear(GL_COLOR_BUFFER_BIT);
			glFinish();
			timestamps[i] = Sys_Microseconds();
		}

		idLib::Printf("\nswapinterval %i\n", swapInterval);
		for (int i = 1; i < MAX_FRAMES; i++)
		{
			idLib::Printf("%i microseconds\n", (int)(timestamps[i] - timestamps[i - 1]));
		}
	}
}

/*
========================
GLimp_GetOldGammaRamp
========================
*/
static void GLimp_SaveGamma()
{
	HDC hDC;
	BOOL success;

	hDC = GetDC(GetDesktopWindow());
	success = GetDeviceGammaRamp(hDC, win32.oldHardwareGamma);
	common->DPrintf("...getting default gamma ramp: %s\n", success ? "success" : "failed");
	ReleaseDC(GetDesktopWindow(), hDC);
}

/*
========================
GLimp_RestoreGamma
========================
*/
static void GLimp_RestoreGamma()
{
	HDC hDC;
	BOOL success;

	// if we never read in a reasonable looking
	// table, don't write it out
	if (win32.oldHardwareGamma[0][255] == 0)
	{
		return;
	}

	hDC = GetDC(GetDesktopWindow());
	success = SetDeviceGammaRamp(hDC, win32.oldHardwareGamma);
	common->DPrintf("...restoring hardware gamma: %s\n", success ? "success" : "failed");
	ReleaseDC(GetDesktopWindow(), hDC);
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
bool R_CheckWinExtension(const char* name)
{
	if (!strstr(glConfig.wgl_extensions_string, name))
	{
		idLib::Printf("X..%s not found\n", name);
		return false;
	}

	idLib::Printf("...using %s\n", name);
	return true;
}


/*
==================
GLW_GetWGLExtensionsWithFakeWindow
==================
*/
void GLW_CheckWGLExtensions(HDC hDC)
{
}


//=============================================================================

/*
====================
GLW_WM_CREATE
====================
*/
void GLW_WM_CREATE(HWND hWnd)
{
}

/*
========================
CreateOpenGLContextOnDC
========================
*/
static HGLRC CreateOpenGLContextOnDC(const HDC hdc, const bool debugContext)
{
	int useOpenGL32 = r_useOpenGL32.GetInteger();
	HGLRC m_hrc = NULL;

	if (m_hrc == NULL)
	{
		int err = GetLastError();
		switch (err)
		{
		default: idLib::Printf("unknown error: 0x%x\n", err);
			break;
		}
	}

	return m_hrc;
}

/*
====================
GLW_ChoosePixelFormat

Returns -1 on failure, or a pixel format
====================
*/
static int GLW_ChoosePixelFormat(const HDC hdc, const int multisamples, const bool stereo3D)
{
	return 0;
}


/*
====================
GLW_InitDriver

Set the pixelformat for the window before it is
shown, and create the rendering context
====================
*/
static bool GLW_InitDriver(windowParms_t parms)
{
	return true;
}

/*
====================
GLW_CreateWindowClasses
====================
*/
static void GLW_CreateWindowClasses()
{
}

/*
========================
GetDisplayName
========================
*/
static const char* GetDisplayName(const int deviceNum)
{
	static DISPLAY_DEVICE device;
	device.cb = sizeof(device);
	if (!EnumDisplayDevices(
		0, // lpDevice
		deviceNum,
		&device,
		0 /* dwFlags */))
	{
		return NULL;
	}
	return device.DeviceName;
}

/*
========================
GetDeviceName
========================
*/
static idStr GetDeviceName(const int deviceNum)
{
	DISPLAY_DEVICE device = {};
	device.cb = sizeof(device);
	if (!EnumDisplayDevices(
		0, // lpDevice
		deviceNum,
		&device,
		0 /* dwFlags */))
	{
		return "";
	}

	// get the monitor for this display
	if (!(device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
	{
		return "";
	}

	return idStr(device.DeviceName);
}

/*
========================
GetDisplayCoordinates
========================
*/
static bool GetDisplayCoordinates(const int deviceNum, int& x, int& y, int& width, int& height, int& displayHz)
{
	idStr deviceName = GetDeviceName(deviceNum);
	if (deviceName.Length() == 0)
	{
		return false;
	}

	DISPLAY_DEVICE device = {};
	device.cb = sizeof(device);
	if (!EnumDisplayDevices(
		0, // lpDevice
		deviceNum,
		&device,
		0 /* dwFlags */))
	{
		return false;
	}

	DISPLAY_DEVICE monitor;
	monitor.cb = sizeof(monitor);
	if (!EnumDisplayDevices(
		deviceName.c_str(),
		0,
		&monitor,
		0 /* dwFlags */))
	{
		return false;
	}

	DEVMODE devmode;
	devmode.dmSize = sizeof(devmode);
	if (!EnumDisplaySettings(deviceName.c_str(),ENUM_CURRENT_SETTINGS, &devmode))
	{
		return false;
	}

	common->Printf("display device: %i\n", deviceNum);
	common->Printf("  DeviceName  : %s\n", device.DeviceName);
	common->Printf("  DeviceString: %s\n", device.DeviceString);
	common->Printf("  StateFlags  : 0x%x\n", device.StateFlags);
	common->Printf("  DeviceID    : %s\n", device.DeviceID);
	common->Printf("  DeviceKey   : %s\n", device.DeviceKey);
	common->Printf("      DeviceName  : %s\n", monitor.DeviceName);
	common->Printf("      DeviceString: %s\n", monitor.DeviceString);
	common->Printf("      StateFlags  : 0x%x\n", monitor.StateFlags);
	common->Printf("      DeviceID    : %s\n", monitor.DeviceID);
	common->Printf("      DeviceKey   : %s\n", monitor.DeviceKey);
	common->Printf("          dmPosition.x      : %i\n", devmode.dmPosition.x);
	common->Printf("          dmPosition.y      : %i\n", devmode.dmPosition.y);
	common->Printf("          dmBitsPerPel      : %i\n", devmode.dmBitsPerPel);
	common->Printf("          dmPelsWidth       : %i\n", devmode.dmPelsWidth);
	common->Printf("          dmPelsHeight      : %i\n", devmode.dmPelsHeight);
	common->Printf("          dmDisplayFlags    : 0x%x\n", devmode.dmDisplayFlags);
	common->Printf("          dmDisplayFrequency: %i\n", devmode.dmDisplayFrequency);

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
static const char* DMDFO(int dmDisplayFixedOutput)
{
	switch (dmDisplayFixedOutput)
	{
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
static void PrintDevMode(DEVMODE& devmode)
{
	common->Printf("          dmPosition.x        : %i\n", devmode.dmPosition.x);
	common->Printf("          dmPosition.y        : %i\n", devmode.dmPosition.y);
	common->Printf("          dmBitsPerPel        : %i\n", devmode.dmBitsPerPel);
	common->Printf("          dmPelsWidth         : %i\n", devmode.dmPelsWidth);
	common->Printf("          dmPelsHeight        : %i\n", devmode.dmPelsHeight);
	common->Printf("          dmDisplayFixedOutput: %s\n", DMDFO(devmode.dmDisplayFixedOutput));
	common->Printf("          dmDisplayFlags      : 0x%x\n", devmode.dmDisplayFlags);
	common->Printf("          dmDisplayFrequency  : %i\n", devmode.dmDisplayFrequency);
}

/*
====================
DumpAllDisplayDevices
====================
*/
void DumpAllDisplayDevices()
{
	common->Printf("\n");
	for (int deviceNum = 0; ; deviceNum++)
	{
		DISPLAY_DEVICE device = {};
		device.cb = sizeof(device);
		if (!EnumDisplayDevices(
			0, // lpDevice
			deviceNum,
			&device,
			0 /* dwFlags */))
		{
			break;
		}

		common->Printf("display device: %i\n", deviceNum);
		common->Printf("  DeviceName  : %s\n", device.DeviceName);
		common->Printf("  DeviceString: %s\n", device.DeviceString);
		common->Printf("  StateFlags  : 0x%x\n", device.StateFlags);
		common->Printf("  DeviceID    : %s\n", device.DeviceID);
		common->Printf("  DeviceKey   : %s\n", device.DeviceKey);

		for (int monitorNum = 0; ; monitorNum++)
		{
			DISPLAY_DEVICE monitor = {};
			monitor.cb = sizeof(monitor);
			if (!EnumDisplayDevices(
				device.DeviceName,
				monitorNum,
				&monitor,
				0 /* dwFlags */))
			{
				break;
			}

			common->Printf("      DeviceName  : %s\n", monitor.DeviceName);
			common->Printf("      DeviceString: %s\n", monitor.DeviceString);
			common->Printf("      StateFlags  : 0x%x\n", monitor.StateFlags);
			common->Printf("      DeviceID    : %s\n", monitor.DeviceID);
			common->Printf("      DeviceKey   : %s\n", monitor.DeviceKey);

			DEVMODE currentDevmode = {};
			if (!EnumDisplaySettings(device.DeviceName,ENUM_CURRENT_SETTINGS, &currentDevmode))
			{
				common->Printf("ERROR:  EnumDisplaySettings(ENUM_CURRENT_SETTINGS) failed!\n");
			}
			common->Printf("          -------------------\n");
			common->Printf("          ENUM_CURRENT_SETTINGS\n");
			PrintDevMode(currentDevmode);

			DEVMODE registryDevmode = {};
			if (!EnumDisplaySettings(device.DeviceName,ENUM_REGISTRY_SETTINGS, &registryDevmode))
			{
				common->Printf("ERROR:  EnumDisplaySettings(ENUM_CURRENT_SETTINGS) failed!\n");
			}
			common->Printf("          -------------------\n");
			common->Printf("          ENUM_CURRENT_SETTINGS\n");
			PrintDevMode(registryDevmode);

			for (int modeNum = 0; ; modeNum++)
			{
				DEVMODE devmode = {};

				if (!EnumDisplaySettings(device.DeviceName, modeNum, &devmode))
				{
					break;
				}

				if (devmode.dmBitsPerPel != 32)
				{
					continue;
				}
				if (devmode.dmDisplayFrequency < 60)
				{
					continue;
				}
				if (devmode.dmPelsHeight < 720)
				{
					continue;
				}
				common->Printf("          -------------------\n");
				common->Printf("          modeNum             : %i\n", modeNum);
				PrintDevMode(devmode);
			}
		}
	}
	common->Printf("\n");
}

/*
====================
R_GetModeListForDisplay
====================
*/
bool R_GetModeListForDisplay(const int requestedDisplayNum, idList<vidMode_t>& modeList)
{
	modeList.Clear();

	bool verbose = false;

	for (int displayNum = requestedDisplayNum; ; displayNum++)
	{
		DISPLAY_DEVICE device;
		device.cb = sizeof(device);
		if (!EnumDisplayDevices(
			0, // lpDevice
			displayNum,
			&device,
			0 /* dwFlags */))
		{
			return false;
		}

		// get the monitor for this display
		if (!(device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
		{
			continue;
		}

		DISPLAY_DEVICE monitor;
		monitor.cb = sizeof(monitor);
		if (!EnumDisplayDevices(
			device.DeviceName,
			0,
			&monitor,
			0 /* dwFlags */))
		{
			continue;
		}

		DEVMODE devmode;
		devmode.dmSize = sizeof(devmode);

		if (verbose)
		{
			common->Printf("display device: %i\n", displayNum);
			common->Printf("  DeviceName  : %s\n", device.DeviceName);
			common->Printf("  DeviceString: %s\n", device.DeviceString);
			common->Printf("  StateFlags  : 0x%x\n", device.StateFlags);
			common->Printf("  DeviceID    : %s\n", device.DeviceID);
			common->Printf("  DeviceKey   : %s\n", device.DeviceKey);
			common->Printf("      DeviceName  : %s\n", monitor.DeviceName);
			common->Printf("      DeviceString: %s\n", monitor.DeviceString);
			common->Printf("      StateFlags  : 0x%x\n", monitor.StateFlags);
			common->Printf("      DeviceID    : %s\n", monitor.DeviceID);
			common->Printf("      DeviceKey   : %s\n", monitor.DeviceKey);
		}

		for (int modeNum = 0; ; modeNum++)
		{
			if (!EnumDisplaySettings(device.DeviceName, modeNum, &devmode))
			{
				break;
			}

			if (devmode.dmBitsPerPel != 32)
			{
				continue;
			}
			if ((devmode.dmDisplayFrequency != 60) && (devmode.dmDisplayFrequency != 120))
			{
				continue;
			}
			if (devmode.dmPelsHeight < 720)
			{
				continue;
			}
			if (verbose)
			{
				common->Printf("          -------------------\n");
				common->Printf("          modeNum             : %i\n", modeNum);
				common->Printf("          dmPosition.x        : %i\n", devmode.dmPosition.x);
				common->Printf("          dmPosition.y        : %i\n", devmode.dmPosition.y);
				common->Printf("          dmBitsPerPel        : %i\n", devmode.dmBitsPerPel);
				common->Printf("          dmPelsWidth         : %i\n", devmode.dmPelsWidth);
				common->Printf("          dmPelsHeight        : %i\n", devmode.dmPelsHeight);
				common->Printf("          dmDisplayFixedOutput: %s\n", DMDFO(devmode.dmDisplayFixedOutput));
				common->Printf("          dmDisplayFlags      : 0x%x\n", devmode.dmDisplayFlags);
				common->Printf("          dmDisplayFrequency  : %i\n", devmode.dmDisplayFrequency);
			}
			vidMode_t mode;
			mode.width = devmode.dmPelsWidth;
			mode.height = devmode.dmPelsHeight;
			mode.displayHz = devmode.dmDisplayFrequency;
			modeList.AddUnique(mode);
		}
		if (modeList.Num() > 0)
		{
			class idSort_VidMode : public idSort_Quick<vidMode_t, idSort_VidMode>
			{
			public:
				int Compare(const vidMode_t& a, const vidMode_t& b) const
				{
					int wd = a.width - b.width;
					int hd = a.height - b.height;
					int fd = a.displayHz - b.displayHz;
					return (hd != 0) ? hd : (wd != 0) ? wd : fd;
				}
			};

			// sort with lowest resolution first
			modeList.SortWithTemplate(idSort_VidMode());

			return true;
		}
	}
	// Never gets here
}
