#pragma once
#include "framework.h"

#define IOCTL_GET_PROCINFO		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1730, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

#define IOCTL_READ_VMEM			CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1732, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

#define IOCTL_SET_CURSOR	    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1734, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

#define IOCTL_SET_WNDPROC	    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1736, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)



using PROCINFO = struct _PROCINFO
{
	DWORD64 clientBaseAddr;
	DWORD64 engineBaseAddr;
};
using PPROCINFO = PROCINFO *;


using READ = struct _READ
{
	DWORD64 readAddr;
	PUCHAR ourBuffer;
	DWORD64 Length;
	ULONG status;
};
using PREAD = READ *;


using CURSOR = struct _CURSOR
{
	POINT cursorPos;
};
using PCURSOR = CURSOR *;

using PROTECT = struct _PROTECT
{
	HWND hWnd;
	UINT dwFlag;
};
using PPROTECT = PROTECT *;


class Driver
{
private:
	HANDLE64 pDriver;
	LPWSTR RegistryPath = const_cast<LPWSTR>(L"\\\\.\\HelloCs2");
public:
	BOOL Init()
	{
		this->pDriver = CreateFileW(this->RegistryPath, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, NULL, nullptr);

		if (this->pDriver) return TRUE;

		return FALSE;
	}


	template <typename T>
	T fetch_Data(IN DWORD64 Address, IN int Offset)
	{
		if (!this->pDriver) return (T)FALSE;
		
		std::unique_ptr<READ> request{};
		DWORD Bytes = 0;
		T Buffer = {};
		

		request->readAddr = (Address + Offset);
		request->Length = sizeof(T);
		request->ourBuffer = reinterpret_cast<PUCHAR>(&Buffer);


		DeviceIoControl(this->pDriver, IOCTL_READ_VMEM, &request, sizeof(READ),
			&request, sizeof(READ), &Bytes, nullptr);
		
		return (T)Buffer;
	}

	VOID Read_To_Buffer(IN DWORD64 Address, IN DWORD Length, OUT PDWORD64 bufferAddr)
	{
		if (!this->pDriver) return ;
		std::unique_ptr<READ> request{};
		DWORD Bytes = 0;

		PVOID heapBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, Length);
		if (!heapBuffer) return;

		request->readAddr = Address;
		request->Length = Length;
		request->ourBuffer = reinterpret_cast<PUCHAR>(heapBuffer);

		DeviceIoControl(this->pDriver, IOCTL_READ_VMEM, &request, sizeof(READ),
			&request, sizeof(READ), &Bytes, nullptr);

		
		std::memcpy(bufferAddr, heapBuffer, Length);
		HeapFree(GetProcessHeap(), 0, heapBuffer);

	}

	
	VOID Get_Proc_Info(PDWORD64 clientBaseAddr, PDWORD64 engineBaseAddr)
	{
		if (!this->pDriver) return;

		std::unique_ptr<PROCINFO> request{};
		DWORD Bytes = 0;
	
		request->clientBaseAddr = 0;
		request->engineBaseAddr = 0;
	
		if (DeviceIoControl(this->pDriver, IOCTL_GET_PROCINFO, &request, sizeof(PROCINFO),
			&request, sizeof(PROCINFO), &Bytes, nullptr))
		{	
			*clientBaseAddr = request->clientBaseAddr;
			*engineBaseAddr = request->engineBaseAddr;
		}		
	}

	VOID Set_Cursor(POINT cursorPos)
	{
		if (!this->pDriver) return;
		
		std::unique_ptr<CURSOR> request{};
		DWORD Bytes = 0;

		request->cursorPos.x = cursorPos.x;
		request->cursorPos.y = cursorPos.y;


		DeviceIoControl(this->pDriver, IOCTL_SET_CURSOR, &request, sizeof(CURSOR),
			&request, sizeof(CURSOR), &Bytes, nullptr);

		return;
	}


	VOID Set_Window_Protection(HWND hWnd, UINT dwFlag)
	{
		if (!this->pDriver) return;

		std::unique_ptr<PROTECT> request{};
		DWORD Bytes = 0;

		request->hWnd = hWnd;
		request->dwFlag = dwFlag;

		DeviceIoControl(this->pDriver, IOCTL_SET_WNDPROC, &request, sizeof(PROTECT),
			&request, sizeof(PROTECT), &Bytes, nullptr);

		return;
	}
};


inline Driver driver;

