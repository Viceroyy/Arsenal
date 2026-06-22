#include "../SDK/SDK.h"
/*
MAKE_HOOK(CInput_GetUserCmd, U::VFunc.Get<void*>(I::Input, 8u), CUserCmd*,
	void* rcx, int sequence_number)
{
	return &I::Input->m_pCommands[sequence_number % MULTIPLAYER_BACKUP];
}*/