#include "../SDK/SDK.h"

MAKE_SIGNATURE(C_WeaponCSBase_DrawCrosshair, "client.dll", "55 8B EC A1 ? ? ? ? 83 EC ? 83 78 ? ? 57 8B F9 0F 84 ? ? ? ? 68", 0x0);

MAKE_HOOK(C_WeaponCSBase_DrawCrosshair, S::C_WeaponCSBase_DrawCrosshair(), bool, __fastcall,
	void* ecx, void* edx)
{
	if (!CFG::Visuals_DrawCrosshairOnSnipers || CFG::Visuals_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
		return CALL_ORIGINAL(ecx, edx);

	if (auto pWeapon = reinterpret_cast<C_WeaponCSBase*>(ecx))
	{
		if (pWeapon->GetWpnData()->m_WeaponType == WEAPONTYPE_SNIPER_RIFLE)
		{
			int iBackup = pWeapon->GetWpnData()->m_WeaponType;
			pWeapon->GetWpnData()->m_WeaponType = 255;
			CALL_ORIGINAL(ecx, edx);
			pWeapon->GetWpnData()->m_WeaponType = iBackup;
		}
	}

	return CALL_ORIGINAL(ecx, edx);
}