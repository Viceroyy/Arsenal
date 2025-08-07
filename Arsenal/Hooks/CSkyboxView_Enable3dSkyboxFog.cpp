#include "../SDK/SDK.h"

MAKE_SIGNATURE(CSkyboxView_Enable3dSkyboxFog, "client.dll", "40 57 48 83 EC ? E8 ? ? ? ? 48 8B F8 48 85 C0 0F 84 ? ? ? ? 48 8B 0D", 0x0);

MAKE_HOOK(CSkyboxView_Enable3dSkyboxFog, S::CSkyboxView_Enable3dSkyboxFog(), void,
	void* rcx)
{
	if (CFG::Visuals_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
	{
		CALL_ORIGINAL(rcx);

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

	CALL_ORIGINAL(rcx);
}
