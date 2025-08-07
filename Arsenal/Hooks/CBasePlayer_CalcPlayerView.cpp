#include "../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CBasePlayer_CalcPlayerView, "client.dll", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B D9 4D 8B F1", 0x0);

MAKE_HOOK(CBasePlayer_CalcPlayerView, S::CBasePlayer_CalcPlayerView(), void,
	void* rcx, Vector& eyeOrigin, Vector& eyeAngles, float& fov)
{
	if (CFG::Visuals_NoRecoil)
	{
		if (auto pBasePlayer = reinterpret_cast<C_BasePlayer*>(rcx))
		{
			Vector vOldPunch = pBasePlayer->m_vecPunchAngle();
			pBasePlayer->m_vecPunchAngle().Init();
			CALL_ORIGINAL(rcx, eyeOrigin, eyeAngles, fov);
			pBasePlayer->m_vecPunchAngle() = vOldPunch;
			return;
		}
	}

	CALL_ORIGINAL(rcx, eyeOrigin, eyeAngles, fov);
}