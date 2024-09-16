#include "EnginePrediction.h"

void CFeatures_EnginePrediction::Start(C_CSPlayer* pLocal, CUserCmd* cmd)
{
	if (!pLocal || pLocal->deadflag() || !I::MoveHelper)
		return;

	memset(&m_MoveData, 0, sizeof(CMoveData));

	cmd->random_seed = MD5_PseudoRandom(cmd->command_number) & std::numeric_limits<int>::max();

	pLocal->m_pCurrentCommand() = cmd;
	C_BaseEntity::SetPredictionRandomSeed(cmd);
	C_BaseEntity::SetPredictionPlayer(pLocal);

	m_fOldCurrentTime = I::GlobalVars->curtime;
	m_fOldFrameTime = I::GlobalVars->frametime;
	m_nOldTickCount = I::GlobalVars->tickcount;

	const int nOldTickBase = pLocal->m_nTickBase();
	const bool bOldIsFirstPrediction = I::ClientPrediction->m_bFirstTimePredicted;
	const bool bOldInPrediction = I::ClientPrediction->m_bInPrediction;

	const int nServerTicks = GetTickbase(pLocal, cmd);

	I::GlobalVars->curtime = TICKS_TO_TIME(nServerTicks);
	I::GlobalVars->frametime = (I::ClientPrediction->m_bEnginePaused ? 0.0f : TICK_INTERVAL);
	I::GlobalVars->tickcount = nServerTicks;

	I::ClientPrediction->m_bFirstTimePredicted = false;
	I::ClientPrediction->m_bInPrediction = true;

	I::GameMovement->StartTrackPredictionErrors(pLocal);

	if (cmd->weaponselect != 0)
	{
		C_BaseCombatWeapon* pWeapon = dynamic_cast<C_BaseCombatWeapon*>(Util::EntityByIndex(cmd->weaponselect));

		if (pWeapon)
			pLocal->SelectItem(pWeapon->GetName(), cmd->weaponsubtype);
	}

	if (cmd->impulse)
	{
		if (!pLocal->GetClientVehicle() || pLocal->UsingStandardWeaponsInVehicle())
			pLocal->m_nImpulse() = cmd->impulse;
	}

	pLocal->UpdateButtonState(cmd->buttons);

	I::ClientPrediction->SetLocalViewAngles(cmd->viewangles);

	if (pLocal->PhysicsRunThink())
		pLocal->PreThink();

	const int thinktick = pLocal->m_nNextThinkTick();

	if (thinktick > 0 && thinktick < nServerTicks)
	{
		pLocal->m_nNextThinkTick() = TICK_NEVER_THINK;
		pLocal->Think();
	}

	I::ClientPrediction->SetupMove(pLocal, cmd, I::MoveHelper, &m_MoveData);
	I::GameMovement->ProcessMovement(pLocal, &m_MoveData);
	I::ClientPrediction->FinishMove(pLocal, cmd, &m_MoveData);

	pLocal->m_nTickBase() = nOldTickBase;

	I::ClientPrediction->m_bInPrediction = bOldInPrediction;
	I::ClientPrediction->m_bFirstTimePredicted = bOldIsFirstPrediction;
}

void CFeatures_EnginePrediction::Finish(C_CSPlayer* pLocal, CUserCmd* cmd)
{
	if (!pLocal || pLocal->deadflag())
		return;

	I::GameMovement->FinishTrackPredictionErrors(pLocal);

	pLocal->m_pCurrentCommand() = NULL;
	C_BaseEntity::SetPredictionRandomSeed(NULL);
	C_BaseEntity::SetPredictionPlayer(NULL);

	I::GlobalVars->curtime = m_fOldCurrentTime;
	I::GlobalVars->frametime = m_fOldFrameTime;
	I::GlobalVars->tickcount = m_nOldTickCount;
}

int CFeatures_EnginePrediction::GetTickbase(C_CSPlayer* pLocal, CUserCmd* cmd)
{
	static int s_nTick = 0;
	static CUserCmd* s_pLastCmd = nullptr;

	if (cmd)
	{
		if (!s_pLastCmd || s_pLastCmd->hasbeenpredicted)
			s_nTick = pLocal->m_nTickBase();
		else
			s_nTick++;

		s_pLastCmd = cmd;
	}

	return s_nTick;
}
