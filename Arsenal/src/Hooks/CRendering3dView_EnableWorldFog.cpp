#include "../SDK/SDK.h"

MAKE_SIGNATURE(CRendering3dView_EnableWorldFog, "client.dll", "40 53 48 83 EC ? 48 8B 0D ? ? ? ? 48 89 74 24", 0x0);

MAKE_HOOK(CRendering3dView_EnableWorldFog, S::CRendering3dView_EnableWorldFog(), void,
	void* rcx)
{
	if (CFG::Visuals_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
	{
		CALL_ORIGINAL(rcx);

		return;
	}

	if (CFG::Visuals_Remove_Fog)
	{
		if (auto pRenderContext = I::MaterialSystem->GetRenderContext())
		{
			const float clr[]{ 0.0f, 0.0f, 0.0f };

			pRenderContext->FogMode(MATERIAL_FOG_LINEAR);
			pRenderContext->FogColor3fv(clr);
			pRenderContext->FogStart(0.0f);
			pRenderContext->FogEnd(0.0f);
			pRenderContext->FogMaxDensity(0.0f);

			return;
		}
	}

	CALL_ORIGINAL(rcx);
}
