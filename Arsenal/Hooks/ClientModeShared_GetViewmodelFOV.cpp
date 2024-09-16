#include "../SDK/SDK.h"

MAKE_HOOK(ClientModeShared_GetViewmodelFOV, U::VFunc.Get<void*>(I::ClientMode, 32u), float, __fastcall)
{
	if (CFG::Visuals_ViewmodelFOV == 70 || CFG::Visuals_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
		return CALL_ORIGINAL();

	return CFG::Visuals_ViewmodelFOV;
}