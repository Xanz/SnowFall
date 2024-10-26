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
				// common->Printf("          dmDisplayFixedOutput: %s\n", DMDFO(devmode.dmDisplayFixedOutput));
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
