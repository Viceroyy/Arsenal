#include "NetworkFix.h"

MAKE_SIGNATURE(CL_ReadPackets, "engine.dll", "55 8B EC 83 EC ? 8B 0D ? ? ? ? 53 33 DB 89 5D ? 89 5D ? 8B 41 ? 89 5D ? 89 5D ? 85 C0 74 ? 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 53 53 53 53 8D 4D ? 51 50 8B 40 ? FF D0 8B 45 ? 83 C4 ? 8B 0D ? ? ? ? 89 45 ? 8B 45 ? 89 45 ? 8B 59 ? 8B 0D ? ? ? ? 89 5D ? FF 15 ? ? ? ? 88 45 ? 84 C0 74 ? 8B 0D ? ? ? ? 6A ? 6A ? 68 ? ? ? ? 6A ? 68 ? ? ? ? FF 15 ? ? ? ? 8B 0D", 0x0);

MAKE_HOOK(CL_ReadPackets, S::CL_ReadPackets(), void, __cdecl, 
	bool bFinalTick)
{
	if (F::NetworkFix.ShouldReadPackets())
	{
		CALL_ORIGINAL(bFinalTick);
	}
}

void CReadPacketState::Store()
{
	m_flFrameTimeClientState = I::ClientState->m_frameTime;
	m_flFrameTime = I::GlobalVars->frametime;
	m_flCurTime = I::GlobalVars->curtime;
	m_nTickCount = I::GlobalVars->tickcount;
}

void CReadPacketState::Restore()
{
	I::ClientState->m_frameTime = m_flFrameTimeClientState;
	I::GlobalVars->frametime = m_flFrameTime;
	I::GlobalVars->curtime = m_flCurTime;
	I::GlobalVars->tickcount = m_nTickCount;
}

void CNetworkFix::FixInputDelay(bool bFinalTick)
{
	if (!I::EngineClient->IsInGame())
	{
		return;
	}

	if (auto pNetChannel = I::EngineClient->GetNetChannelInfo())
	{
		if (pNetChannel->IsLoopback())
		{
			return;
		}
	}

	CReadPacketState Backup = {};

	Backup.Store();

	Hooks::CL_ReadPackets::Hook.Original<Hooks::CL_ReadPackets::FN>()(bFinalTick);

	m_State.Store();

	Backup.Restore();
}

bool CNetworkFix::ShouldReadPackets()
{
	if (!I::EngineClient->IsInGame())
		return true;

	if (auto pNetChannel = I::EngineClient->GetNetChannelInfo())
	{
		if (pNetChannel->IsLoopback())
			return true;
	}

	m_State.Restore();

	return false;
}