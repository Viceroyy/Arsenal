#include "../SDK/SDK.h"

MAKE_SIGNATURE(GetClientInterpAmount, "client.dll", "55 8B EC A1 ? ? ? ? 83 EC ? A8", 0x0);

MAKE_HOOK(GetClientInterpAmount, S::GetClientInterpAmount(), float, __cdecl)
{
	return CFG::Visuals_NoInterpolation ? 0.f : CALL_ORIGINAL();
}