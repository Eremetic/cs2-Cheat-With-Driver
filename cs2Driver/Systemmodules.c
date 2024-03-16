#include "SystemModules.h"



PVOID Query_Module(IN WCHAR* module, OUT PSIZE_T moduleSize ,OUT PULONG returnstatus)
{
#ifdef _DEBUG 
	DbgPrint("[+]Query_Module() Function Called\n");
#endif

	PSYSTEM_MODULE_INFORMATION   p_smi;
	ULONG				  szBuffer = 0;
	SIZE_T				   szImage = 0;
	ANSI_STRING			  temp = { 0 };
	UNICODE_STRING	cur_Module = { 0 };
	PVOID			  Base_Addr = NULL;
	NTSTATUS				status = 1;


	do
	{
		///calling to get buffer size
		status = ZwQuerySystemInformation(SystemModuleInformation, 0, szBuffer, &szBuffer);

		///allocating memory for real call
		p_smi = ExAllocatePool2(POOL_FLAG_NON_PAGED, szBuffer, TAG);
		if (p_smi)
		{
			///real call to get information
			status = ZwQuerySystemInformation(SystemModuleInformation, p_smi, szBuffer, &szBuffer);
		}
		else
		{
			*returnstatus = STATUS_FAILED_BUFFER_ALLOC;
			return NULL;
		}


	} while (status == STATUS_INFO_LENGTH_MISMATCH);



	/// query through the modules to find the bass address and size
	PSYSTEM_MODULE_ENTRY p_me = p_smi->Module;

	for (ULONG_PTR i = 0; i < p_smi->Count; i++)
	{
		///converting uchar(ansi) to unicode
		RtlInitAnsiString(&temp, (PCSZ)((CHAR*)(p_me[i].FullPathName + p_me[i].OffsetToFileName)));
		RtlAnsiStringToUnicodeString(&cur_Module, &temp, TRUE);

		///finding user selected module
		if (RSHasher(module, cur_Module.Buffer))
		{
			Base_Addr = p_me[i].ImageBase;
			szImage = p_me[i].ImageSize;
			break;
		}
	}

	if (Base_Addr != NULL || szImage != 0)
	{
#ifdef _DEBUG 
		DbgPrint("[+]Query_Module() Operation Successfull\n");
#endif

		ExFreePool(p_smi);
		*moduleSize = szImage;
		return Base_Addr;
	}

	*returnstatus = STATUS_FAILED_BASE_ADDR;
#ifdef _DEBUG 
	DbgPrint("[!]Query_Module() Operation Unsuccessfull\n");
#endif

	return NULL;
}



PVOID Function_Addr(IN PVOID BaseAddress, IN const char* procName)
{
	LPBYTE         pBase = ((LPBYTE)BaseAddress);
	PVOID                      result = NULL;
	PIMAGE_DOS_HEADER      dos_Header = NULL;
	PIMAGE_NT_HEADERS64     nt_Header = NULL;
	PIMAGE_EXPORT_DIRECTORY exportDir = NULL;

	dos_Header = (PIMAGE_DOS_HEADER)BaseAddress;
	if (dos_Header->e_magic != IMAGE_DOS_SIG)
	{
		return NULL;
	}

	nt_Header = (PIMAGE_NT_HEADERS64)(pBase + dos_Header->e_lfanew);
	if (nt_Header->Signature != IMAGE_NT_SIG)
	{
		return NULL;
	}

	if ((nt_Header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress) == 0)
	{
		return NULL;
	}

	exportDir = (PIMAGE_EXPORT_DIRECTORY)(pBase + nt_Header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);


	PDWORD         nameArray = (PDWORD)(pBase + exportDir->AddressOfNames);
	PWORD      ordinal = (PWORD)(pBase + exportDir->AddressOfNameOrdinals);
	PDWORD  addressArray = (PDWORD)(pBase + exportDir->AddressOfFunctions);

	for (DWORD i = 0; i < exportDir->NumberOfNames; i++)
	{
		char* name = (char*)(pBase + nameArray[i]);
		if (name)
		{
			if (strcmp(procName, name) == 0)
			{
				result = C_PTR(((ULONG64)BaseAddress + addressArray[ordinal[i]]));
				return result;
			}
		}
	}
	return NULL;
}