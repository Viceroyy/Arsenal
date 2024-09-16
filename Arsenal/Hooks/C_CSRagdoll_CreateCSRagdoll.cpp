#include "../SDK/SDK.h"

MAKE_SIGNATURE(C_CSRagdoll_CreateCSRagdoll, "client.dll", "55 8B EC B8 ? ? ? ? E8 ? ? ? ? 53 56 57 8B F9 8B 97", 0x0);

MAKE_HOOK(C_CSRagdoll_CreateCSRagdoll, S::C_CSRagdoll_CreateCSRagdoll(), void, __fastcall,
	void* ecx, void* edx)
{
	if (CFG::Visuals_NoRagdolls)
		return;

	CALL_ORIGINAL(ecx, edx);
}