#pragma once
#include "../DrawManager/DrawManager.h"
#include <unordered_map>

enum struct EGroupType
{
	PLAYERS_ALL, PLAYERS_ENEMIES, PLAYERS_TEAMMATES,
	WORLD_C4PLANTED, WORLD_WEAPONS
};

class CHelpers_EntityCache
{
	C_CSPlayer* m_pLocal = nullptr;
	C_WeaponCSBase* m_pLocalWeapon = nullptr;
	C_CSPlayerResource* m_pPlayerResource = nullptr;
	C_CSPlayer* m_pObservedTarget = nullptr;

	std::unordered_map<EGroupType, std::vector<C_BaseEntity*>> m_mGroups = {};

public:
	void Fill();
	void Clear();

	C_CSPlayer* GetLocal() { return m_pLocal; }
	C_WeaponCSBase* GetWeapon() { return m_pLocalWeapon; }
	C_CSPlayerResource* GetPR() { return m_pPlayerResource; }
	C_CSPlayer* GetObservedTarget() { return m_pObservedTarget; }

	const std::vector<C_BaseEntity*>& GetGroup(const EGroupType& Group) { return m_mGroups[Group]; }
};

namespace H { inline CHelpers_EntityCache EntityCache; }