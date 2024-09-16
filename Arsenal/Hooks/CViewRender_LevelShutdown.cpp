#include "../SDK/SDK.h"

#include "../Features/Backtrack/Backtrack.h"
#include "../Features/Materials/Materials.h"

MAKE_SIGNATURE(CViewRender_LevelShutdown, "client.dll", "8B 0D ? ? ? ? 8B 01 FF 60 ? CC CC CC CC CC B8 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC CC B8", 0x0);

MAKE_HOOK(CViewRender_LevelShutdown, S::CViewRender_LevelShutdown(), void, __fastcall,
	void* ecx, void* edx)
{
	//F::Materials.UnloadMaterials();

	CALL_ORIGINAL(ecx, edx);
}
