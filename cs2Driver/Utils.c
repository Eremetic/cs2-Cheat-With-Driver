#include "Utils.h"
#include "Memory.h"

static UINT32 RS_Sub(UINT32 Value, UINT Count)
{
	DWORD Mask = (CHAR_BIT * sizeof(Value) - 1);
	Count &= Mask;
#pragma warning( push )
#pragma warning( disable : 4146)
	return (Value >> Count) | (Value << ((-Count) & Mask));
#pragma warning( pop ) 
}

BOOL RSHasher(IN PWCHAR String1, IN PWCHAR String2)
{
	int S1_Value = 0;
	int S2_Value = 0;

	for (int i = 0; i < wcslen(String1); i++)
	{
		S1_Value = String1[i] + RS_Sub(S1_Value, _SEED);
	}

	for (int i = 0; i < wcslen(String2); i++)
	{
		S2_Value = String2[i] + RS_Sub(S2_Value, _SEED);
	}

	if (S2_Value == S1_Value)
	{
		return TRUE;
	}


	return FALSE;
}

BOOL static Compare_Memory(IN PVOID Memory, IN PVOID Pattern, IN SIZE_T patternLength)
{
	SIZE_T rtSize = 0;

	rtSize = RtlCompareMemory(Memory, Pattern, patternLength);
	
	if (rtSize == patternLength) return TRUE;
		
	return FALSE;
}


PVOID Signature_Scanner(IN PVOID Memory, IN SIZE_T Length, IN PVOID PatternBuffer, IN SIZE_T PatternLength) 
{
	if ((!Memory || !Length) || (!PatternBuffer || !PatternLength)) 
	{
		return NULL;
	}

	for (SIZE_T Len = 0; Len < Length; Len++) 
	{
		
		if (Compare_Memory(C_PTR((U_PTR(Memory) + Len)), PatternBuffer, PatternLength))
		{
			return C_PTR((U_PTR(Memory) + Len));
		}
	}

	return NULL;
}


static ULONG64 Get_PageTable_Entry(IN ULONG64 virtualAddr, IN ULONG64 MiGetPteBaseAddr)
{
	ULONG64 pteBaseAddr = *(ULONG64*)(MiGetPteBaseAddr + 0x13);
	
	return pteBaseAddr + (virtualAddr >> 9 & 0x7ffffffff8);
}


VOID Fetch_Physical_Address(IN VIRTUAL_ADDRESS Linear, IN ULONG64 PTEbaseAddr, IN PVOID userModeBuffer ,OUT PULONG status)
{
	if (Linear.value == 0 || PTEbaseAddr == 0)
	{
		*status = STATUS_FAILED_PTE;
		return;
	}

	MM_COPY_ADDRESS pCopy = { 0 };
	SIZE_T rtBytes = 0;
	
	pCopy.PhysicalAddress.QuadPart = Get_PageTable_Entry(Linear.value, PTEbaseAddr);
	MmCopyMemory(userModeBuffer, pCopy, sizeof(PVOID), MM_COPY_MEMORY_PHYSICAL, &rtBytes);
	
}

