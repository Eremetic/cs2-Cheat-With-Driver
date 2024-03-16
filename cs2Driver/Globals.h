#pragma once
#include <ntdef.h>
#include "ntifs.h"
#include <ntddk.h>
#include <minwindef.h>


#define _DEBUG 1

///dlls names and exe
#define _CLIENT  L"C:\\ProgramFiles(x86)\\Steam\\steamapps\\common\\Counter-StrikeGlobalOffensive\\game\\bin\\win64\\engine2.dll"
#define _ENGINE L"C:\\ProgramFiles(x86)\\Steam\\steamapps\\common\\Counter-StrikGlobalOffensive\\game\\csgo\\bin\\win64\\client.dll"
#define _EXE L"cs2.exe"
#define _ASC L"ac_client.exe"
#define _WINLOG L"winlogon.exe"
#define _WIN32K L"win32kbase.sys"
#define _SETCURSOR "gptCursorAsync"
#define _SETPROTECT "NtUserSetWindowDisplayAffinity"

///Pool Tag
#define TAG (ULONG)1300135

///RSHasher
//size of char bit
#define CHAR_BIT 8
//SEED
#define _SEED 13

///macros
#define C_PTR( x ) ((void*) x )
#define U_PTR( x ) ((ULONG64) x )
#define PointerToOffset(Base, Offset)		((ULONG64)(((ULONG64)(Base)) + ((ULONG64)(Offset))))
#define OffsetToPointer(Pointer, Base)		((ULONG64)(((ULONG64)(Pointer)) - ((ULONG64)(Base))))

typedef HANDLE HWND;


///PEPROCESS Offsets
enum _OFFSETS
{
	DirectoryTableBase = 0x28,	///ULONGLONG
};

/// structs
typedef union _VIRTUAL_ADDRESS
{
	PVOID pValue;
	ULONG_PTR value;
	struct
	{
		ULONG_PTR offset : 12;
		ULONG_PTR pt_index : 9;
		ULONG_PTR pd_index : 9;
		ULONG_PTR pdp_index : 9;
		ULONG_PTR pml4_index : 9;
		ULONG_PTR reserved : 16;
	}Bit;
} VIRTUAL_ADDRESS, * PVIRTUAL_ADDRESS;

typedef struct tagPOINT 
{
	LONG x;
	LONG y;
} POINT, * PPOINT, * NPPOINT, * LPPOINT;


/// for ZwQuerySystemInformation
typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemProcessInformation = 5,
	SystemModuleInformation = 11
} SYSTEM_INFORMATION_CLASS;


///ErrorCodes
#define STATUS_FAILED_BUFFER_ALLOC	0x0f34f5
#define STATUS_FAILED_TO_READ_MEM  0x0f75171
#define STATUS_MM_COPY_FAILED	   0x01710ff
#define STATUS_FAILED_BASE_ADDR       0x0f34
#define STATUS_FAILED_EPROCESS        0x0397
#define STATUS_FAILED_PML4E		   0xff91474
#define STATUS_FAILED_PDPTE		   0xff87361
#define STATUS_FAILED_LRGPDPTE	  0xff873611
#define STATUS_FAILED_PDE		  0xff772251
#define STATUS_FAILED_LRGPDE	  0xff772222
#define STATUS_FAILED_PTE		   0xff00EA1
#define STATUS_FAILED_OPENSECTION   0xee4321
#define STATUS_FAILED_HANDLE_REF    0xef8698
#define STATUS_FAILED_HAL 			0xea9921
#define STATUS_FAILED_UNMAP			0xeb0726
#define STATUS_FAILED_MAP			0xeb6270
#define STATUS_FAILED_CR3			 0x06393



///undocumented API's
NTKERNELAPI
NTSTATUS
IoCreateDriver(
	IN PUNICODE_STRING DriverName,
	OPTIONAL IN PDRIVER_INITIALIZE InitializationFunction
);


NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySystemInformation(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	OUT PVOID SystemInformation, IN ULONG SystemInformationLength,
	OUT PULONG ReturnLength OPTIONAL
);

typedef NTSTATUS (__stdcall* pfnNtUserSetWindowDisplayAffinity)(
	IN HWND hWnd,
	IN DWORD dwFlag
);


NTKERNELAPI
PVOID
PsGetProcessSectionBaseAddress(
	IN PEPROCESS Process
);
