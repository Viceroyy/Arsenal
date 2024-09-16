#include "../SDK/SDK.h"

MAKE_SIGNATURE(C_BaseAnimating_Interpolate, "client.dll", "55 8B EC 83 EC ? 56 8B F1 83 BE ? ? ? ? ? 0F 85", 0x0);

MAKE_HOOK(C_BaseAnimating_Interpolate, S::C_BaseAnimating_Interpolate(), bool, __fastcall, 
	void* ecx, void* edx, float currentTime)
{
	return CFG::Visuals_NoInterpolation ? true : CALL_ORIGINAL(ecx, edx, currentTime);
}