#include "../SDK/SDK.h"

#include "../Features/Backtrack/Backtrack.h"
#include "../Features/Materials/Materials.h"

MAKE_SIGNATURE(CViewRender_LevelInit, "client.dll", "56 8B F1 8B 0D ? ? ? ? 8B 01 FF 50 ? 8B 0D", 0x0);

MAKE_HOOK(CViewRender_LevelInit, S::CViewRender_LevelInit(), void, __fastcall,
	void* ecx, void* edx)
{
	//F::Materials.ReloadMaterials();
	F::Backtrack.Restart();

	CALL_ORIGINAL(ecx, edx);
}
