#include "../Hooks.h"

DEFINE_HOOK(ClientModeShared_GetViewmodelFOV, float, __fastcall)
{
	if (CFG::Visual_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
		return Func.Original<FN>()();

	return CFG::Visual_ViewmodelFOV;
}