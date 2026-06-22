#include "../SDK/SDK.h"

MAKE_SIGNATURE(C_BaseEntity_Interpolate, "client.dll", "48 8B C4 48 89 70 ? F3 0F 11 48", 0x0);

MAKE_HOOK(C_BaseEntity_Interpolate, S::C_BaseEntity_Interpolate(), bool,
	void* rcx, float currentTime)
{
	return CFG::Visuals_NoInterpolation ? true : CALL_ORIGINAL(rcx, currentTime);
}