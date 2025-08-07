#include "../SDK/SDK.h"

std::unordered_map<void*, std::pair<int, float>> pAnimatingInfo;

MAKE_SIGNATURE(C_BaseAnimating_FrameAdvance, "client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 57 48 81 EC ? ? ? ? 44 0F 29 54 24", 0x0);

MAKE_HOOK(C_BaseAnimating_FrameAdvance, S::C_BaseAnimating_FrameAdvance(), float,
	void* rcx, float flInterval)
{
	if (!CFG::Visuals_NoInterpolation || rcx == H::Entities.GetLocal())
		return CALL_ORIGINAL(rcx, flInterval);

	const auto pEntity = static_cast<C_BaseEntity*>(rcx);
	if (pEntity && pEntity->IsPlayer())
	{
		if (pEntity->m_flSimulationTime() == pEntity->m_flOldSimulationTime() || I::GlobalVars->tickcount == pAnimatingInfo[rcx].first)
		{
			pAnimatingInfo[rcx].second += flInterval;
			return 0.f;
		}
	}

	flInterval = pAnimatingInfo[rcx].second; pAnimatingInfo[rcx].second = 0.f; pAnimatingInfo[rcx].first = I::GlobalVars->tickcount;
	return CALL_ORIGINAL(rcx, flInterval);
}