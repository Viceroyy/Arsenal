#include "Backtrack.h"

#include "../CFG.h"

void CLagRecords::EraseRecord(C_CSPlayer* pPlayer, int nRecord)
{
	auto& v = m_LagRecords[pPlayer];
	v.erase(v.begin() + nRecord);
}

void CLagRecords::EraseAllRecords(C_CSPlayer* pPlayer)
{
	if (m_LagRecords.find(pPlayer) != m_LagRecords.end())
		m_LagRecords.erase(pPlayer);
}

bool CLagRecords::IsSimulationTimeValid(float flCurSimTime, float flCmprSimTime)
{
	return flCurSimTime - flCmprSimTime < 0.2f;
}

void CLagRecords::AddRecord(C_CSPlayer* pPlayer)
{
	LagRecord_t Record = {};

	m_bSettingUpBones = true;

	const auto result = pPlayer->SetupBones(Record.m_BoneMatrix, 128, BONE_USED_BY_ANYTHING, I::GlobalVars->curtime);

	m_bSettingUpBones = false;

	if (!result)
		return;

	Record.m_pPlayer = pPlayer;
	Record.m_flSimulationTime = pPlayer->m_flSimulationTime();
	Record.m_vAbsOrigin = pPlayer->GetAbsOrigin();
	Record.m_vVecOrigin = pPlayer->m_vecOrigin();
	Record.m_vAbsAngles = pPlayer->GetAbsAngles();
	Record.m_vEyeAngles = pPlayer->GetEyeAngles();
	Record.m_vVelocity = pPlayer->m_vecVelocity();
	Record.m_vCenter = pPlayer->GetCenter();
	Record.m_nFlags = pPlayer->m_fFlags();

	//if (auto pAnimState = pPlayer->GetAnimState())
		//Record.m_flFeetYaw = pAnimState->m_flCurrentFeetYaw;

	m_LagRecords[pPlayer].emplace_front(Record);
}

const LagRecord_t* CLagRecords::GetRecord(C_CSPlayer* pPlayer, int nRecord, bool bSafe)
{
	if (!bSafe)
	{
		if (!m_LagRecords.contains(pPlayer))
			return nullptr;

		if (nRecord < 0 || nRecord > static_cast<int>(m_LagRecords[pPlayer].size() - 1))
			return nullptr;
	}

	return &m_LagRecords[pPlayer][nRecord];
}

bool CLagRecords::HasRecords(C_CSPlayer* pPlayer, int* pTotalRecords)
{
	if (m_LagRecords.contains(pPlayer))
	{
		const size_t nSize = m_LagRecords[pPlayer].size();

		if (nSize <= 0)
			return false;

		if (pTotalRecords)
			*pTotalRecords = static_cast<int>(nSize - 1);

		return true;
	}

	return false;
}

void CLagRecords::UpdateRecords()
{
	const auto pLocal = H::EntityCache.GetLocal();

	if (!pLocal || pLocal->deadflag())
	{
		if (!m_LagRecords.empty())
		{
			m_LagRecords.clear();
		}

		return;
	}

	for (const auto pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ENEMIES))
	{
		if (!pEntity || pEntity == pLocal)
		{
			continue;
		}

		const auto pPlayer = pEntity->As<C_CSPlayer>();

		if (pPlayer->deadflag())
		{
			m_LagRecords[pPlayer].clear();
		}
	}

	for (auto& records : m_LagRecords | std::views::values)
	{
		for (auto it = records.begin(); it != records.end(); )
		{
			const auto& curRecord = *it;
			if (!curRecord.m_pPlayer || !IsSimulationTimeValid(curRecord.m_pPlayer->m_flSimulationTime(), curRecord.m_flSimulationTime))
			{
				it = records.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

bool CLagRecords::DiffersFromCurrent(const LagRecord_t* pRecord)
{
	const auto pPlayer = pRecord->m_pPlayer;

	if (!pPlayer)
		return false;

	if (static_cast<int>((pPlayer->m_vecOrigin() - pRecord->m_vAbsOrigin).Lenght()) != 0)
		return true;

	if (static_cast<int>((pPlayer->GetEyeAngles() - pRecord->m_vEyeAngles).Lenght()) != 0)
		return true;

	if (pPlayer->m_fFlags() != pRecord->m_nFlags)
		return true;

	/*if (auto pAnimState = pPlayer->GetAnimState())
	{
		if (fabsf(pAnimState->m_flCurrentFeetYaw - pRecord->m_flFeetYaw) > 0.0f)
			return true;
	}*/

	return false;
}

void CLagRecordMatrixHelper::Set(const LagRecord_t* pRecord)
{
	if (!pRecord)
		return;

	const auto pPlayer = pRecord->m_pPlayer;

	if (!pPlayer || pPlayer->deadflag())
		return;

	const auto pCachedBoneData = pPlayer->GetCachedBoneData();

	if (!pCachedBoneData)
		return;

	m_pPlayer = pPlayer;
	m_vAbsOrigin = pPlayer->GetAbsOrigin();
	m_vAbsAngles = pPlayer->GetAbsAngles();
	memcpy(m_BoneMatrix, pCachedBoneData->Base(), sizeof(matrix3x4_t) * pCachedBoneData->Count());

	memcpy(pCachedBoneData->Base(), pRecord->m_BoneMatrix, sizeof(matrix3x4_t) * pCachedBoneData->Count());

	pPlayer->SetAbsOrigin(pRecord->m_vAbsOrigin);
	pPlayer->SetAbsAngles(pRecord->m_vAbsAngles);

	m_bSuccessfullyStored = true;
}

void CLagRecordMatrixHelper::Restore()
{
	if (!m_bSuccessfullyStored || !m_pPlayer)
		return;

	const auto pCachedBoneData = m_pPlayer->GetCachedBoneData();

	if (!pCachedBoneData)
		return;

	m_pPlayer->SetAbsOrigin(m_vAbsOrigin);
	m_pPlayer->SetAbsAngles(m_vAbsAngles);
	memcpy(pCachedBoneData->Base(), m_BoneMatrix, sizeof(matrix3x4_t) * pCachedBoneData->Count());

	m_pPlayer = nullptr;
	m_vAbsOrigin.Init();
	m_vAbsAngles.Init();
	std::memset(m_BoneMatrix, 0, sizeof(matrix3x4_t) * 128);
	m_bSuccessfullyStored = false;
}