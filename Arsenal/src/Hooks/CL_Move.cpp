#include "../SDK/SDK.h"
#include "../Features/Backtrack/Backtrack.h"
#include "../Features/NetworkFix/NetworkFix.h"

MAKE_SIGNATURE(CL_Move, "engine.dll", "40 55 53 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 83 3D", 0x0);

MAKE_HOOK(CL_Move, S::CL_Move(), void,
	float accumulated_extra_samples, bool bFinalTick)
{
	auto CallOriginal = [&](bool bFinal)
		{
			F::NetworkFix.FixInputDelay(bFinal);
			CALL_ORIGINAL(accumulated_extra_samples, bFinal);
		};

	if (G.Unload)
		return CALL_ORIGINAL(accumulated_extra_samples, bFinalTick);

	F::Backtrack.m_iTickCount = I::GlobalVars->tickcount + 1;
	if (!Util::IsLoopback())
		F::Backtrack.m_iTickCount--;

	CallOriginal(bFinalTick);
}