#include "../SDK/SDK.h"
#include "../Features/Materials/Materials.h"

MAKE_SIGNATURE(CBaseClient_Connect, "engine.dll", "55 8B EC 53 56 57 68 ? ? ? ? 8B F1", 0x0);

MAKE_HOOK(CBaseClient_Connect, S::CBaseClient_Connect(), void, __fastcall,
	void* ecx, void* edx, const char* szName, int nUserID, INetChannel* pNetChannel, bool bFakePlayer, int clientChallenge)
{
	//F::Materials.ReloadMaterials();

	CALL_ORIGINAL(ecx, edx, szName, nUserID, pNetChannel, bFakePlayer, clientChallenge);
}