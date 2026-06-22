#include "../SDK/SDK.h"

MAKE_HOOK(ClientModeShared_DoPostScreenSpaceEffects, U::VFunc.Get<void*>(I::ClientMode, 39u), bool,
	void* rcx, const CViewSetup* pSetup)
{
	return CALL_ORIGINAL(rcx, pSetup);
}