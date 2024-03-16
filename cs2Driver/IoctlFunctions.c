#include "IoctlFunctions.h"
#include "Memory.h"
#include "SystemModules.h"

///Global Variables
DWORD64 clientDllBase = 0;
DWORD64 engineDllBase = 0;
DWORD64 cs2CR3 = 0;
ULONG64 miGetPte = 0;
PVOID krnlBase = NULL;
SIZE_T krnlSize = 0;



VOID ImageLoadCallback(
	PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo)
{
	UNREFERENCED_PARAMETER(ProcessId);
	if (wcsstr(FullImageName->Buffer, _CLIENT))
	{
#ifdef _DEBUG
		DbgPrint("Found ClientDll");
#endif // _DEBUG
		
		engineDllBase = (DWORD64)ImageInfo->ImageBase;

		cs2CR3 = Process_CR3(_EXE, NULL);
	}
	if (wcsstr(FullImageName->Buffer, _ENGINE))
	{
#ifdef _DEBUG
		DbgPrint("Found EngineDll");
#endif // _DEBUG
		clientDllBase = (DWORD64)ImageInfo->ImageBase;
	}

}


NTSTATUS Read_Game(IN PIO_STACK_LOCATION ourStack, IN PIRP Irp)
{
	ULONG status = 1;
	DWORD	bytesIO = 0;
	
	PREAD request = (PREAD)Irp->AssociatedIrp.SystemBuffer;

	if (ourStack->Parameters.DeviceIoControl.InputBufferLength == sizeof(READ) &&
		ourStack->Parameters.DeviceIoControl.OutputBufferLength == sizeof(READ))
	{
#ifdef _DEBUG
		DbgPrint("Read_Game Function Called\n");
#endif // _DEBUG
		DbgPrint("Read Address [ 0x%I64X ]\n", request->readAddr);

		MM_COPY_ADDRESS pAddr = { 0 };
		ULONG busAddr = 0;
		SIZE_T rtBytes = 0;
		VIRTUAL_ADDRESS Linear = { 0 };

		if (cs2CR3 == 0)
		{
			cs2CR3 = Process_CR3(_EXE, &status);
		}

		Linear.value = request->readAddr;
		
		pAddr.PhysicalAddress.QuadPart = VtoP(Linear, cs2CR3, NULL);
			
		BOOL translate = FALSE;
		translate = HalTranslateBusAddress(0, 0, pAddr.PhysicalAddress, &busAddr, &pAddr.PhysicalAddress);
	
		MmCopyMemory(request->ourBuffer, pAddr, request->Length, MM_COPY_MEMORY_PHYSICAL, &rtBytes);

	
		bytesIO = sizeof(READ);
		request->status = status;

		Irp->IoStatus.Status = STATUS_SUCCESS;
		Irp->IoStatus.Information = bytesIO;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);

		return Irp->IoStatus.Status;
	}
	

	bytesIO = sizeof(READ);
	request->status = status;
	Irp->IoStatus.Status = (NTSTATUS)STATUS_INFO_LENGTH_MISMATCH;
	Irp->IoStatus.Information = bytesIO;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Irp->IoStatus.Status;
}


NTSTATUS Game_Info(IN PIO_STACK_LOCATION ourStack, IN PIRP Irp)
{
	DWORD	bytesIO = 0;

	if (ourStack->Parameters.DeviceIoControl.InputBufferLength == sizeof(PROCINFO) &&
		ourStack->Parameters.DeviceIoControl.OutputBufferLength == sizeof(PROCINFO))
	{
		PPROCINFO request = (PPROCINFO)Irp->AssociatedIrp.SystemBuffer;


		request->clientBaseAddr = clientDllBase;
		request->engineBaseAddr = engineDllBase;

		bytesIO = sizeof(PROCINFO);
	
		Irp->IoStatus.Status = STATUS_SUCCESS;
		Irp->IoStatus.Information = bytesIO;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);

		return Irp->IoStatus.Status;
	}


	bytesIO = 0;

	Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
	Irp->IoStatus.Information = bytesIO;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Irp->IoStatus.Status;
}



NTSTATUS Set_Cursor(IN PIO_STACK_LOCATION ourStack, IN PIRP Irp)
{
	
	PEPROCESS	 process = NULL;
	KAPC_STATE	  state = { 0 };
	PVOID gptCursorAsync = NULL,
			  win32kBase = NULL;
	ULONG			 status = 1;
	SIZE_T	        bytesIO = 0;
	
	
	if (ourStack->Parameters.DeviceIoControl.InputBufferLength == sizeof(CURSOR) &&
		ourStack->Parameters.DeviceIoControl.OutputBufferLength == sizeof(CURSOR))
	{
		PCURSOR request = (PCURSOR)Irp->AssociatedIrp.SystemBuffer;

		process = Process(_WINLOG);
		if (!process)
		{
#ifdef _DEBUG
			DbgPrint("Set Cursor - Failed To Fet Process Details\n");
#endif // _DEBUG

			goto cleanup;
		}
		
		win32kBase = Query_Module(_WIN32K, NULL, &status);
		if (status != (ULONG)STATUS_SUCCESS)
		{
#ifdef _DEBUG
			DbgPrint("Set Cursor - Failed To Fetch Module Base With ERROR : %lu\n", status);
#endif // _DEBUG

			goto cleanup;
		}

		gptCursorAsync = Function_Addr(win32kBase, _SETCURSOR);
		if (!gptCursorAsync)
		{
#ifdef _DEBUG
			DbgPrint("gptCursorAsync Failed Due Top Null Pointer\n");
#endif // _DEBUG

			goto cleanup;
		}

		KeStackAttachProcess(process, &state);

		*(PPOINT)(gptCursorAsync) = request->cursorPos;

		KeUnstackDetachProcess(&state);

#ifdef _DEBUG
		DbgPrint("Set New Cursor Position\n");
#endif // _DEBUG

		bytesIO = sizeof(CURSOR);

		Irp->IoStatus.Status = STATUS_SUCCESS;
		Irp->IoStatus.Information = bytesIO;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);

		return Irp->IoStatus.Status;
	}

cleanup:
	Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
	Irp->IoStatus.Information = bytesIO;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return status;
}


NTSTATUS Set_Display_Protection(IN PIO_STACK_LOCATION ourStack, IN PIRP Irp)
{
	PEPROCESS		   process = NULL;
	KAPC_STATE		    state = { 0 };
	PVOID SetDisplayProtection = NULL, 
				    win32kBase = NULL;
	ULONG				   status = 1;
	NTSTATUS		    ntstatus = -1;
	SIZE_T				  bytesIO = 0;
	pfnNtUserSetWindowDisplayAffinity NtUserSetWindowDisplayAffinity = NULL;


	if (ourStack->Parameters.DeviceIoControl.InputBufferLength == sizeof(PROTECT) &&
		ourStack->Parameters.DeviceIoControl.OutputBufferLength == sizeof(PROTECT))
	{
		PPROTECT request = (PPROTECT)Irp->AssociatedIrp.SystemBuffer;

		win32kBase = Query_Module(_WIN32K, NULL, &status);
		if (status != (ULONG)STATUS_SUCCESS)
		{
#ifdef _DEBUG
			DbgPrint("Set Display Protection - Failed To Fetch Module Base With ERROR : %lu\n", status);
#endif // _DEBUG
			goto cleanup;
		}

		SetDisplayProtection = Function_Addr(win32kBase, _SETPROTECT);
		if (!SetDisplayProtection)
		{
#ifdef _DEBUG
			DbgPrint("Set Display Protection Failed Due To Null Pointe\n");
#endif // _DEBUG
			
			goto cleanup;
		}

		process = Process(_WINLOG);
		if (!process)
		{
#ifdef _DEBUG
			DbgPrint("Set Display Protection - Failed To Fet Process Details\n");
#endif // _DEBUG

			goto cleanup;
		}
	
		KeStackAttachProcess(process, &state);
		NtUserSetWindowDisplayAffinity = (pfnNtUserSetWindowDisplayAffinity)SetDisplayProtection;

		if (!NT_SUCCESS(ntstatus = NtUserSetWindowDisplayAffinity(request->hWnd, request->dwFlag)))
		{
			KeUnstackDetachProcess(&state);
#ifdef _DEBUG
			DbgPrint("Function Failed With ERROR : %llu\n", ntstatus);
#endif // _DEBUG
			goto cleanup;
		}
		KeUnstackDetachProcess(&state);


#ifdef _DEBUG
		DbgPrint("Changed Window Display Affinity Via Alternate Route\n");
#endif // _DEBUG

		bytesIO = sizeof(PROTECT);

		Irp->IoStatus.Status = STATUS_SUCCESS;
		Irp->IoStatus.Information = bytesIO;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);

		return Irp->IoStatus.Status;

	}

cleanup:
	Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
	Irp->IoStatus.Information = bytesIO;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return status;
}




NTSTATUS Kernel_Info(IN PIO_STACK_LOCATION ourStack, IN PIRP Irp)
{
	DWORD bytesIO = 0;
	ULONG status = 1;

	UCHAR _MIGETPTEADDRESS[] = { 0x48, 0xC1, 0xE9, 0x09, 0x48, 0xB8, 0xF8, 0xFF, 0xFF, 0xFF, 0x7f, 0x0, 0x0, 0x0, 0x48, 0x23, 0xC8, 0x48, 0xB8 };

	if (ourStack->Parameters.DeviceIoControl.InputBufferLength == sizeof(KERNELINFO) &&
		ourStack->Parameters.DeviceIoControl.OutputBufferLength == sizeof(KERNELINFO))
	{
		PKERNELINFO request = (PKERNELINFO)Irp->AssociatedIrp.SystemBuffer;

		if (!krnlBase)
		{
			krnlBase = Query_Module(_KRNL, &krnlSize, &status);

#ifdef _DEBUG
			DbgPrint("Kernel Base At : 0x%p\n", krnlBase);
#endif // _DEBUG
		}

		if (!miGetPte)
		{
			miGetPte = (ULONG64)Signature_Scanner(C_PTR((U_PTR(krnlBase) + _MIGETPTEOFFSET)), PAGE_SIZE, _MIGETPTEADDRESS, sizeof(_MIGETPTEADDRESS));

#ifdef _DEBUG
			DbgPrint("Sig Found At : 0x%I64X\n", miGetPte);
#endif // _DEBUG
		}
		if (miGetPte == 0) status = (ULONG)STATUS_ACPI_ADDRESS_NOT_MAPPED;
	

		request->returnBuffer = *(ULONG64*)(miGetPte + 0x13);
#ifdef _DEBUG
		DbgPrint("PTE Base At: 0x%I64X\n", request->returnBuffer);
#endif // _DEBUG
		
		if (request->returnBuffer == 0) status = (ULONG)STATUS_ADDRESS_NOT_ASSOCIATED;

		request->status = status;
		

		bytesIO = sizeof(KERNELINFO);

		Irp->IoStatus.Status = STATUS_SUCCESS;
		Irp->IoStatus.Information = bytesIO;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);

		return Irp->IoStatus.Status;
	}


	bytesIO = 0;

	Irp->IoStatus.Status = STATUS_INFO_LENGTH_MISMATCH;
	Irp->IoStatus.Information = bytesIO;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Irp->IoStatus.Status;
}