#include "../SDK/SDK.h"

MAKE_HOOK(IBaseClientDLL_LevelInitPostEntity, U::VFunc.Get<void*>(I::BaseClientDLL, 6u), void, __fastcall, 
	void* ecx, void* edx)
{
	CALL_ORIGINAL(ecx, edx);
}