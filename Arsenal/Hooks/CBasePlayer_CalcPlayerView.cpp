#include "../SDK/SDK.h"

#include "../../Features/CFG.h"

MAKE_HOOK(CBasePlayer_CalcPlayerView, U::Offsets.C_BasePlayer_CalcPlayerView, void, __fastcall, C_BasePlayer* pThis, void* edx, Vector& eyeOrigin, Vector& eyeAngles, float& fov)
{
	if (CFG::Visual_NoRecoil && pThis)
	{
		const Vector vOldPunch = pThis->m_vecPunchAngle();

		pThis->m_vecPunchAngle().Init();
		CALL_ORIGINAL(pThis, edx, eyeOrigin, eyeAngles, fov);
		pThis->m_vecPunchAngle() = vOldPunch;
	}
	else
	{
		CALL_ORIGINAL(pThis, edx, eyeOrigin, eyeAngles, fov);
	}
}