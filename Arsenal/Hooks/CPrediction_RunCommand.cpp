#include "../SDK/SDK.h"

MAKE_HOOK(CPrediction_RunCommand, U::VFunc.Get<void*>(I::ClientPrediction, 17u), void, __fastcall, void* ecx, void* edx, C_BasePlayer* player, CUserCmd* ucmd, IMoveHelper* moveHelper)
{
	CALL_ORIGINAL(ecx, edx, player, ucmd, moveHelper);

	if (!I::MoveHelper)
		I::MoveHelper = moveHelper;
}