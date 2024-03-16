#pragma once
#include "Globals.h"
#include "Utils.h"

PVOID Query_Module(IN WCHAR* module, OUT PSIZE_T moduleSize ,OUT PULONG returnstatus);

PVOID Function_Addr(IN PVOID BaseAddress, IN const char* procName);


#define _KRNL L"ntoskrnl.exe"

///custom definitions
#define IMAGE_DIRECTORY_ENTRY_EXPORT	    0
#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES   16
#define IMAGE_DOS_SIG				   0x5A4D
#define IMAGE_NT_SIG			   0x00004550
#define IMAGE_SIZEOF_SHORT_NAME				8
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT 11
#define IMAGE_DIRECTORY_ENTRY_IAT		   12
#define MEMORYREAD				   0x40000000
#define MEMORYWRITE				   0x80000000

///Image Header Structs
typedef struct _IMAGE_DOS_HEADER
{
	USHORT   e_magic;
	USHORT   e_cblp;
	USHORT   e_cp;
	USHORT   e_crlc;
	USHORT   e_cparhdr;
	USHORT   e_minalloc;
	USHORT   e_maxalloc;
	USHORT   e_ss;
	USHORT   e_sp;
	USHORT   e_csum;
	USHORT   e_ip;
	USHORT   e_cs;
	USHORT   e_lfarlc;
	USHORT   e_ovno;
	USHORT   e_res[4];
	USHORT   e_oemid;
	USHORT   e_oeminfo;
	USHORT   e_res2[10];
	LONG   e_lfanew;
} IMAGE_DOS_HEADER, * PIMAGE_DOS_HEADER;


typedef struct _IMAGE_SECTION_HEADER
{
	BYTE  Name[IMAGE_SIZEOF_SHORT_NAME];
	union {
		DWORD PhysicalAddress;
		DWORD VirtualSize;
	} Misc;
	DWORD VirtualAddress;
	DWORD SizeOfRawData;
	DWORD PointerToRawData;
	DWORD PointerToRelocations;
	DWORD PointerToLinenumbers;
	WORD  NumberOfRelocations;
	WORD  NumberOfLinenumbers;
	DWORD Characteristics;
} IMAGE_SECTION_HEADER, * PIMAGE_SECTION_HEADER;

typedef struct _IMAGE_IMPORT_BY_NAME
{
	WORD    Hint;
	CHAR   Name[1];
} IMAGE_IMPORT_BY_NAME, * PIMAGE_IMPORT_BY_NAME;

typedef struct _IMAGE_THUNK_DATA64
{
	union
	{
		ULONGLONG ForwarderString;  // PBYTE 
		ULONGLONG Function;         // PDWORD
		ULONGLONG Ordinal;
		ULONGLONG AddressOfData;    // PIMAGE_IMPORT_BY_NAME
	} u1;
} IMAGE_THUNK_DATA64, * PIMAGE_THUNK_DATA64;


typedef struct _IMAGE_IMPORT_DESCRIPTOR
{
	union
	{
		DWORD   Characteristics;
		DWORD   OriginalFirstThunk;
	}u1;
	DWORD   TimeDateStamp;
	DWORD   Name;
	DWORD   FirstThunk;
} IMAGE_IMPORT_DESCRIPTOR;
typedef IMAGE_IMPORT_DESCRIPTOR UNALIGNED* PIMAGE_IMPORT_DESCRIPTOR;


typedef struct _IMAGE_EXPORT_DIRECTORY
{
	DWORD Characteristics;
	DWORD TimeDateStamp;
	WORD MajorVersion;
	WORD MinorVersion;
	DWORD Name;
	DWORD Base;
	DWORD NumberOfFunctions;
	DWORD NumberOfNames;
	DWORD AddressOfFunctions;
	DWORD AddressOfNames;
	DWORD AddressOfNameOrdinals;
} IMAGE_EXPORT_DIRECTORY, * PIMAGE_EXPORT_DIRECTORY;


typedef struct _IMAGE_DATA_DIRECTORY
{
	DWORD VirtualAddress;
	DWORD Size;
} IMAGE_DATA_DIRECTORY, * PIMAGE_DATA_DIRECTORY;


typedef struct _IMAGE_OPTIONAL_HEADER64
{
	WORD                 Magic;
	BYTE                 MajorLinkerVersion;
	BYTE                 MinorLinkerVersion;
	DWORD                SizeOfCode;
	DWORD                SizeOfInitializedData;
	DWORD                SizeOfUninitializedData;
	DWORD                AddressOfEntryPoint;
	DWORD                BaseOfCode;
	ULONGLONG            ImageBase;
	DWORD                SectionAlignment;
	DWORD                FileAlignment;
	WORD                 MajorOperatingSystemVersion;
	WORD                 MinorOperatingSystemVersion;
	WORD                 MajorImageVersion;
	WORD                 MinorImageVersion;
	WORD                 MajorSubsystemVersion;
	WORD                 MinorSubsystemVersion;
	DWORD                Win32VersionValue;
	DWORD                SizeOfImage;
	DWORD                SizeOfHeaders;
	DWORD                CheckSum;
	WORD                 Subsystem;
	WORD                 DllCharacteristics;
	ULONGLONG            SizeOfStackReserve;
	ULONGLONG            SizeOfStackCommit;
	ULONGLONG            SizeOfHeapReserve;
	ULONGLONG            SizeOfHeapCommit;
	DWORD                LoaderFlags;
	DWORD                NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, * PIMAGE_OPTIONAL_HEADER64;

typedef struct _IMAGE_FILE_HEADER
{
	USHORT Machine;
	USHORT NumberOfSections;
	ULONG TimeDateStamp;
	ULONG PointerToSymbolTable;
	ULONG NumberOfSymbols;
	USHORT SizeOfOptionalHeader;
	USHORT Characteristics;
}IMAGE_FILE_HEADER, * PIMAGE_FILE_HEADER;

typedef struct _IMAGE_NT_HEADERS64
{
	DWORD                   Signature;
	IMAGE_FILE_HEADER       FileHeader;
	IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64, * PIMAGE_NT_HEADERS64;

typedef struct _SYSTEM_MODULE_ENTRY
{
	HANDLE Section;
	PVOID MappedBase;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	UCHAR  FullPathName[256];
} SYSTEM_MODULE_ENTRY, * PSYSTEM_MODULE_ENTRY;

typedef struct _SYSTEM_MODULE_INFORMATION
{
	ULONG Count;
	SYSTEM_MODULE_ENTRY Module[1];
} SYSTEM_MODULE_INFORMATION, * PSYSTEM_MODULE_INFORMATION;