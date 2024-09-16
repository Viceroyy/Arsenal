#include "../SDK/SDK.h"

MAKE_SIGNATURE(DoEnginePostProcessing, "client.dll", "55 8B EC 81 EC ? ? ? ? 8B 0D ? ? ? ? 53 33 DB", 0x0);

MAKE_HOOK(DoEnginePostProcessing, S::DoEnginePostProcessing(), void, __cdecl,
	int x, int y, int w, int h, bool bFlashlightIsOn, bool bPostVGui)
{
	if (!CFG::Visuals_NoPostProcessing || CFG::Visuals_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
		CALL_ORIGINAL(x, y, w, h, bFlashlightIsOn, bPostVGui);
}