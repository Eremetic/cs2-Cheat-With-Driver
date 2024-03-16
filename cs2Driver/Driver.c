#include "Globals.h"
#include "IoctlFunctions.h"
#include "Memory.h"
#include "Utils.h"


DRIVER_INITIALIZE DriverEntry;
#pragma alloc_text(INIT, DriverEntry)



static NTSTATUS UnsupportedDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return Irp->IoStatus.Status;
}



NTSTATUS DriverUnload(IN PDRIVER_OBJECT driverObject)
{
#ifdef _DEBUG
	DbgPrint("Driver Unload Called");
#endif // _DEBUG

	UNICODE_STRING symLink;

	RtlInitUnicodeString(&symLink, L"\\DosDevices\\HelloCs2");

	PsRemoveLoadImageNotifyRoutine(ImageLoadCallback);
	
	IoDeleteSymbolicLink(&symLink);
	IoDeleteDevice(driverObject->DeviceObject);

	return STATUS_SUCCESS;
}

static NTSTATUS CreateDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return Irp->IoStatus.Status;
}


static NTSTATUS CloseDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return Irp->IoStatus.Status;
}




static NTSTATUS IoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	ULONG													bytesIO = 0;
	NTSTATUS												 status = 1;
	PIO_STACK_LOCATION		  stack = IoGetCurrentIrpStackLocation(Irp);


	switch (stack->Parameters.DeviceIoControl.IoControlCode)
	{
	case IOCTL_GET_PROCINFO:
		status = Game_Info(stack, Irp);
		break;
	case IOCTL_READ_VMEM:
		status = Read_Game(stack, Irp);
		break;
	case IOCTL_SET_CURSOR:
		status = Set_Cursor(stack, Irp);
		break;
	case IOCTL_SET_WNDPROC:
		status = Set_Display_Protection(stack, Irp);
		break;
	case IOCTL_GET_KRNL_INFO:
		status = Kernel_Info(stack, Irp);
		break;
	case STATUS_INVALID_PARAMETER:
	{status = STATUS_INVALID_PARAMETER;
	bytesIO = 0; }
	break;
	}

	return status;
}



NTSTATUS DriverInitialize(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{

	NTSTATUS				      status;
	UNREFERENCED_PARAMETER(RegistryPath);

	UNICODE_STRING deviceName, symLink;
	PDEVICE_OBJECT deviceObject;

	PsSetLoadImageNotifyRoutine(ImageLoadCallback);
	

	RtlInitUnicodeString(&deviceName, L"\\Device\\HelloCs2");
	RtlInitUnicodeString(&symLink, L"\\DosDevices\\HelloCs2");


	if(NT_SUCCESS(status = IoCreateDevice(DriverObject, 0, &deviceName, FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN, FALSE, &deviceObject)))
	{
		if (!NT_SUCCESS(status = IoCreateSymbolicLink(&symLink, &deviceName)))
		{
			IoDeleteDevice(deviceObject);
			return status;
		}
	}
	else
	{
		return status;
	}

	deviceObject->Flags |= DO_BUFFERED_IO;

	for (ULONG t = 0; t <= IRP_MJ_MAXIMUM_FUNCTION; t++)
		DriverObject->MajorFunction[t] = UnsupportedDispatch;

	DriverObject->MajorFunction[IRP_MJ_CREATE] = CreateDispatch;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseDispatch;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControl;
	DriverObject->DriverUnload = DriverUnload;
	deviceObject->Flags &= ~DO_DEVICE_INITIALIZING;


	return status;
}


NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
#ifdef _DEBUG 
	DbgPrint("[+]Driver Entry called\n");
#endif

	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	NTSTATUS status = STATUS_UNSUCCESSFUL;

	if (NT_SUCCESS(status = IoCreateDriver(NULL, &DriverInitialize)))
	{

#ifdef _DEBUG 
		DbgPrint("[+]Driver Entry Successfull\n");
#endif
	}



	return status;
}

