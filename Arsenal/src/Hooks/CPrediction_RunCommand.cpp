#include "../SDK/SDK.h"

MAKE_HOOK(CPrediction_RunCommand, U::VFunc.Get<void*>(I::ClientPrediction, 17u), void,
	void* rcx, C_BasePlayer* player, CUserCmd* ucmd, IMoveHelper* moveHelper)
{
	CALL_ORIGINAL(rcx, player, ucmd, moveHelper);

	if (!I::MoveHelper)
		I::MoveHelper = moveHelper;
}