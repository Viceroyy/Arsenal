#include "../SDK/SDK.h"
#include <intrin.h>

MAKE_SIGNATURE(CClientState_GetTime, "engine.dll", "55 8B EC 51 80 B9 ? ? ? ? ? F3 0F 2A 81", 0x0);
MAKE_SIGNATURE(CL_FireEvents_CClientState_GetTime_Call, "engine.dll", "E8 ? ? ? ? D9 43 ? DF F1", 0x5);

MAKE_HOOK(CClientState_GetTime, S::CClientState_GetTime(), float, __fastcall, 
	void* ecx, void* edx)
{
	static auto dwDesired = S::CL_FireEvents_CClientState_GetTime_Call();
	const auto dwRetAddr = std::uintptr_t(_ReturnAddress());

	if (dwRetAddr == dwDesired)
		return std::numeric_limits<float>::max();

	return CALL_ORIGINAL(ecx, edx);
}
