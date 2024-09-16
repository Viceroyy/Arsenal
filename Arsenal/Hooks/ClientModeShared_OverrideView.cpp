#include "../SDK/SDK.h"

#include "../Features/Visuals/Visuals.h"

MAKE_HOOK(ClientModeShared_OverrideView, U::VFunc.Get<void*>(I::ClientMode, 16u), void, __fastcall, 
	void* ecx, void* edx, CViewSetup* pSetup)
{
	//CALL_ORIGINAL(ecx, edx, pSetup);
	if (!pSetup || CFG::Visuals_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
		return;

	G.FOV = pSetup->fov;

	F::Visuals.DoFOV(pSetup);
	F::Visuals.DoThirdperson(pSetup);
}