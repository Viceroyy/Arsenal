#include "../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CBasePlayer_CalcPlayerView, "client.dll", "55 8B EC 83 EC 18 53 56 8B F1 8B", 0x0);

MAKE_HOOK(CBasePlayer_CalcPlayerView, S::CBasePlayer_CalcPlayerView(), void, __fastcall, 
	void* ecx, void* edx, Vector& eyeOrigin, Vector& eyeAngles, float& fov)
{
	if (CFG::Visuals_NoRecoil)
	{
		if (auto pBasePlayer = reinterpret_cast<C_BasePlayer*>(ecx))
		{
			Vector vOldPunch = pBasePlayer->m_vecPunchAngle();
			pBasePlayer->m_vecPunchAngle().Init();
			CALL_ORIGINAL(ecx, edx, eyeOrigin, eyeAngles, fov);
			pBasePlayer->m_vecPunchAngle() = vOldPunch;
			return;
		}
	}

	CALL_ORIGINAL(ecx, edx, eyeOrigin, eyeAngles, fov);
}