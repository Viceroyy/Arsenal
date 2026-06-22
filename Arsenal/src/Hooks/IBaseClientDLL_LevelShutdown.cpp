#include "../SDK/SDK.h"

#include "../Features/Materials/Materials.h"
#include "../Features/WorldModulation/WorldModulation.h"

MAKE_HOOK(IBaseClientDLL_LevelShutdown, U::VFunc.Get<void*>(I::BaseClientDLL, 7u), void,
	void* rcx)
{
	CALL_ORIGINAL(rcx);

	H::Entities.Clear(true);
	//G.DormancyMap.clear();

	F::Materials.CleanUp();
	F::WorldModulation.LevelShutdown();
}