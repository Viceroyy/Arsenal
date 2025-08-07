#include "../SDK/SDK.h"

MAKE_SIGNATURE(C_CSRagdoll_CreateCSRagdoll, "client.dll", "48 89 4C 24 ? 56 41 56", 0x0);

MAKE_HOOK(C_CSRagdoll_CreateCSRagdoll, S::C_CSRagdoll_CreateCSRagdoll(), void,
	void* rcx)
{
	if (CFG::Visuals_NoRagdolls)
		return;

	CALL_ORIGINAL(rcx);
}