#include "Backtrack.h"

void CBacktrack::Restart()
{
	mRecords.clear();
	dSequences.clear();
	iLastInSequence = 0;
}

// Returns the current (custom) backtrack latency
float CBacktrack::GetFake()
{
	if (bFakeLatency)
	{
		if (CFG::Misc_Backtrack_LatencyMode == 1)
		{
			auto pNetChan = I::EngineClient->GetNetChannelInfo();
			if (!pNetChan)
				return 0.0f;

			return (0.2f - pNetChan->GetLatency(FLOW_OUTGOING) - 0.02f);
		}
		else if (CFG::Misc_Backtrack_LatencyMode == 2)
			return std::clamp(static_cast<float>(CFG::Misc_Backtrack_Latency), 0.f, flMaxUnlag * 1000.f) / 1000.f;
	}
	return 0.0f;
}

// Returns the current real latency
float CBacktrack::GetReal(int iFlow)
{
	auto pNetChan = I::EngineClient->GetNetChannelInfo();
	if (!pNetChan)
		return 0.f;

	if (iFlow != -1)
		return pNetChan->GetLatency(iFlow) - (iFlow == FLOW_INCOMING ? GetFake() : 0.f);
	return pNetChan->GetLatency(FLOW_INCOMING) - GetFake() + pNetChan->GetLatency(FLOW_OUTGOING);
}

// Store the last 2048 sequences
void CBacktrack::UpdateDatagram()
{
	auto pNetChan = static_cast<CNetChannel*>(I::EngineClient->GetNetChannelInfo());
	if (!pNetChan)
		return;

	if (pNetChan->m_nInSequenceNr > iLastInSequence)
	{
		iLastInSequence = pNetChan->m_nInSequenceNr;
		dSequences.push_front(CIncomingSequence(pNetChan->m_nInReliableState, pNetChan->m_nInSequenceNr, I::GlobalVars->realtime));
	}

	if (dSequences.size() > 2048)
		dSequences.pop_back();
}

bool CBacktrack::WithinRewind(const TickRecord& record)
{
	auto pNetChan = I::EngineClient->GetNetChannelInfo();
	if (!pNetChan)
		return false;

	const float flCorrect = std::clamp(pNetChan->GetLatency(FLOW_OUTGOING) + ROUND_TO_TICKS(flFakeInterp) + GetFake(), 0.f, flMaxUnlag) - pNetChan->GetLatency(FLOW_OUTGOING);
	const int iServerTick = iTickCount + 1;

	const float flDelta = flCorrect - TICKS_TO_TIME(iServerTick - TIME_TO_TICKS(record.flSimTime));

	return fabsf(flDelta) < 0.2f;
}

std::deque<TickRecord>* CBacktrack::GetRecords(C_BaseEntity* pEntity)
{
	if (mRecords[pEntity].empty())
		return nullptr;

	return &mRecords[pEntity];
}

std::deque<TickRecord> CBacktrack::GetValidRecords(std::deque<TickRecord>* pRecords, C_CSPlayer* pLocal, bool bDistance)
{
	std::deque<TickRecord> validRecords = {};
	if (!pRecords)
		return validRecords;

	for (auto& pTick : *pRecords)
	{
		if (!WithinRewind(pTick))
			continue;

		validRecords.push_back(pTick);
	}

	if (pLocal)
	{
		if (bDistance)
			std::sort(validRecords.begin(), validRecords.end(), [&](const TickRecord& a, const TickRecord& b) -> bool
				{
					if (CFG::Misc_Backtrack_PreferOnShot && a.bOnShot != b.bOnShot)
						return a.bOnShot > b.bOnShot;

					return pLocal->m_vecOrigin().DistTo(a.vOrigin) < pLocal->m_vecOrigin().DistTo(b.vOrigin);
				});
		else
		{
			auto pNetChan = I::EngineClient->GetNetChannelInfo();
			if (!pNetChan)
				return validRecords;

			const float flCorrect = std::clamp(pNetChan->GetLatency(FLOW_OUTGOING) + ROUND_TO_TICKS(flFakeInterp) + GetFake(), 0.f, flMaxUnlag) - pNetChan->GetLatency(FLOW_OUTGOING);
			const int iServerTick = iTickCount + 1;

			std::sort(validRecords.begin(), validRecords.end(), [&](const TickRecord& a, const TickRecord& b) -> bool
				{
					if (CFG::Misc_Backtrack_PreferOnShot && a.bOnShot != b.bOnShot)
						return a.bOnShot > b.bOnShot;

					const float flADelta = flCorrect - TICKS_TO_TIME(iServerTick - TIME_TO_TICKS(a.flSimTime));
					const float flBDelta = flCorrect - TICKS_TO_TIME(iServerTick - TIME_TO_TICKS(b.flSimTime));
					return fabsf(flADelta) < fabsf(flBDelta);
				});
		}
	}

	return validRecords;
}

void CBacktrack::StoreNolerp()
{
	for (auto& pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ENEMIES))
	{
		if (pEntity->entindex() == I::EngineClient->GetLocalPlayer())
			continue;

		// more of a placeholder, still interpolated iirc
		bSettingUpBones = true;
		mBones[pEntity].first = pEntity->SetupBones(mBones[pEntity].second, 128, BONE_USED_BY_ANYTHING, pEntity->m_flSimulationTime());
		bSettingUpBones = false;

		mEyeAngles[pEntity] = pEntity->As<C_CSPlayer>()->GetEyeAngles();
	}
}

void CBacktrack::MakeRecords()
{
	if (iLastCreationTick == I::GlobalVars->tickcount)
		return;
	iLastCreationTick = I::GlobalVars->tickcount;

	for (auto& pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ENEMIES))
	{
		if (pEntity->entindex() == I::EngineClient->GetLocalPlayer() || !mBones[pEntity].first)
			continue;

		const float flSimTime = pEntity->m_flSimulationTime(), flOldSimTime = pEntity->m_flOldSimulationTime();
		if (TIME_TO_TICKS(flSimTime - flOldSimTime) <= 0)
			continue;

		const TickRecord curRecord = {
			flSimTime,
			I::GlobalVars->curtime,
			I::GlobalVars->tickcount,
			mDidShoot[pEntity->entindex()],
			*reinterpret_cast<BoneMatrixes*>(&mBones[pEntity].second),
			pEntity->m_vecOrigin()
		};

		bool bLagComp = false;
		if (!mRecords[pEntity].empty()) // check for lagcomp breaking here
		{
			const Vector vDelta = curRecord.vOrigin - mRecords[pEntity].front().vOrigin;

			static auto sv_lagcompensation_teleport_dist = U::ConVars.FindVar("sv_lagcompensation_teleport_dist");
			const float flDist = powf(sv_lagcompensation_teleport_dist ? sv_lagcompensation_teleport_dist->GetFloat() : 64.f, 2.f);
			if (vDelta.Lenght2DSqr() > flDist)
			{
				bLagComp = true;
				for (auto& pRecord : mRecords[pEntity])
					pRecord.bInvalid = true;
			}

			for (auto& pRecord : mRecords[pEntity])
			{
				if (!pRecord.bInvalid)
					continue;

				pRecord.bOnShot = curRecord.bOnShot;
				pRecord.BoneMatrix = curRecord.BoneMatrix;
				pRecord.vOrigin = curRecord.vOrigin;
			}
		}

		mRecords[pEntity].push_front(curRecord);
		mLagCompensation[pEntity] = bLagComp;

		mDidShoot[pEntity->entindex()] = false;
	}
}

void CBacktrack::CleanRecords()
{
	for (auto& pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ENEMIES))
	{
		auto pPlayer = pEntity->As<C_CSPlayer>();
		if (pEntity->entindex() == I::EngineClient->GetLocalPlayer())
			continue;

		if (!pEntity->IsPlayer() || pEntity->IsDormant() || pPlayer->deadflag())
		{
			mRecords[pEntity].clear();
			continue;
		}

		const float flDeadtime = I::GlobalVars->curtime + GetReal() - flMaxUnlag;
		while (!mRecords[pEntity].empty())
		{
			if (mRecords[pEntity].back().flSimTime >= flDeadtime)
				break;

			mRecords[pEntity].pop_back();
		}
	}
}

void CBacktrack::FrameStageNotify()
{
	UpdateDatagram();
	if (!I::EngineClient->IsInGame())
		return Restart();

	static auto sv_maxunlag = U::ConVars.FindVar("sv_maxunlag");
	flMaxUnlag = sv_maxunlag ? sv_maxunlag->GetFloat() : 1.f;

	StoreNolerp();
	MakeRecords();
	CleanRecords();
}

// Adjusts the fake latency ping
void CBacktrack::AdjustPing(CNetChannel* netChannel)
{
	for (const auto& cSequence : dSequences)
	{
		if (I::GlobalVars->realtime - cSequence.CurTime >= GetFake())
		{
			netChannel->m_nInReliableState = cSequence.InReliableState;
			netChannel->m_nInSequenceNr = cSequence.SequenceNr;
			break;
		}
	}
}

std::optional<TickRecord> CBacktrack::GetHitRecord(CUserCmd* pCmd, C_CSPlayer* pEntity, const Vector vAngles, const Vector vPos)
{
	std::optional<TickRecord> cReturnRecord{};
	float flLastAngle = 45.f;

	for (const auto& rCurQuery : mRecords[pEntity])
	{
		if (!WithinRewind(rCurQuery)) { continue; }
		for (int iCurHitbox = 0; iCurHitbox < 18; iCurHitbox++)
		{
			//	it's possible to set entity positions and bones back to this record and then see what hitbox we will hit and rewind to that record, bt i dont wanna
			const Vector vHitboxPos = pEntity->GetHitboxPosMatrix(iCurHitbox, (matrix3x4_t*)(&rCurQuery.BoneMatrix.BoneMatrix));
			const Vector vAngleTo = U::Math.CalcAngle(vPos, vHitboxPos);
			const float flFOVTo = U::Math.CalcFov(vAngles, vAngleTo);
			if (flFOVTo < flLastAngle)
			{
				cReturnRecord = rCurQuery;
				flLastAngle = flFOVTo;
			}
		}
	}
	return cReturnRecord;
}

void CBacktrack::BacktrackToCrosshair(CUserCmd* pCmd)
{
	if (pCmd->buttons & IN_ATTACK)
	{
		C_CSPlayer* pLocal = H::EntityCache.GetLocal();
		if (!pLocal)
			return;

		const Vector vShootPos = pLocal->GetShootPos();
		const Vector vAngles = pCmd->viewangles;

		std::optional<TickRecord> cReturnTick;
		for (const auto& Player : H::EntityCache.GetGroup(EGroupType::PLAYERS_ENEMIES))
		{
			auto pPlayer = Player->As<C_CSPlayer>();
			if (pPlayer->entindex() == I::EngineClient->GetLocalPlayer())
				continue;

			if (!pPlayer->IsPlayer() || pPlayer->IsDormant() || pPlayer->deadflag())
				continue;

			if (const std::optional<TickRecord> checkRec = GetHitRecord(pCmd, pPlayer, vAngles, vShootPos))
			{
				cReturnTick = checkRec;
				break;
			}
		}
		if (cReturnTick)
		{
			pCmd->tick_count = TIME_TO_TICKS(cReturnTick->flSimTime) + TIME_TO_TICKS(F::Backtrack.flFakeInterp);
		}
	}
}
