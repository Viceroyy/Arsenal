#include "../Hooks.h"

#include "../../Features/CFG.h"

DEFINE_HOOK(CBasePlayer_CalcPlayerView, void, __fastcall, C_BasePlayer* pThis, void* edx, Vector& eyeOrigin, Vector& eyeAngles, float& fov)
{
	if (CFG::Visual_NoRecoil && pThis)
	{
		const Vector vOldPunch = pThis->m_vecPunchAngle();

		pThis->m_vecPunchAngle().Init();
		Func.Original<FN>()(pThis, edx, eyeOrigin, eyeAngles, fov);
		pThis->m_vecPunchAngle() = vOldPunch;
	}
	else
	{
		Func.Original<FN>()(pThis, edx, eyeOrigin, eyeAngles, fov);
	}
}