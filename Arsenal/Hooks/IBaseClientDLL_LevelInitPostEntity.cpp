#include "../SDK/SDK.h"

MAKE_HOOK(IBaseClientDLL_LevelInitPostEntity, U::VFunc.Get<void*>(I::BaseClientDLL, 6u), void,
	void* rcx)
{
	CALL_ORIGINAL(rcx);
}