#include "../SDK/SDK.h"

MAKE_SIGNATURE(C_BaseEntity_Interpolate, "client.dll", "55 8B EC 83 EC ? 53 57 8D 45", 0x0);

MAKE_HOOK(C_BaseEntity_Interpolate, S::C_BaseEntity_Interpolate(), bool, __fastcall,
	void* ecx, void* edx, float currentTime)
{
	return CFG::Visuals_NoInterpolation ? true : CALL_ORIGINAL(ecx, edx, currentTime);
}