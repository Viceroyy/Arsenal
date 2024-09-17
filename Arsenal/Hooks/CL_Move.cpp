#include "../SDK/SDK.h"
#include "../Features/Backtrack/Backtrack.h"
#include "../Features/NetworkFix/NetworkFix.h"

MAKE_SIGNATURE(CL_Move, "engine.dll", "55 8B EC 83 EC ? 83 3D ? ? ? ? ? 0F 8C ? ? ? ? E8", 0x0);

MAKE_HOOK(CL_Move, S::CL_Move(), void, __cdecl,
	float accumulated_extra_samples, bool bFinalTick)
{
	auto CallOriginal = [&](bool bFinal)
	{
		F::NetworkFix.FixInputDelay(bFinal);
		CALL_ORIGINAL(accumulated_extra_samples, bFinal);
	};

	if (G.Unload)
		return CALL_ORIGINAL(accumulated_extra_samples, bFinalTick);

	F::Backtrack.iTickCount = I::GlobalVars->tickcount;

	CallOriginal(bFinalTick);
}