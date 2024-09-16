#include "../SDK/SDK.h"

MAKE_SIGNATURE(C_BaseEntity_ResetLatched, "client.dll", "53 8B D9 8B 03 8B 80 ? ? ? ? FF D0 84 C0 75 ? 57", 0x0);

MAKE_HOOK(C_BaseEntity_ResetLatched, S::C_BaseEntity_ResetLatched(), void, __fastcall, 
	void* ecx, void* edx)
{
	return;
}