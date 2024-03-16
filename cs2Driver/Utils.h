#pragma once
#include "Globals.h"


BOOL RSHasher(IN PWCHAR String1, IN PWCHAR String2);

PVOID Signature_Scanner(IN PVOID Memory, IN SIZE_T Length, IN PVOID PatternBuffer, IN SIZE_T PatternLength);

VOID Fetch_Physical_Address(IN VIRTUAL_ADDRESS Linear, IN ULONG64 PTEbaseAddr, IN PVOID userModeBuffer, OUT PULONG status);


#define _MOVRAX 0x7FFFFFFFF8
#define _MIGETPTEOFFSET 0x299F24	///500 before function but we will scan PAGE_SIZE (0x1000)

