//#include "Backtrack.h"
//
//void CFeatures_Backtrack::Restart()
//{
//	mRecords.clear();
//}
//
//// Returns the current real latency
//float CFeatures_Backtrack::GetReal(int iFlow)
//{
//	auto pNetChan = I::EngineClient->GetNetChannelInfo();
//	if (!pNetChan)
//		return 0.f;
//
//	if (iFlow != -1)
//		return pNetChan->GetLatency(iFlow);
//	return pNetChan->GetLatency(FLOW_INCOMING) + pNetChan->GetLatency(FLOW_OUTGOING);
//}
//
//bool CFeatures_Backtrack::WithinRewind(const TickRecord& record)
//{
//	auto pNetChan = I::EngineClient->GetNetChannelInfo();
//	if (!pNetChan)
//		return false;
//
//	const float flCorrect = U::Math.clip(pNetChan->GetLatency(FLOW_OUTGOING) + ROUND_TO_TICKS(flFakeInterp), 0.f, flMaxUnlag) - pNetChan->GetLatency(FLOW_OUTGOING);
//	const int iServerTick = iTickCount;
//
//	const float flDelta = flCorrect - TICKS_TO_TIME(iServerTick - TIME_TO_TICKS(record.flSimTime));
//
//	return fabsf(flDelta) < 200.0f / 1000.0f;
//}
//
//std::deque<TickRecord>* CFeatures_Backtrack::GetRecords(C_BaseEntity* pEntity)
//{
//	if (mRecords[pEntity].empty())
//		return nullptr;
//
//	return &mRecords[pEntity];
//}
//
//std::deque<TickRecord> CFeatures_Backtrack::GetValidRecords(std::deque<TickRecord>* pRecords, C_CSPlayer* pLocal, bool bDistance)
//{
//	std::deque<TickRecord> validRecords = {};
//	if (!pRecords)
//		return validRecords;
//
//	for (auto& pTick : *pRecords)
//	{
//		if (!WithinRewind(pTick))
//			continue;
//
//		validRecords.push_back(pTick);
//	}
//
//	if (pLocal)
//	{
//		if (bDistance)
//			std::sort(validRecords.begin(), validRecords.end(), [&](const TickRecord& a, const TickRecord& b) -> bool
//				{
//					//if (CFG::Backtrack::PreferOnShot.Value && a.bOnShot != b.bOnShot)
//						//return a.bOnShot > b.bOnShot;
//
//					return pLocal->m_vecOrigin().DistTo(a.vOrigin) < pLocal->m_vecOrigin().DistTo(b.vOrigin);
//				});
//		else
//		{
//			auto pNetChan = I::EngineClient->GetNetChannelInfo();
//			if (!pNetChan)
//				return validRecords;
//
//			const float flCorrect = U::Math.clip(pNetChan->GetLatency(FLOW_OUTGOING) + ROUND_TO_TICKS(flFakeInterp), 0.f, flMaxUnlag) - pNetChan->GetLatency(FLOW_OUTGOING);
//			const int iServerTick = iTickCount;
//
//			std::sort(validRecords.begin(), validRecords.end(), [&](const TickRecord& a, const TickRecord& b) -> bool
//				{
//					//if (CFG::Backtrack::PreferOnShot.Value && a.bOnShot != b.bOnShot)
//						//return a.bOnShot > b.bOnShot;
//
//					const float flADelta = flCorrect - TICKS_TO_TIME(iServerTick - TIME_TO_TICKS(a.flSimTime));
//					const float flBDelta = flCorrect - TICKS_TO_TIME(iServerTick - TIME_TO_TICKS(b.flSimTime));
//					return fabsf(flADelta) < fabsf(flBDelta);
//				});
//		}
//	}
//
//	return validRecords;
//}
//
//
//
//void CFeatures_Backtrack::StoreNolerp()
//{
//	for (auto& pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ALL))
//	{
//		if (pEntity->entindex() == I::EngineClient->GetLocalPlayer())
//			continue;
//
//		// more of a placeholder, still interpolated iirc
//		bSettingUpBones = true;
//		mBones[pEntity].first = pEntity->SetupBones(mBones[pEntity].second, 128, BONE_USED_BY_ANYTHING, pEntity->m_flSimulationTime());
//		bSettingUpBones = false;
//
//		mEyeAngles[pEntity] = pEntity->As<C_CSPlayer>()->EyeAngles();
//	}
//}
//
//void CFeatures_Backtrack::MakeRecords()
//{
//	if (iLastCreationTick == I::GlobalVars->tickcount)
//		return;
//	iLastCreationTick = I::GlobalVars->tickcount;
//
//	for (auto& pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ALL))
//	{
//		if (pEntity->entindex() == I::EngineClient->GetLocalPlayer() || !mBones[pEntity].first)
//			continue;
//
//		const float flSimTime = pEntity->m_flSimulationTime();//, flOldSimTime = pEntity->m_flOldSimulationTime();
//		/*if (TIME_TO_TICKS(flSimTime - flOldSimTime) <= 0)
//			continue;*/
//
//		const TickRecord curRecord = {
//			flSimTime,
//			I::GlobalVars->curtime,
//			I::GlobalVars->tickcount,
//			mDidShoot[pEntity->entindex()],
//			*reinterpret_cast<BoneMatrixes*>(&mBones[pEntity].second),
//			pEntity->m_vecOrigin()
//		};
//
//		bool bLagComp = false;
//		if (!mRecords[pEntity].empty()) // check for lagcomp breaking here
//		{
//			const Vector vDelta = curRecord.vOrigin - mRecords[pEntity].front().vOrigin;
//
//			static auto sv_lagcompensation_teleport_dist = I::Cvar->FindVar("sv_lagcompensation_teleport_dist");
//			const float flDist = powf(sv_lagcompensation_teleport_dist ? sv_lagcompensation_teleport_dist->GetFloat() : 64.f, 2.f);
//			if (vDelta.Lenght2DSqr() > flDist)
//			{
//				bLagComp = true;
//				for (auto& pRecord : mRecords[pEntity])
//					pRecord.bInvalid = true;
//			}
//
//			for (auto& pRecord : mRecords[pEntity])
//			{
//				if (!pRecord.bInvalid)
//					continue;
//
//				pRecord.bOnShot = curRecord.bOnShot;
//				pRecord.BoneMatrix = curRecord.BoneMatrix;
//				pRecord.vOrigin = curRecord.vOrigin;
//			}
//		}
//
//		mRecords[pEntity].push_front(curRecord);
//		mLagCompensation[pEntity] = bLagComp;
//
//		mDidShoot[pEntity->entindex()] = false;
//	}
//}
//
//void CFeatures_Backtrack::CleanRecords()
//{
//	for (auto& pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ALL))
//	{
//		auto pPlayer = pEntity->As<C_CSPlayer>();
//		if (pEntity->entindex() == I::EngineClient->GetLocalPlayer())
//			continue;
//
//		if (!pEntity->IsPlayer() || pEntity->IsDormant() || !pPlayer->IsAlive())
//		{
//			mRecords[pEntity].clear();
//			continue;
//		}
//
//		//const int iOldSize = pRecords.size();
//
//		const int flDeadtime = I::GlobalVars->curtime + GetReal() - flMaxUnlag; // int ???
//		while (!mRecords[pEntity].empty())
//		{
//			if (mRecords[pEntity].back().flSimTime >= flDeadtime)
//				break;
//
//			mRecords[pEntity].pop_back();
//		}
//
//		//const int iNewSize = pRecords.size();
//		//if (iOldSize != iNewSize)
//		//	SDK::Output("Clear", std::format("{} -> {}", iOldSize, iNewSize).c_str(), { 255, 0, 200, 255 }, CFG::Debug::Logging.Value);
//	}
//}
//
//
//
//void CFeatures_Backtrack::FrameStageNotify()
//{
//	if (!I::EngineClient->IsInGame())
//		return Restart();
//
//	static auto sv_maxunlag = I::Cvar->FindVar("sv_maxunlag");
//	flMaxUnlag = sv_maxunlag ? sv_maxunlag->GetFloat() : 1.f;
//
//	StoreNolerp();
//	MakeRecords();
//	CleanRecords();
//}