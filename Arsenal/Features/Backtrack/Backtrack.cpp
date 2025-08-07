#include "Backtrack.h"

void CBacktrack::Reset()
{
	m_mRecords.clear();
	m_dSequences.clear();
	m_iLastInSequence = 0;
}

float CBacktrack::GetReal(int iFlow, bool bNoFake)
{
	auto pNetChan = I::EngineClient->GetNetChannelInfo();
	if (!pNetChan)
		return 0.f;

	if (iFlow != MAX_FLOWS)
		return pNetChan->GetLatency(iFlow) - (bNoFake && iFlow == FLOW_INCOMING ? GetFakeLatency() : 0.f);
	return pNetChan->GetLatency(FLOW_INCOMING) + pNetChan->GetLatency(FLOW_OUTGOING) - (bNoFake ? GetFakeLatency() : 0.f);
}

float CBacktrack::GetWishFake()
{
	return std::clamp(CFG::Misc_Backtrack_Latency / 1000.f, 0.f, m_flMaxUnlag);
}

float CBacktrack::GetFakeLatency()
{
	return m_flFakeLatency;
}

float CBacktrack::GetFakeInterp()
{
	if (CFG::Misc_AntiCheatCompatibility)
		return std::min(m_flFakeInterp, 0.1f);

	return m_flFakeInterp;
}

float CBacktrack::GetWindow()
{
	return 0.2f;
}

void CBacktrack::UpdateDatagram()
{
	auto pNetChan = reinterpret_cast<CNetChannel*>(I::EngineClient->GetNetChannelInfo());
	if (!pNetChan)
		return;

	if (auto pLocal = H::Entities.GetLocal())
		m_nOldTickBase = pLocal->m_nTickBase();

	if (pNetChan->m_nInSequenceNr > m_iLastInSequence)
	{
		m_iLastInSequence = pNetChan->m_nInSequenceNr;
		m_dSequences.emplace_front(pNetChan->m_nInReliableState, pNetChan->m_nInSequenceNr, I::GlobalVars->realtime);
	}

	if (m_dSequences.size() > 67)
		m_dSequences.pop_back();
}



bool CBacktrack::GetRecords(C_BaseEntity* pEntity, std::vector<TickRecord*>& vReturn)
{
	if (!m_mRecords.contains(pEntity))
		return false;

	auto& vRecords = m_mRecords[pEntity];
	for (auto& tRecord : vRecords)
		vReturn.push_back(&tRecord);
	return true;
}

std::vector<TickRecord*> CBacktrack::GetValidRecords(std::vector<TickRecord*>& vRecords, C_CSPlayer* pLocal, bool bDistance, float flTimeMod)
{
	if (vRecords.empty())
		return {};

	auto pNetChan = I::EngineClient->GetNetChannelInfo();
	if (!pNetChan)
		return {};

	std::vector<TickRecord*> vReturn = {};
	float flCorrect = std::clamp(GetReal(MAX_FLOWS, false) + ROUND_TO_TICKS(GetFakeInterp()), 0.f, m_flMaxUnlag);
	int iServerTick = m_iTickCount + TIME_TO_TICKS(GetReal(FLOW_OUTGOING));

	if (!CFG::Misc_AntiCheatCompatibility && GetWindow())
	{
		for (auto pRecord : vRecords)
		{
			float flDelta = fabsf(flCorrect - TICKS_TO_TIME(iServerTick - TIME_TO_TICKS(pRecord->m_flSimTime + flTimeMod)));
			if (flDelta > GetWindow())
				continue;

			vReturn.push_back(pRecord);
		}
	}

	if (vReturn.empty())
	{	// make sure there is at least 1 record
		float flMinDelta = 0.2f;
		for (auto pRecord : vRecords)
		{
			float flDelta = fabsf(flCorrect - TICKS_TO_TIME(iServerTick - TIME_TO_TICKS(pRecord->m_flSimTime + flTimeMod)));
			if (flDelta > flMinDelta)
				continue;

			flMinDelta = flDelta;
			vReturn = { pRecord };
		}
	}
	else if (pLocal && vReturn.size() > 1)
	{
		if (bDistance)
			std::sort(vReturn.begin(), vReturn.end(), [&](const TickRecord* a, const TickRecord* b) -> bool
				{
					if (CFG::Misc_Backtrack_PreferOnShot && a->m_bOnShot != b->m_bOnShot)
						return a->m_bOnShot > b->m_bOnShot;

					return pLocal->m_vecOrigin().DistTo(a->m_vOrigin) < pLocal->m_vecOrigin().DistTo(b->m_vOrigin);
				});
		else
		{
			std::sort(vReturn.begin(), vReturn.end(), [&](const TickRecord* a, const TickRecord* b) -> bool
				{
					if (CFG::Misc_Backtrack_PreferOnShot && a->m_bOnShot != b->m_bOnShot)
						return a->m_bOnShot > b->m_bOnShot;

					const float flADelta = flCorrect - TICKS_TO_TIME(iServerTick - TIME_TO_TICKS(a->m_flSimTime + flTimeMod));
					const float flBDelta = flCorrect - TICKS_TO_TIME(iServerTick - TIME_TO_TICKS(b->m_flSimTime + flTimeMod));
					return fabsf(flADelta) < fabsf(flBDelta);
				});
		}
	}

	return vReturn;
}

void CBacktrack::MakeRecords()
{
	for (auto& pEntity : H::Entities.GetGroup(EGroupType::PLAYERS_ALL))
	{
		auto pPlayer = pEntity->As<C_CSPlayer>();
		if (pPlayer->entindex() == I::EngineClient->GetLocalPlayer() || pPlayer->IsDormant() || !pPlayer->IsAlive()
			|| !H::Entities.GetDeltaTime(pPlayer->entindex()))
			continue;

		auto aBones = H::Entities.GetBones(pPlayer->entindex());
		if (!aBones)
			continue;

		auto& vRecords = m_mRecords[pPlayer];

		const TickRecord* pLastRecord = !vRecords.empty() ? &vRecords.front() : nullptr;
		vRecords.emplace_front(
			pPlayer->m_flSimulationTime(),
			pPlayer->m_vecOrigin(),
			pPlayer->m_vecMins(),
			pPlayer->m_vecMaxs(),
			*reinterpret_cast<BoneMatrix*>(aBones),
			m_mDidShoot[pPlayer->entindex()],
			pPlayer->m_vecOrigin()
		);
		const TickRecord& tCurRecord = vRecords.front();

		bool bLagComp = false;
		if (pLastRecord)
		{
			const Vector vDelta = tCurRecord.m_vBreak - pLastRecord->m_vBreak;

			static auto sv_lagcompensation_teleport_dist = U::ConVars.FindVar("sv_lagcompensation_teleport_dist");
			const float flDist = powf(sv_lagcompensation_teleport_dist->GetFloat(), 2.f);
			if (vDelta.Length2DSqr() > flDist)
			{
				bLagComp = true;
				if (!H::Entities.GetLagCompensation(pPlayer->entindex()))
				{
					vRecords.resize(1);
					vRecords.front().m_flSimTime = std::numeric_limits<float>::max(); // hack
				}
				std::for_each(vRecords.begin(), vRecords.end(), [](auto& tRecord) { tRecord.m_bInvalid = true; });
			}

			for (auto& tRecord : vRecords)
			{
				if (!tRecord.m_bInvalid)
					continue;

				tRecord.m_vOrigin = tCurRecord.m_vOrigin;
				tRecord.m_vMins = tCurRecord.m_vMins;
				tRecord.m_vMaxs = tCurRecord.m_vMaxs;
				tRecord.m_BoneMatrix = tCurRecord.m_BoneMatrix;
				tRecord.m_bOnShot = tCurRecord.m_bOnShot;
			}
		}

		H::Entities.SetLagCompensation(pPlayer->entindex(), bLagComp);
		m_mDidShoot[pPlayer->entindex()] = false;
	}
}

void CBacktrack::CleanRecords()
{
	for (auto& pEntity : H::Entities.GetGroup(EGroupType::PLAYERS_ALL))
	{
		auto pPlayer = pEntity->As<C_CSPlayer>();
		if (pPlayer->entindex() == I::EngineClient->GetLocalPlayer())
			continue;

		auto& vRecords = m_mRecords[pPlayer];

		if (pPlayer->IsDormant() || !pPlayer->IsAlive())
		{
			vRecords.clear();
			continue;
		}

		const int flDeadtime = I::GlobalVars->curtime + GetReal() - m_flMaxUnlag; // int ???
		if (vRecords.size() > 1 && vRecords.back().m_flSimTime == std::numeric_limits<float>::max())
			vRecords.pop_back();
		while (!vRecords.empty())
		{
			if (vRecords.back().m_flSimTime < flDeadtime || vRecords.size() > 1 && vRecords.back().m_flSimTime == std::numeric_limits<float>::max())
				vRecords.pop_back();
			else
				break;
		}
	}
}

void CBacktrack::Store()
{
	UpdateDatagram();
	if (!I::EngineClient->IsInGame())
		return;

	static auto sv_maxunlag = U::ConVars.FindVar("sv_maxunlag");
	m_flMaxUnlag = sv_maxunlag->GetFloat();

	MakeRecords();
	CleanRecords();
}

void CBacktrack::AdjustPing(CNetChannel* pNetChan)
{
	m_nOldInSequenceNr = pNetChan->m_nInSequenceNr, m_nOldInReliableState = pNetChan->m_nInReliableState;

	auto Set = [&]()
		{
			if (!CFG::Misc_Backtrack_Latency)
				return 0.f;

			auto pLocal = H::Entities.GetLocal();
			if (!pLocal)
				return 0.f;

			static auto host_timescale = U::ConVars.FindVar("host_timescale");
			float flTimescale = host_timescale->GetFloat();

			static float flStaticReal = 0.f;
			float flFake = GetWishFake(), flReal = TICKS_TO_TIME(pLocal->m_nTickBase() - m_nOldTickBase);
			flStaticReal += (flReal + 5 * TICK_INTERVAL - flStaticReal) * 0.1f;

			int nInReliableState = pNetChan->m_nInReliableState, nInSequenceNr = pNetChan->m_nInSequenceNr; float flLatency = 0.f;
			for (auto& cSequence : m_dSequences)
			{
				nInReliableState = cSequence.m_nInReliableState;
				nInSequenceNr = cSequence.m_nSequenceNr;
				flLatency = (I::GlobalVars->realtime - cSequence.m_flTime) * flTimescale - TICK_INTERVAL;

				if (flLatency > flFake || m_nLastInSequenceNr >= cSequence.m_nSequenceNr || flLatency > m_flMaxUnlag - flStaticReal)
					break;
			}
			if (flLatency > 1.f) // hacky failsafe
				return 0.f;

			pNetChan->m_nInReliableState = nInReliableState;
			pNetChan->m_nInSequenceNr = nInSequenceNr;
			return flLatency;
		};

	auto flLatency = Set();
	m_nLastInSequenceNr = pNetChan->m_nInSequenceNr;

	if (CFG::Misc_Backtrack_Latency || m_flFakeLatency)
	{
		m_flFakeLatency = std::clamp(m_flFakeLatency + (flLatency - m_flFakeLatency) * 0.1f, m_flFakeLatency - TICK_INTERVAL, m_flFakeLatency + TICK_INTERVAL);
		if (!flLatency && m_flFakeLatency < TICK_INTERVAL)
			m_flFakeLatency = 0.f;
	}
}

void CBacktrack::RestorePing(CNetChannel* pNetChan)
{
	pNetChan->m_nInSequenceNr = m_nOldInSequenceNr, pNetChan->m_nInReliableState = m_nOldInReliableState;
}

/*void CBacktrack::Draw(C_CSPlayer* pLocal)
{
	if (!(Vars::Menu::Indicators.Value & Vars::Menu::IndicatorsEnum::Ping) || !pLocal->IsAlive())
		return;

	auto pResource = H::Entities.GetPR();
	auto pNetChan = I::EngineClient->GetNetChannelInfo();
	if (!pResource || !pNetChan)
		return;

	static float flFakeLatency = 0.f;
	{
		static Timer tTimer = {};
		if (tTimer.Run(0.5f))
			flFakeLatency = GetFakeLatency();
	}
	float flFakeLerp = GetFakeInterp() > G::Lerp ? GetFakeInterp() : 0.f;

	float flFake = std::min(flFakeLatency + flFakeLerp, m_flMaxUnlag) * 1000;
	float flLatency = std::max(pNetChan->GetLatency(FLOW_INCOMING) + pNetChan->GetLatency(FLOW_OUTGOING) - flFakeLatency, 0.f) * 1000;
	int iLatencyScoreboard = pResource->m_iPing(pLocal->entindex());

	int x = Vars::Menu::PingDisplay.Value.x;
	int y = Vars::Menu::PingDisplay.Value.y + 8;
	const auto& fFont = H::Fonts.GetFont(FONT_INDICATORS);
	const int nTall = fFont.m_nTall + H::Draw.Scale(1);

	EAlign align = ALIGN_TOP;
	if (x <= 100 + H::Draw.Scale(50, Scale_Round))
	{
		x -= H::Draw.Scale(42, Scale_Round);
		align = ALIGN_TOPLEFT;
	}
	else if (x >= H::Draw.m_nScreenW - 100 - H::Draw.Scale(50, Scale_Round))
	{
		x += H::Draw.Scale(42, Scale_Round);
		align = ALIGN_TOPRIGHT;
	}

	if (flFake || Vars::Backtrack::Interp.Value > G::Lerp * 1000)
		H::Draw.StringOutlined(fFont, x, y, Vars::Menu::Theme::Active.Value, Vars::Menu::Theme::Background.Value, align, std::format("Ping {:.0f} (+ {:.0f}) ms", flLatency, flFake).c_str());
	else
		H::Draw.StringOutlined(fFont, x, y, Vars::Menu::Theme::Active.Value, Vars::Menu::Theme::Background.Value, align, std::format("Ping {:.0f} ms", flLatency).c_str());
	H::Draw.StringOutlined(fFont, x, y += nTall, Vars::Menu::Theme::Active.Value, Vars::Menu::Theme::Background.Value, align, std::format("Scoreboard {} ms", iLatencyScoreboard).c_str());
}*/

// Private helper function to find the best record and its FOV for a single entity.
std::optional<RecordWithFOV> CBacktrack::GetBestRecordWithFOV(
	C_CSPlayer* pEntity,
	const Vector& vAngles,
	const Vector& vPos) const noexcept
{
	auto pLocal = H::Entities.GetLocal();
	if (!pLocal || !pEntity) // Added a check for pEntity.
		return std::nullopt;

	std::vector<TickRecord*> rawRecords;
	if (!F::Backtrack.GetRecords(pEntity, rawRecords) || rawRecords.empty())
		return std::nullopt;

	const auto filteredRecords = F::Backtrack.GetValidRecords(rawRecords, pLocal);

	if (filteredRecords.empty())
		return std::nullopt;

	float flBestFOV = std::numeric_limits<float>::max();
	TickRecord* pBestRecord = nullptr;

	for (const auto pRecord : filteredRecords)
	{
		if (!pRecord)
			continue;

		const auto pBoneMatrix = &pRecord->m_BoneMatrix;

		float flRecordBestFOV = std::numeric_limits<float>::max();
		for (int iCurHitbox = 0; iCurHitbox < 18; ++iCurHitbox)
		{
			// Crucial safety check: The optional must have a value before dereferencing.
			if (const auto vHitboxPos = pEntity->GetHitboxPosMatrix(iCurHitbox, (matrix3x4_t*)pBoneMatrix))
			{
				const Vector vAngleTo = U::Math.CalcAngle(vPos, *vHitboxPos);
				const float flCurrentFOV = U::Math.CalcFov(vAngles, vAngleTo);
				flRecordBestFOV = std::min(flRecordBestFOV, flCurrentFOV);
			}
		}

		if (flRecordBestFOV < flBestFOV)
		{
			flBestFOV = flRecordBestFOV;
			pBestRecord = pRecord;
		}
	}

	if (pBestRecord)
	{
		return RecordWithFOV{ *pBestRecord, flBestFOV };
	}

	return std::nullopt;
}

// Main public function to get a hit record.
std::optional<TickRecord> CBacktrack::GetHitRecord(C_CSPlayer* pEntity, const Vector& vAngles, const Vector& vPos)
{
	if (auto bestRecordWithFov = GetBestRecordWithFOV(pEntity, vAngles, vPos))
	{
		return bestRecordWithFov->record;
	}

	return std::nullopt;
}

// Main public function to backtrack to the crosshair.
void CBacktrack::BacktrackToCrosshair(CUserCmd* pCmd)
{
	if (!pCmd || !(pCmd->buttons & IN_ATTACK)) // Added a check for pCmd.
		return;

	C_CSPlayer* pLocal = H::Entities.GetLocal();
	if (!pLocal)
		return;

	const Vector vShootPos = pLocal->GetShootPos();
	const Vector vAngles = pCmd->viewangles;

	std::optional<TickRecord> bestOverallRecord = std::nullopt;
	float bestOverallFOV = std::numeric_limits<float>::max();

	const auto enemies = H::Entities.GetGroup(EGroupType::PLAYERS_ENEMIES);
	for (auto pPlayer : enemies | std::views::filter([](const auto& p) noexcept {
		const auto pEnemy = p->As<C_CSPlayer>();
		return pEnemy && !pEnemy->IsDormant() && !pEnemy->deadflag();
		}))
	{
		const auto pEnemy = pPlayer->As<C_CSPlayer>();

		if (auto recordWithFOV = GetBestRecordWithFOV(pEnemy, vAngles, vShootPos))
		{
			if (recordWithFOV->fov < bestOverallFOV)
			{
				bestOverallFOV = recordWithFOV->fov;
				bestOverallRecord = recordWithFOV->record;
			}
		}
	}

	if (bestOverallRecord)
	{
		// Added a check for TIME_TO_TICKS to prevent invalid tick counts if TIME is somehow negative.
		const int nTick = TIME_TO_TICKS(bestOverallRecord->m_flSimTime);
		const int nInterp = TIME_TO_TICKS(F::Backtrack.GetFakeInterp());
		if (nTick >= 0 && nInterp >= 0)
		{
			pCmd->tick_count = nTick + nInterp;
		}
	}
}
