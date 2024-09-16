#include "../SDK/SDK.h"
#include "../Features/Materials/Materials.h"

MAKE_SIGNATURE(CBaseClient_Disconnect, "engine.dll", "55 8B EC 81 EC ? ? ? ? 57 8B 7D ? 83 BF ? ? ? ? ? 0F 84 ? ? ? ? 56", 0x0);

MAKE_HOOK(CBaseClient_Disconnect, S::CBaseClient_Disconnect(), void, __fastcall,
	void* ecx, void* edx, const char* fmt, ...)
{
	//F::Materials.UnloadMaterials();

	CALL_ORIGINAL(ecx, edx, fmt);
}