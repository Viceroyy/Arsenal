#include "../SDK/SDK.h"

#include "../Features/Materials/Materials.h"
#include "../Features/Outlines/Outlines.h"

auto drawing_world{ false };

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

MAKE_SIGNATURE(CBaseWorldView_DrawExecute, "client.dll", "55 8B EC 83 EC ? A1 ? ? ? ? 53 56 57 8B D9", 0x0);

MAKE_HOOK(CBaseWorldView_DrawExecute, S::CBaseWorldView_DrawExecute(), void, __fastcall, 
	void* ecx, void* edx, float waterHeight, view_id_t viewID, float waterZAdjust)
{
	drawing_world = true;
	CALL_ORIGINAL(ecx, edx, waterHeight, viewID, waterZAdjust);
	drawing_world = false;
}

MAKE_SIGNATURE(CParticleSystemMgr_DrawRenderCache, "client.dll", "55 8B EC 81 EC ? ? ? ? 53 8B D9 57 89 5D ? 8B BB", 0x0);

MAKE_HOOK(CParticleSystemMgr_DrawRenderCache, S::CParticleSystemMgr_DrawRenderCache(), void, __fastcall, 
	void* ecx, void* edx, bool bShadowDepth)
{
	if (drawing_world)
	{
		if (auto rc{ I::MaterialSystem->GetRenderContext() })
		{
			rc->ClearBuffers(false, false, true);
		}

		F::Materials.Run();
		F::Outlines.RunModels();
	}

	CALL_ORIGINAL(ecx, edx, bShadowDepth);
}