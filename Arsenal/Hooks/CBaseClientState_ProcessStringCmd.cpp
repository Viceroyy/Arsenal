#include "../SDK/SDK.h"

MAKE_SIGNATURE(CBaseClientState_ProcessStringCmd, "engine.dll", "55 8B EC 80 B9 ? ? ? ? ? 74 ? 83 3D", 0x0);

MAKE_HOOK(CBaseClientState_ProcessStringCmd, S::CBaseClientState_ProcessStringCmd(), bool, __stdcall,
	NET_StringCmd* msg)
{
	return true;
}