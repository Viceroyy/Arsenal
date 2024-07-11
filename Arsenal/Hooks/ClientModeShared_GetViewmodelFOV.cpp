#include "../SDK/SDK.h"

MAKE_HOOK(ClientModeShared_GetViewmodelFOV, U::VFunc.Get<void*>(I::ClientMode, 32u), float, __fastcall)
{
	if (CFG::Visual_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
		return CALL_ORIGINAL();

	return CFG::Visual_ViewmodelFOV;
}