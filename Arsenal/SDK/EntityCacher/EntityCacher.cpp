#include "EntityCacher.h"

void CHelpers_EntityCache::Fill()
{
	auto pLocal = I::ClientEntityList->GetClientEntity(I::EngineClient->GetLocalPlayer());
	if (!pLocal)
		return;

	m_pLocal = pLocal->As<C_CSPlayer>();
	if (auto pEntity = m_pLocal->GetActiveCSWeapon())
		m_pLocalWeapon = pEntity;

	switch (m_pLocal->m_iObserverMode())
	{
		case OBS_MODE_FIRSTPERSON:
		case OBS_MODE_THIRDPERSON:
		{
			if (auto pObservedTarget = m_pLocal->m_hObserverTarget().Get())
				m_pObservedTarget = pObservedTarget->As<C_CSPlayer>();
			break;
		}
		default: break;
	}

	for (int n = 1; n < I::ClientEntityList->GetHighestEntityIndex(); n++)
	{
		auto pClientEntity = I::ClientEntityList->GetClientEntity(n);
		if (!pClientEntity || pClientEntity->IsDormant())
			continue;

		auto pEntity = pClientEntity->As<C_BaseEntity>();

		switch (pEntity->GetClassID())
		{
			case ECSClientClass::CCSPlayer:
			{
				m_mGroups[EGroupType::PLAYERS_ALL].push_back(pEntity);
				m_mGroups[pEntity->m_iTeamNum() != m_pLocal->m_iTeamNum() ? EGroupType::PLAYERS_ENEMIES : EGroupType::PLAYERS_TEAMMATES].push_back(pEntity);
				break;
			}
			case ECSClientClass::CPlantedC4:
			{
				m_mGroups[EGroupType::WORLD_C4PLANTED].push_back(pEntity);
				break;
			}
			/*case ECSClientClass::CAK47:
			case ECSClientClass::CWeaponP228:
			case ECSClientClass::CWeaponGlock:
			case ECSClientClass::CWeaponScout:
			case ECSClientClass::CHEGrenade:
			case ECSClientClass::CWeaponXM1014:
			case ECSClientClass::CC4:
			case ECSClientClass::CWeaponMAC10:
			case ECSClientClass::CWeaponAug:
			case ECSClientClass::CSmokeGrenade:
			case ECSClientClass::CWeaponElite:
			case ECSClientClass::CWeaponFiveSeven:
			case ECSClientClass::CWeaponUMP45:
			case ECSClientClass::CWeaponSG550:
			case ECSClientClass::CWeaponGalil:
			case ECSClientClass::CWeaponFamas:
			case ECSClientClass::CWeaponUSP:
			case ECSClientClass::CWeaponAWP:
			case ECSClientClass::CWeaponMP5Navy:
			case ECSClientClass::CWeaponM249:
			case ECSClientClass::CWeaponM3:
			case ECSClientClass::CWeaponM4A1:
			case ECSClientClass::CWeaponTMP:
			case ECSClientClass::CWeaponG3SG1:
			case ECSClientClass::CFlashbang:
			case ECSClientClass::CDEagle:
			case ECSClientClass::CWeaponSG552:
			case ECSClientClass::CWeaponP90:
			{
				m_mGroups[EGroupType::WORLD_WEAPONS].push_back(pEntity);
				break;
			}*/
		}
	}
}

void CHelpers_EntityCache::Clear()
{
	m_pLocal = nullptr;
	m_pLocalWeapon = nullptr;
	m_pObservedTarget = nullptr;
	m_mGroups.clear();
}