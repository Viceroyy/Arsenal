#include "../SDK/SDK.h"

MAKE_HOOK(IBaseClientDLL_LevelShutdown, U::VFunc.Get<void*>(I::BaseClientDLL, 7u), void, __fastcall, void* ecx, void* edx)
{
	H::EntityCache.Clear();

	CALL_ORIGINAL(ecx, edx);
}