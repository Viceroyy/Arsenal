#include "../SDK/SDK.h"

MAKE_SIGNATURE(CSkyboxView_Enable3dSkyboxFog, "client.dll", "55 8B EC 83 EC ? 57 E8 ? ? ? ? 8B F8 85 FF 74", 0x0);

MAKE_HOOK(CSkyboxView_Enable3dSkyboxFog, S::CSkyboxView_Enable3dSkyboxFog(), void, __fastcall,
	void* ecx, void* edx)
{
	if (CFG::Visuals_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
	{
		CALL_ORIGINAL(ecx, edx);

		return;
	}

	if (CFG::Visuals_Remove_Sky_Fog)
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
