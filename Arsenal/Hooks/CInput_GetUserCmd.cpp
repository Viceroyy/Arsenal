#include "../SDK/SDK.h"

MAKE_HOOK(CInput_GetUserCmd, U::VFunc.Get<void*>(I::Input, 8u), CUserCmd*, __fastcall, 
	void* ecx, void* edx, int sequence_number)
{
	return &I::Input->m_pCommands[sequence_number % MULTIPLAYER_BACKUP];
}