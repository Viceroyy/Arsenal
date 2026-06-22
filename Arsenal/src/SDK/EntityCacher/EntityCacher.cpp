#include "EntityCacher.h"
#include "../../Utils/Timer/Timer.h"
#include "../../Features/CFG.h"
#include "../../Features/Backtrack/Backtrack.h"

void CEntities::Store()
{
	auto pLocal = I::ClientEntityList->GetClientEntity(I::EngineClient->GetLocalPlayer());
	if (!pLocal)
		return;

	m_pLocal = pLocal->As<C_CSPlayer>();
	m_pLocalWeapon = m_pLocal->m_hActiveWeapon()->As<C_WeaponCSBase>();

	for (int n = I::EngineClient->GetMaxClients() + 1; n <= I::ClientEntityList->GetHighestEntityIndex(); n++)
	{
		auto pEntity = I::ClientEntityList->GetClientEntity(n)->As<C_BaseEntity>();
		if (!pEntity || pEntity->IsDormant())
			continue;

		auto nClassID = pEntity->GetClassID();
		switch (nClassID)
		{
		case ECSClassID::CCSPlayerResource:
		{
			m_pPlayerResource = pEntity->As<C_CSPlayerResource>();
			break;
		}
		case ECSClassID::CPlantedC4:
		{
			m_mGroups[EGroupType::WORLD_C4PLANTED].push_back(pEntity);
			break;
		}
		case ECSClassID::CAK47:
		case ECSClassID::CWeaponP228:
		case ECSClassID::CWeaponGlock:
		case ECSClassID::CWeaponScout:
		case ECSClassID::CHEGrenade:
		case ECSClassID::CWeaponXM1014:
		case ECSClassID::CC4:
		case ECSClassID::CWeaponMAC10:
		case ECSClassID::CWeaponAug:
		case ECSClassID::CSmokeGrenade:
		case ECSClassID::CWeaponElite:
		case ECSClassID::CWeaponFiveSeven:
		case ECSClassID::CWeaponUMP45:
		case ECSClassID::CWeaponSG550:
		case ECSClassID::CWeaponGalil:
		case ECSClassID::CWeaponFamas:
		case ECSClassID::CWeaponUSP:
		case ECSClassID::CWeaponAWP:
		case ECSClassID::CWeaponMP5Navy:
		case ECSClassID::CWeaponM249:
		case ECSClassID::CWeaponM3:
		case ECSClassID::CWeaponM4A1:
		case ECSClassID::CWeaponTMP:
		case ECSClassID::CWeaponG3SG1:
		case ECSClassID::CFlashbang:
		case ECSClassID::CDEagle:
		case ECSClassID::CWeaponSG552:
		case ECSClassID::CWeaponP90:
		{
			if (pEntity->As<C_BaseCombatWeapon>()->m_hOwner().Get() == nullptr)
				m_mGroups[EGroupType::WORLD_WEAPONS].push_back(pEntity);
			break;
		}
		}
	}

	static Timer tTimer = {};
	bool bUpdateInfo = tTimer.Run(1.f);
	int iLag;
	{
		static int iStaticTickcout = I::GlobalVars->tickcount;
		iLag = I::GlobalVars->tickcount - iStaticTickcout - 1;
		iStaticTickcout = I::GlobalVars->tickcount;
	}
	std::unordered_map<uint32_t, uint64_t> mParties = {};
	std::unordered_map<uint32_t, bool> mF2P = {};
	std::unordered_map<uint32_t, int> mLevels = {};

	for (int n = 1; n <= I::EngineClient->GetMaxClients(); n++)
	{
		auto pPlayer = I::ClientEntityList->GetClientEntity(n)->As<C_CSPlayer>();
		if (!pPlayer || !pPlayer->IsPlayer())
			continue;

		auto pResource = GetPR();
		if (pResource && pResource->m_bValid(n))
		{
			if (pPlayer->IsDormant())
			{
				pPlayer->m_lifeState() = pResource->m_bAlive(n) ? LIFE_ALIVE : LIFE_DEAD;
				pPlayer->m_iHealth() = pResource->m_iHealth(n);
				if (m_mDormancy.contains(n))
				{
					auto& tDormancy = m_mDormancy[n];
					if (I::EngineClient->Time() - tDormancy.LastUpdate < 5.f)
						pPlayer->SetAbsOrigin(pPlayer->m_vecOrigin() = tDormancy.Location);
					else
						m_mDormancy.erase(n);
				}
			}
			else if (pResource->m_bAlive(n))
				m_mDormancy[n] = { pPlayer->m_vecOrigin(), I::EngineClient->Time() };
		}

		m_mModels[n] = FNV1A::Hash32(I::ModelInfoClient->GetModelName(pPlayer->GetModel()));
		m_mGroups[EGroupType::PLAYERS_ALL].push_back(pPlayer);
		m_mGroups[pPlayer->m_iTeamNum() != m_pLocal->m_iTeamNum() ? EGroupType::PLAYERS_ENEMIES : EGroupType::PLAYERS_TEAMMATES].push_back(pPlayer);
		if (n == I::EngineClient->GetLocalPlayer())
			continue;	

		bool bDormant = pPlayer->IsDormant();
		float flOldSimTime = m_mOldSimTimes[n] = m_mSimTimes.contains(n) ? m_mSimTimes[n] : pPlayer->m_flOldSimulationTime();
		float flSimTime = m_mSimTimes[n] = (bDormant ? m_pLocal : pPlayer)->m_flSimulationTime(); // lol
		float flDeltaTime = m_mDeltaTimes[n] = TICKS_TO_TIME(std::clamp(TIME_TO_TICKS(flSimTime - flOldSimTime) - iLag, 0, 24));
		if (flDeltaTime)
		{
			m_mLagTimes[n] = flDeltaTime;
			m_mSetTicks[n] = I::GlobalVars->tickcount;
			if (!bDormant)
			{
				m_mOrigins[n].emplace_front(pPlayer->m_vecOrigin() + Vector(0, 0, pPlayer->GetSize().z), flSimTime);
				if (m_mOrigins[n].size() > 5)
					m_mOrigins[n].pop_back();
			}
			else
				m_mOrigins[n].clear();
		}
		m_mChokes[n] = I::GlobalVars->tickcount - m_mSetTicks[n];

		if (!flDeltaTime)
			continue;
		else if (bDormant)
		{
			m_mBones[n].first = false;
			continue;
		}

		m_mOldAngles[n] = m_mEyeAngles[n];
		m_mEyeAngles[n] = pPlayer->As<C_CSPlayer>()->GetEyeAngles();
	}

	for (int n = 1; n <= I::EngineClient->GetMaxClients(); n++)
	{
		auto pPlayer = I::ClientEntityList->GetClientEntity(n)->As<C_CSPlayer>();
		if (!pPlayer || !pPlayer->IsPlayer()
			|| n == I::EngineClient->GetLocalPlayer() && !I::EngineClient->IsPlayingDemo() // local player managed in CreateMove
			|| pPlayer->IsDormant() || !pPlayer->IsAlive())
			continue;

		int iDeltaTicks = TIME_TO_TICKS(H::Entities.GetDeltaTime(n));
		if (!iDeltaTicks)
			continue;

		if (CFG::Visuals_NoInterpolation)
		{
			float flOldFrameTime = I::GlobalVars->frametime;
			I::GlobalVars->frametime = I::ClientPrediction->m_bEnginePaused ? 0.f : TICK_INTERVAL;
			/*for (int i = 0; i < iDeltaTicks; i++)
			{
				G::UpdatingAnims = true;

				pPlayer->UpdateClientSideAnimation();

				G::UpdatingAnims = false;
			}*/
			I::GlobalVars->frametime = flOldFrameTime;
		}

		m_bSettingUpBones = true;
		m_mBones[n].first = pPlayer->SetupBones(m_mBones[n].second, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, m_mSimTimes[n]);
		m_bSettingUpBones = false;
	}
}

void CEntities::Clear(bool bShutdown)
{
	m_pLocal = nullptr;
	m_pLocalWeapon = nullptr;
	m_pPlayerResource = nullptr;
	m_mGroups.clear();

	if (bShutdown)
	{
		m_mSimTimes.clear();
		m_mOldSimTimes.clear();
		m_mDeltaTimes.clear();
		m_mLagTimes.clear();
		m_mChokes.clear();
		m_mSetTicks.clear();
		m_mBones.clear();
		m_mOldAngles.clear();
		m_mEyeAngles.clear();
		m_mLagCompensation.clear();
		m_mDormancy.clear();
		m_mAvgVelocities.clear();
		m_mModels.clear();
		m_mOrigins.clear();
	}
}

void CEntities::ManualNetwork(const StartSoundParams_t& params)
{
	if (params.soundsource <= 0 || params.soundsource == I::EngineClient->GetLocalPlayer())
		return;

	auto pEntity = I::ClientEntityList->GetClientEntity(params.soundsource)->As<C_BaseEntity>();
	if (pEntity && pEntity->IsDormant() && pEntity->IsPlayer())
		m_mDormancy[params.soundsource] = { params.origin, I::EngineClient->Time() };
}

C_CSPlayer* CEntities::GetLocal()
{
	return I::ClientEntityList->GetClientEntity(I::EngineClient->GetLocalPlayer())->As<C_CSPlayer>();
	//return m_pLocal;
}
C_WeaponCSBase* CEntities::GetWeapon()
{
	auto pLocal = GetLocal();
	return pLocal ? pLocal->m_hActiveWeapon()->As<C_WeaponCSBase>() : nullptr;
	//return m_pLocalWeapon;
}
C_CSPlayerResource* CEntities::GetPR()
{
	return m_pPlayerResource;
}

const std::vector<C_BaseEntity*>& CEntities::GetGroup(const EGroupType& Group) { return m_mGroups[Group]; }

float CEntities::GetSimTime(int iIndex) { if (m_mSimTimes.contains(iIndex)) return m_mSimTimes[iIndex]; auto pEntity = I::ClientEntityList->GetClientEntity(iIndex)->As<C_CSPlayer>(); if (pEntity) return pEntity->m_flSimulationTime(); return 0.f; }
float CEntities::GetOldSimTime(int iIndex) { if (m_mOldSimTimes.contains(iIndex)) return m_mOldSimTimes[iIndex]; auto pEntity = I::ClientEntityList->GetClientEntity(iIndex)->As<C_CSPlayer>(); if (pEntity) return pEntity->m_flOldSimulationTime(); return 0.f; }
float CEntities::GetDeltaTime(int iIndex) { return m_mDeltaTimes.contains(iIndex) ? m_mDeltaTimes[iIndex] : TICK_INTERVAL; }
float CEntities::GetLagTime(int iIndex) { return m_mLagTimes.contains(iIndex) ? m_mLagTimes[iIndex] : TICK_INTERVAL; }
int CEntities::GetChoke(int iIndex) { return m_mChokes.contains(iIndex) ? m_mChokes[iIndex] : 0; }
bool CEntities::GetDormancy(int iIndex) { return m_mDormancy.contains(iIndex); }
matrix3x4_t* CEntities::GetBones(int iIndex) { return m_mBones[iIndex].first ? m_mBones[iIndex].second : nullptr; }
Vector CEntities::GetEyeAngles(int iIndex) { return m_mEyeAngles.contains(iIndex) ? m_mEyeAngles[iIndex] : Vector(); }
bool CEntities::GetLagCompensation(int iIndex) { return m_mLagCompensation[iIndex]; }
void CEntities::SetLagCompensation(int iIndex, bool bLagComp) { m_mLagCompensation[iIndex] = bLagComp; }
Vector* CEntities::GetAvgVelocity(int iIndex) { return iIndex != I::EngineClient->GetLocalPlayer() ? &m_mAvgVelocities[iIndex] : nullptr; }
void CEntities::SetAvgVelocity(int iIndex, Vector vAvgVelocity) { m_mAvgVelocities[iIndex] = vAvgVelocity; }
uint32_t CEntities::GetModel(int iIndex) { return m_mModels[iIndex]; }
std::deque<VelFixRecord>* CEntities::GetOrigins(int iIndex) { return m_mOrigins.contains(iIndex) ? &m_mOrigins[iIndex] : nullptr; }

bool CEntities::IsSettingUpBones() { return m_bSettingUpBones; }