#include "../SDK/SDK.h"

#include "../Features/Materials/Materials.h"
#include "../Features/Outlines/Outlines.h"
#include "../Features/WorldModulation/WorldModulation.h"

MAKE_HOOK(IBaseClientDLL_LevelShutdown, U::VFunc.Get<void*>(I::BaseClientDLL, 7u), void, __fastcall, 
	void* ecx, void* edx)
{
	CALL_ORIGINAL(ecx, edx);

	H::EntityCache.Clear();
	G.DormancyMap.clear();

	F::Materials.CleanUp();
	F::Outlines.CleanUp();
	F::WorldModulation.LevelShutdown();
}