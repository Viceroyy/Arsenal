#include "../SDK/SDK.h"

MAKE_SIGNATURE(S_StartSound, "engine.dll", "40 53 48 83 EC ? 48 83 79 ? ? 48 8B D9 75 ? 33 C0", 0x0);

MAKE_HOOK(S_StartSound, S::S_StartSound(), int,
	StartSoundParams_t& params)
{
	if (!params.staticsound)
		H::Entities.ManualNetwork(params);

	return CALL_ORIGINAL(params);
}