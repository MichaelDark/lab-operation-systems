// lab4_sysinfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void PrintSystemInfo(LPSYSTEM_INFO sysInfo);
void PrintMemoryStatus(LPMEMORYSTATUSEX memStatus);
void PrintMemoryBasicInfo(PMEMORY_BASIC_INFORMATION memInfo);

int main()
{
	SYSTEM_INFO sysInfo;
	MEMORYSTATUSEX memStatus = { sizeof(memStatus) };
	MEMORY_BASIC_INFORMATION memInfo;
	int a = 5;

	GetSystemInfo(&sysInfo);
	PrintSystemInfo(&sysInfo);

	GlobalMemoryStatusEx(&memStatus);
	PrintMemoryStatus(&memStatus);

	VirtualQueryEx(GetCurrentProcess(), &a, &memInfo, sizeof(memInfo));
	PrintMemoryBasicInfo(&memInfo);

	system("pause");
    return 0;
}

void PrintSystemInfo(LPSYSTEM_INFO sysInfo)
{
	_tprintf(_T("===GetSystemInfo===\r\n"));
	_tprintf(_T("PageSize             : %16I64d\r\n"), sysInfo->dwPageSize);
	_tprintf(_T("MinApplicationAddres : 0x%I64x\r\n"), sysInfo->lpMinimumApplicationAddress);
	_tprintf(_T("MaxApplicationAddress: 0x%I64x\r\n"), sysInfo->lpMaximumApplicationAddress);
	_tprintf(_T("AllocationGranularity: %16I64d\r\n"), sysInfo->dwAllocationGranularity);
	_tprintf(_T("\r\n"));
}

void PrintMemoryStatus(LPMEMORYSTATUSEX memStatus)
{
	_tprintf(_T("===GlobalMemoryStatusEx===\r\n"));
	_tprintf(_T("Length                   : %16I64d\r\n"), memStatus->dwLength);
	_tprintf(_T("MemoryLoad               : %16I64d\r\n"), memStatus->dwMemoryLoad);
	_tprintf(_T("Physical  Available/Total: 0x%I64x - 0x%I64x\r\n"), memStatus->ullAvailPhys, memStatus->ullTotalPhys);
	_tprintf(_T("PageFile  Available/Total: 0x%I64x - 0x%I64x\r\n"), memStatus->ullAvailPageFile, memStatus->ullTotalPageFile);
	_tprintf(_T("Virtual   Available/Total: 0x%I64x - 0x%I64x\r\n"), memStatus->ullAvailVirtual, memStatus->ullTotalVirtual);
	_tprintf(_T("VirtualEx Available      : %16I64d\r\n"), memStatus->ullAvailExtendedVirtual);
	_tprintf(_T("\r\n"));
}

void PrintMemoryBasicInfo(PMEMORY_BASIC_INFORMATION memInfo)
{
	TCHAR* type = new TCHAR[25];
	TCHAR* state = new TCHAR[25];
	TCHAR* allocationProtect = new TCHAR[25];

	switch (memInfo->Type)
	{
		case MEM_IMAGE: { _tcscpy(type, _T("MEM_IMAGE")); break; }
		case MEM_MAPPED: { _tcscpy(type, _T("MEM_MAPPED")); break; }
		case MEM_PRIVATE: { _tcscpy(type, _T("MEM_PRIVATE")); break; }
	}

	switch (memInfo->State)
	{
		case MEM_COMMIT: { _tcscpy(state, _T("MEM_COMMIT")); break; }
		case MEM_RESERVE: { _tcscpy(state, _T("MEM_RESERVE")); break; }
		case MEM_FREE: { _tcscpy(state, _T("MEM_FREE")); break; }
	}
	switch (memInfo->AllocationProtect)
	{
		case PAGE_EXECUTE: { _tcscpy(allocationProtect, _T("PAGE_EXECUTE")); break; }
		case PAGE_EXECUTE_READ: { _tcscpy(allocationProtect, _T("PAGE_EXECUTE_READ")); break; }
		case PAGE_EXECUTE_READWRITE: { _tcscpy(allocationProtect, _T("PAGE_EXECUTE_READWRITE")); break; }
		case PAGE_EXECUTE_WRITECOPY: { _tcscpy(allocationProtect, _T("PAGE_EXECUTE_WRITECOPY")); break; }
		case PAGE_NOACCESS: { _tcscpy(allocationProtect, _T("PAGE_NOACCESS")); break; }
		case PAGE_READONLY: { _tcscpy(allocationProtect, _T("PAGE_READONLY")); break; }
		case PAGE_READWRITE: { _tcscpy(allocationProtect, _T("PAGE_READWRITE")); break; }
		case PAGE_WRITECOPY: { _tcscpy(allocationProtect, _T("PAGE_WRITECOPY")); break; }
	}

	_tprintf(_T("===MemoryBasicInfo===\r\n"));
	_tprintf(_T("BaseAddress      : 0x%I64x\r\n"), memInfo->BaseAddress);
	_tprintf(_T("AllocationBase   : 0x%I64x\r\n"), memInfo->AllocationBase);
	_tprintf(_T("AllocationProtect: %s\r\n"), allocationProtect);
	_tprintf(_T("RegionSize       : %16I64d\r\n"), memInfo->RegionSize);
	_tprintf(_T("State            : %s\r\n"), state);
	_tprintf(_T("Protect          : %16I64d\r\n"), memInfo->Protect);
	_tprintf(_T("Type             : %s\r\n"), type);
	_tprintf(_T("\r\n"));
}

