#include "../SDK/SDK.h"

MAKE_SIGNATURE(C_BaseAnimating_Interpolate, "client.dll", "48 8B C4 48 89 70 ? F3 0F 11 48", 0x0);

MAKE_HOOK(C_BaseAnimating_Interpolate, S::C_BaseAnimating_Interpolate(), bool,
	void* rcx, float currentTime)
{
	if (CFG::Visuals_NoInterpolation && rcx != H::Entities.GetLocal())
		return true;

	return CALL_ORIGINAL(rcx, currentTime);
}