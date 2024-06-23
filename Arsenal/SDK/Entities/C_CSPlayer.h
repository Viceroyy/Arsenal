#pragma once
#include "C_BasePlayer.h"
#include "C_WeaponCSBase.h"

class C_WeaponCSBase;

class ICSPlayerAnimStateHelpers
{
public:
	virtual C_WeaponCSBase* CSAnim_GetActiveWeapon() = 0;
	virtual bool CSAnim_CanMove() = 0;
};

class C_CSPlayer : public C_BasePlayer, public ICSPlayerAnimStateHelpers
{
public:
	virtual ~C_CSPlayer() = 0;

	NETVAR(m_flStamina, float, "CCSPlayer", "m_flStamina");
	NETVAR(m_iDirection, int, "CCSPlayer", "m_iDirection");
	NETVAR(m_iShotsFired, int, "CCSPlayer", "m_iShotsFired");
	NETVAR(m_flVelocityModifier, float, "CCSPlayer", "m_flVelocityModifier");
	NETVAR(m_vecOrigin, Vector, "CCSPlayer", "m_vecOrigin");
	NETVAR(m_bPlayerDominated, void*, "CCSPlayer", "m_bPlayerDominated");
	NETVAR(m_bPlayerDominatingMe, void*, "CCSPlayer", "m_bPlayerDominatingMe");
	NETVAR(m_iAddonBits, int, "CCSPlayer", "m_iAddonBits");
	NETVAR(m_iPrimaryAddon, int, "CCSPlayer", "m_iPrimaryAddon");
	NETVAR(m_iSecondaryAddon, int, "CCSPlayer", "m_iSecondaryAddon");
	NETVAR(m_iThrowGrenadeCounter, int, "CCSPlayer", "m_iThrowGrenadeCounter");
	NETVAR(m_iPlayerState, int, "CCSPlayer", "m_iPlayerState");
	NETVAR(m_iAccount, int, "CCSPlayer", "m_iAccount");
	NETVAR(m_bInBombZone, bool, "CCSPlayer", "m_bInBombZone");
	NETVAR(m_bInBuyZone, bool, "CCSPlayer", "m_bInBuyZone");
	NETVAR(m_iClass, int, "CCSPlayer", "m_iClass");
	NETVAR(m_ArmorValue, int, "CCSPlayer", "m_ArmorValue");
	NETVAR(m_angEyeAngles, float, "CCSPlayer", "m_angEyeAngles[0]");
	NETVAR(m_bHasDefuser, bool, "CCSPlayer", "m_bHasDefuser");
	NETVAR(m_bNightVisionOn, bool, "CCSPlayer", "m_bNightVisionOn");
	NETVAR(m_bHasNightVision, bool, "CCSPlayer", "m_bHasNightVision");
	NETVAR(m_bInHostageRescueZone, bool, "CCSPlayer", "m_bInHostageRescueZone");
	NETVAR(m_bIsDefusing, bool, "CCSPlayer", "m_bIsDefusing");
	NETVAR(m_bResumeZoom, bool, "CCSPlayer", "m_bResumeZoom");
	NETVAR(m_iLastZoom, int, "CCSPlayer", "m_iLastZoom");
	NETVAR(m_bHasHelmet, bool, "CCSPlayer", "m_bHasHelmet");
	NETVAR(m_vecRagdollVelocity, Vector, "CCSPlayer", "m_vecRagdollVelocity");
	NETVAR(m_flFlashDuration, float, "CCSPlayer", "m_flFlashDuration");
	NETVAR(m_flFlashMaxAlpha, float, "CCSPlayer", "m_flFlashMaxAlpha");
	NETVAR(m_iProgressBarDuration, int, "CCSPlayer", "m_iProgressBarDuration");
	NETVAR(m_flProgressBarStartTime, float, "CCSPlayer", "m_flProgressBarStartTime");
	NETVAR(m_hRagdoll, EHANDLE, "CCSPlayer", "m_hRagdoll");
	NETVAR(m_cycleLatch, int, "CCSPlayer", "m_cycleLatch");


public:
	inline C_WeaponCSBase* GetActiveCSWeapon()
	{
		C_BaseCombatWeapon* pWeapon = GetActiveWeapon();

		return pWeapon ? pWeapon->As<C_WeaponCSBase>() : nullptr;
	}
};

namespace Util
{
	inline C_CSPlayer* CSPlayerByIndex(const int entindex)
	{
		IClientEntity* pEntity = Util::EntityByIndex(entindex);

		return pEntity ? pEntity->As<C_CSPlayer>() : nullptr;
	}
}