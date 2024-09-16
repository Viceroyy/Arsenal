#include "../SDK/SDK.h"

MAKE_SIGNATURE(CRendering3dView_EnableWorldFog, "client.dll", "55 8B EC 8B 0D ? ? ? ? 83 EC ? 8B 01 53 56 FF 90 ? ? ? ? 8B F0 85 F6 74 ? 8B 06 8B CE FF 50 ? 33 DB", 0x0);

MAKE_HOOK(CRendering3dView_EnableWorldFog, S::CRendering3dView_EnableWorldFog(), void, __fastcall, 
	void* ecx, void* edx)
{
	if (CFG::Visuals_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
	{
		CALL_ORIGINAL(ecx, edx);

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

	CALL_ORIGINAL(ecx, edx);
}
