#include "../SDK/SDK.h"

std::unordered_map<void*, std::pair<int, float>> pAnimatingInfo;

MAKE_SIGNATURE(C_BaseAnimating_FrameAdvance, "client.dll", "55 8B EC 83 EC ? 56 8B F1 57 80 BE ? ? ? ? ? 0F 85", 0x0);

MAKE_HOOK(C_BaseAnimating_FrameAdvance, S::C_BaseAnimating_FrameAdvance(), float, __fastcall,
	void* ecx, void* edx, float flInterval)
{
	if (!CFG::Visuals_NoInterpolation || ecx == H::EntityCache.GetLocal())
		return CALL_ORIGINAL(ecx, edx, flInterval);

	const auto pEntity = static_cast<C_BaseEntity*>(ecx);
	if (pEntity && pEntity->IsPlayer())
	{
		if (pEntity->m_flSimulationTime() == pEntity->m_flOldSimulationTime() || I::GlobalVars->tickcount == pAnimatingInfo[ecx].first)
		{
			pAnimatingInfo[ecx].second += flInterval;
			return 0.f;
		}
	}

	flInterval = pAnimatingInfo[ecx].second; pAnimatingInfo[ecx].second = 0.f; pAnimatingInfo[ecx].first = I::GlobalVars->tickcount;
	return CALL_ORIGINAL(ecx, edx, flInterval);
}