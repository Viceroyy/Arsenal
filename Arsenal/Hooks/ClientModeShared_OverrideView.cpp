#include "../SDK/SDK.h"

MAKE_HOOK(ClientModeShared_OverrideView, U::VFunc.Get<void*>(I::ClientMode, 16u), void, __fastcall, void* _this, void* _edx, CViewSetup* pSetup)
{
	CALL_ORIGINAL(_this, _edx, pSetup);
	if (CFG::Visual_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
		return;

	auto pLocal = H::EntityCache.GetLocal();
	if (pLocal && pSetup)
	{
		const auto curFOV = pSetup->fov;
		if (curFOV != 40.0f && curFOV != 10.0f && curFOV != 15.0f) // hack TODO: make this shit better
		{
			const int fov = CFG::Visual_FOV;
			if (!fov)
				return;

			pSetup->fov = fov;
		}
	}
}