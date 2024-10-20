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

#pragma warning(disable:4740)	// warning C4740: flow in or out of inline asm code suppresses global optimization
#pragma warning(disable:4731)	// warning C4731: 'XXX' : frame pointer register 'ebx' modified by inline assembly code

/*
==============================================================

	Clock ticks

==============================================================
*/

/*
================
Sys_GetClockTicks
================
*/
double Sys_GetClockTicks()
{
	LARGE_INTEGER li;

	QueryPerformanceCounter(&li);
	return (double)li.LowPart + (double)0xFFFFFFFF * li.HighPart;
}

/*
================
Sys_ClockTicksPerSecond
================
*/
double Sys_ClockTicksPerSecond()
{
	static double ticks = 0;
	if (!ticks)
	{
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		ticks = li.QuadPart;
	}

	return ticks;
}


/*
==============================================================

	CPU

==============================================================
*/

#define _REG_EAX		0
#define _REG_EBX		1
#define _REG_ECX		2
#define _REG_EDX		3

/*
================================================================================================

	CPU

================================================================================================
*/

/*
========================
CountSetBits
Helper function to count set bits in the processor mask.
========================
*/
DWORD CountSetBits(ULONG_PTR bitMask)
{
	DWORD LSHIFT = sizeof(ULONG_PTR) * 8 - 1;
	DWORD bitSetCount = 0;
	ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;

	for (DWORD i = 0; i <= LSHIFT; i++)
	{
		bitSetCount += ((bitMask & bitTest) ? 1 : 0);
		bitTest /= 2;
	}

	return bitSetCount;
}

typedef BOOL (WINAPI *LPFN_GLPI)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);

enum LOGICAL_PROCESSOR_RELATIONSHIP_LOCAL
{
	localRelationProcessorCore,
	localRelationNumaNode,
	localRelationCache,
	localRelationProcessorPackage
};

struct cpuInfo_t
{
	int processorPackageCount;
	int processorCoreCount;
	int logicalProcessorCount;
	int numaNodeCount;

	struct cacheInfo_t
	{
		int count;
		int associativity;
		int lineSize;
		int size;
	} cacheLevel[3];
};

/*
========================
GetCPUInfo
========================
*/
bool GetCPUInfo(cpuInfo_t& cpuInfo)
{
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
	PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
	PCACHE_DESCRIPTOR Cache;
	LPFN_GLPI glpi;
	BOOL done = FALSE;
	DWORD returnLength = 0;
	DWORD byteOffset = 0;

	memset(&cpuInfo, 0, sizeof(cpuInfo));

	glpi = (LPFN_GLPI)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "GetLogicalProcessorInformation");
	if (NULL == glpi)
	{
		idLib::Printf("\nGetLogicalProcessorInformation is not supported.\n");
		return 0;
	}

	while (!done)
	{
		DWORD rc = glpi(buffer, &returnLength);

		if (FALSE == rc)
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				if (buffer)
				{
					free(buffer);
				}

				buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(returnLength);
			}
			else
			{
				idLib::Printf("Sys_CPUCount error: %d\n", GetLastError());
				return false;
			}
		}
		else
		{
			done = TRUE;
		}
	}

	ptr = buffer;

	while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength)
	{
		switch ((LOGICAL_PROCESSOR_RELATIONSHIP_LOCAL)ptr->Relationship)
		{
		case localRelationProcessorCore:
			cpuInfo.processorCoreCount++;

		// A hyperthreaded core supplies more than one logical processor.
			cpuInfo.logicalProcessorCount += CountSetBits(ptr->ProcessorMask);
			break;

		case localRelationNumaNode:
			// Non-NUMA systems report a single record of this type.
			cpuInfo.numaNodeCount++;
			break;

		case localRelationCache:
			// Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache.
			Cache = &ptr->Cache;
			if (Cache->Level >= 1 && Cache->Level <= 3)
			{
				int level = Cache->Level - 1;
				if (cpuInfo.cacheLevel[level].count > 0)
				{
					cpuInfo.cacheLevel[level].count++;
				}
				else
				{
					cpuInfo.cacheLevel[level].associativity = Cache->Associativity;
					cpuInfo.cacheLevel[level].lineSize = Cache->LineSize;
					cpuInfo.cacheLevel[level].size = Cache->Size;
				}
			}
			break;

		case localRelationProcessorPackage:
			// Logical processors share a physical package.
			cpuInfo.processorPackageCount++;
			break;

		default:
			idLib::Printf("Error: Unsupported LOGICAL_PROCESSOR_RELATIONSHIP value.\n");
			break;
		}
		byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
		ptr++;
	}

	free(buffer);

	return true;
}

/*
========================
Sys_GetCPUCacheSize
========================
*/
void Sys_GetCPUCacheSize(int level, int& count, int& size, int& lineSize)
{
	assert(level >= 1 && level <= 3);
	cpuInfo_t cpuInfo;

	GetCPUInfo(cpuInfo);

	count = cpuInfo.cacheLevel[level - 1].count;
	size = cpuInfo.cacheLevel[level - 1].size;
	lineSize = cpuInfo.cacheLevel[level - 1].lineSize;
}

/*
========================
Sys_CPUCount

numLogicalCPUCores	- the number of logical CPU per core
numPhysicalCPUCores	- the total number of cores per package
numCPUPackages		- the total number of packages (physical processors)
========================
*/
void Sys_CPUCount(int& numLogicalCPUCores, int& numPhysicalCPUCores, int& numCPUPackages)
{
	cpuInfo_t cpuInfo;
	GetCPUInfo(cpuInfo);

	numPhysicalCPUCores = cpuInfo.processorCoreCount;
	numLogicalCPUCores = cpuInfo.logicalProcessorCount;
	numCPUPackages = cpuInfo.processorPackageCount;
}

/*
================
Sys_GetCPUId
================
*/
cpuid_t Sys_GetCPUId()
{
	int flags = CPUID_GENERIC;

	flags |= CPUID_SSE;
	flags |= CPUID_SSE2;

	return (cpuid_t)flags;
}


/*
===============================================================================

	FPU

===============================================================================
*/

typedef struct bitFlag_s
{
	const char* name;
	int bit;
} bitFlag_t;

static byte fpuState[128], *statePtr = fpuState;
static char fpuString[2048];
static bitFlag_t controlWordFlags[] = {
	{"Invalid operation", 0},
	{"Denormalized operand", 1},
	{"Divide-by-zero", 2},
	{"Numeric overflow", 3},
	{"Numeric underflow", 4},
	{"Inexact result (precision)", 5},
	{"Infinity control", 12},
	{"", 0}
};
static const char* precisionControlField[] = {
	"Single Precision (24-bits)",
	"Reserved",
	"Double Precision (53-bits)",
	"Double Extended Precision (64-bits)"
};
static const char* roundingControlField[] = {
	"Round to nearest",
	"Round down",
	"Round up",
	"Round toward zero"
};
static bitFlag_t statusWordFlags[] = {
	{"Invalid operation", 0},
	{"Denormalized operand", 1},
	{"Divide-by-zero", 2},
	{"Numeric overflow", 3},
	{"Numeric underflow", 4},
	{"Inexact result (precision)", 5},
	{"Stack fault", 6},
	{"Error summary status", 7},
	{"FPU busy", 15},
	{"", 0}
};

/*
===============
Sys_FPU_PrintStateFlags
===============
*/
int Sys_FPU_PrintStateFlags(char* ptr, int ctrl, int stat, int tags, int inof, int inse, int opof, int opse)
{
	int i, length = 0;

	length += sprintf(ptr + length, "CTRL = %08x\n"
	                  "STAT = %08x\n"
	                  "TAGS = %08x\n"
	                  "INOF = %08x\n"
	                  "INSE = %08x\n"
	                  "OPOF = %08x\n"
	                  "OPSE = %08x\n"
	                  "\n",
	                  ctrl, stat, tags, inof, inse, opof, opse);

	length += sprintf(ptr + length, "Control Word:\n");
	for (i = 0; controlWordFlags[i].name[0]; i++)
	{
		length += sprintf(ptr + length, "  %-30s = %s\n", controlWordFlags[i].name,
		                  (ctrl & (1 << controlWordFlags[i].bit)) ? "true" : "false");
	}
	length += sprintf(ptr + length, "  %-30s = %s\n", "Precision control", precisionControlField[(ctrl >> 8) & 3]);
	length += sprintf(ptr + length, "  %-30s = %s\n", "Rounding control", roundingControlField[(ctrl >> 10) & 3]);

	length += sprintf(ptr + length, "Status Word:\n");
	for (i = 0; statusWordFlags[i].name[0]; i++)
	{
		ptr += sprintf(ptr + length, "  %-30s = %s\n", statusWordFlags[i].name,
		               (stat & (1 << statusWordFlags[i].bit)) ? "true" : "false");
	}
	length += sprintf(ptr + length, "  %-30s = %d%d%d%d\n", "Condition code", (stat >> 8) & 1, (stat >> 9) & 1,
	                  (stat >> 10) & 1, (stat >> 14) & 1);
	length += sprintf(ptr + length, "  %-30s = %d\n", "Top of stack pointer", (stat >> 11) & 7);

	return length;
}

/*
===============
Sys_FPU_StackIsEmpty
===============
*/
bool Sys_FPU_StackIsEmpty()
{
	return true;
}

/*
===============
Sys_FPU_ClearStack
===============
*/
void Sys_FPU_ClearStack()
{
}

/*
===============
Sys_FPU_GetState

  gets the FPU state without changing the state
===============
*/
const char* Sys_FPU_GetState()
{
	return "TODO Sys_FPU_GetState()";
}

/*
===============
Sys_FPU_EnableExceptions
===============
*/
void Sys_FPU_EnableExceptions(int exceptions)
{
}

/*
===============
Sys_FPU_SetPrecision
===============
*/
void Sys_FPU_SetPrecision(int precision)
{
}

/*
================
Sys_FPU_SetRounding
================
*/
void Sys_FPU_SetRounding(int rounding)
{
}

/*
================
Sys_FPU_SetDAZ
================
*/
void Sys_FPU_SetDAZ(bool enable)
{
}

/*
================
Sys_FPU_SetFTZ
================
*/
void Sys_FPU_SetFTZ(bool enable)
{
}
