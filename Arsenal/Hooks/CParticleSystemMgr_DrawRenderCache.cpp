#include "../SDK/SDK.h"

#include "../Features/Materials/Materials.h"

bool isDrawingWorld = false;

enum view_id_t
{
	VIEW_ILLEGAL = -2,
	VIEW_NONE = -1,
	VIEW_MAIN = 0,
	VIEW_3DSKY = 1,
	VIEW_MONITOR = 2,
	VIEW_REFLECTION = 3,
	VIEW_REFRACTION = 4,
	VIEW_INTRO_PLAYER = 5,
	VIEW_INTRO_CAMERA = 6,
	VIEW_SHADOW_DEPTH_TEXTURE = 7,
	VIEW_SSAO = 8,
	VIEW_ID_COUNT
};

MAKE_SIGNATURE(CBaseWorldView_DrawExecute, "client.dll", "48 8B C4 53 55 56 41 56 41 57", 0x0);

MAKE_HOOK(CBaseWorldView_DrawExecute, S::CBaseWorldView_DrawExecute(), void,
	void* rcx, float waterHeight, view_id_t viewID, float waterZAdjust)
{
	isDrawingWorld = true;
	CALL_ORIGINAL(rcx, waterHeight, viewID, waterZAdjust);
	isDrawingWorld = false;
}

MAKE_SIGNATURE(CParticleSystemMgr_DrawRenderCache, "client.dll", "48 8B C4 88 50 ? 48 89 48 ? 55 57", 0x0);

MAKE_HOOK(CParticleSystemMgr_DrawRenderCache, S::CParticleSystemMgr_DrawRenderCache(), void,
	void* rcx, bool bShadowDepth)
{
	if (isDrawingWorld)
	{
		if (const auto rc = I::MaterialSystem->GetRenderContext())
		{
			rc->ClearBuffers(false, false, true);
		}

		F::Materials.Run();
	}

	CALL_ORIGINAL(rcx, bShadowDepth);
}