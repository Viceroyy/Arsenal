#include "../SDK/SDK.h"
#include "../Features/Backtrack/Backtrack.h"
#include "../Features/NetworkFix/NetworkFix.h"

MAKE_SIGNATURE(CL_CheckForPureServerWhitelist, "engine.dll", "55 8B EC 83 3D ? ? ? ? ? 7E ? 80 3D", 0x0);

MAKE_HOOK(CL_CheckForPureServerWhitelist, S::CL_CheckForPureServerWhitelist(), void, __cdecl, 
	void** pFilesToReload)
{
	return;
}