#include "../SDK/SDK.h"

MAKE_SIGNATURE(CClientState_GetClientInterpAmount, "engine.dll", "55 8B EC 8B 0D ? ? ? ? 83 EC ? 85 C9 75 ? 8B 0D", 0x0);

MAKE_HOOK(CClientState_GetClientInterpAmount, S::CClientState_GetClientInterpAmount(), float, __fastcall, 
	void* ecx, void* edx)
{
	const float retVal = CALL_ORIGINAL(ecx, edx);
	G.Lerp = retVal;
	return CFG::Visuals_NoInterpolation ? 0.f : retVal;
}