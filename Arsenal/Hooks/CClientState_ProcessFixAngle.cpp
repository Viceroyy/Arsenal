#include "../SDK/SDK.h"
#include "../Features/Backtrack/Backtrack.h"
#include "../Features/NetworkFix/NetworkFix.h"

MAKE_SIGNATURE(CClientState_ProcessFixAngle, "engine.dll", "55 8B EC 8B 45 ? 83 EC ? F3 0F 10 15", 0x0);

MAKE_HOOK(CClientState_ProcessFixAngle, S::CClientState_ProcessFixAngle(), bool, __fastcall, 
	void* ecx, void* edx, SVC_FixAngle* msg)
{
	return CFG::Visuals_NoForcedAngles ? false : CALL_ORIGINAL(ecx, edx, msg);
}