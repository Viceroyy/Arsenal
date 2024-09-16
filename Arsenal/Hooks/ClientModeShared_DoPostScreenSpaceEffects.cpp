#include "../SDK/SDK.h"

MAKE_HOOK(ClientModeShared_DoPostScreenSpaceEffects, U::VFunc.Get<void*>(I::ClientMode, 39u), bool, __fastcall, 
	void* ecx, void* edx, const CViewSetup* pSetup)
{
	return CALL_ORIGINAL(ecx, edx, pSetup);
}