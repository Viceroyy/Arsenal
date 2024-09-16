#include "../SDK/SDK.h"

#include "../Features/Visuals/Visuals.h"

MAKE_SIGNATURE(S_StartDynamicSound, "engine.dll", "55 8B EC 8B 0D ? ? ? ? 81 EC ? ? ? ? 85 C9", 0x0);

MAKE_HOOK(S_StartDynamicSound, S::S_StartDynamicSound(), int, __cdecl,
	StartSoundParams_t& params)
{
	F::Visuals.ManualNetwork(params);
	return CALL_ORIGINAL(params);
}