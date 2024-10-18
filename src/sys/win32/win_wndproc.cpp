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
#include "../../idlib/precompiled.h"

#include "win_local.h"
#include "../../renderer/tr_local.h"

#include <Windowsx.h>

LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static bool s_alttab_disabled;

static void WIN_DisableAltTab()
{
	if (s_alttab_disabled || win32.win_allowAltTab.GetBool())
	{
		return;
	}
	if (!idStr::Icmp(cvarSystem->GetCVarString("sys_arch"), "winnt"))
	{
		RegisterHotKey(0, 0, MOD_ALT, VK_TAB);
	}
	else
	{
		BOOL old;

		SystemParametersInfo(SPI_SCREENSAVERRUNNING, 1, &old, 0);
	}
	s_alttab_disabled = true;
}

static void WIN_EnableAltTab()
{
	if (!s_alttab_disabled || win32.win_allowAltTab.GetBool())
	{
		return;
	}
	if (!idStr::Icmp(cvarSystem->GetCVarString("sys_arch"), "winnt"))
	{
		UnregisterHotKey(0, 0);
	}
	else
	{
		BOOL old;

		SystemParametersInfo(SPI_SCREENSAVERRUNNING, 0, &old, 0);
	}

	s_alttab_disabled = false;
}

void WIN_Sizing(WORD side, RECT* rect)
{
	if (!R_IsInitialized() || renderSystem->GetWidth() <= 0 || renderSystem->GetHeight() <= 0)
	{
		return;
	}

	// restrict to a standard aspect ratio
	int width = rect->right - rect->left;
	int height = rect->bottom - rect->top;

	// Adjust width/height for window decoration
	RECT decoRect = {0, 0, 0, 0};
	AdjustWindowRect(&decoRect, WINDOW_STYLE | WS_SYSMENU, FALSE);
	int decoWidth = decoRect.right - decoRect.left;
	int decoHeight = decoRect.bottom - decoRect.top;

	width -= decoWidth;
	height -= decoHeight;

	// Clamp to a minimum size
	if (width < SCREEN_WIDTH / 4)
	{
		width = SCREEN_WIDTH / 4;
	}
	if (height < SCREEN_HEIGHT / 4)
	{
		height = SCREEN_HEIGHT / 4;
	}

	const int minWidth = height * 4 / 3;
	const int maxHeight = width * 3 / 4;

	const int maxWidth = height * 16 / 9;
	const int minHeight = width * 9 / 16;

	// Set the new size
	switch (side)
	{
	case WMSZ_LEFT:
		rect->left = rect->right - width - decoWidth;
		rect->bottom = rect->top + idMath::ClampInt(minHeight, maxHeight, height) + decoHeight;
		break;
	case WMSZ_RIGHT:
		rect->right = rect->left + width + decoWidth;
		rect->bottom = rect->top + idMath::ClampInt(minHeight, maxHeight, height) + decoHeight;
		break;
	case WMSZ_BOTTOM:
	case WMSZ_BOTTOMRIGHT:
		rect->bottom = rect->top + height + decoHeight;
		rect->right = rect->left + idMath::ClampInt(minWidth, maxWidth, width) + decoWidth;
		break;
	case WMSZ_TOP:
	case WMSZ_TOPRIGHT:
		rect->top = rect->bottom - height - decoHeight;
		rect->right = rect->left + idMath::ClampInt(minWidth, maxWidth, width) + decoWidth;
		break;
	case WMSZ_BOTTOMLEFT:
		rect->bottom = rect->top + height + decoHeight;
		rect->left = rect->right - idMath::ClampInt(minWidth, maxWidth, width) - decoWidth;
		break;
	case WMSZ_TOPLEFT:
		rect->top = rect->bottom - height - decoHeight;
		rect->left = rect->right - idMath::ClampInt(minWidth, maxWidth, width) - decoWidth;
		break;
	}
}

/*
====================
MainWndProc

main window procedure
====================
*/
LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
