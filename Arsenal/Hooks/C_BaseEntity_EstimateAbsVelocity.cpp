#include "../SDK/SDK.h"

MAKE_SIGNATURE(C_BaseEntity_EstimateAbsVelocity, "client.dll", "55 8B EC 83 EC ? 56 8B F1 E8 ? ? ? ? 3B F0", 0x0);

MAKE_HOOK(C_BaseEntity_EstimateAbsVelocity, S::C_BaseEntity_EstimateAbsVelocity(), void, __fastcall, 
	void* ecx, void* edx, Vector& vel)
{
	if (CFG::Visuals_NoInterpolation)
	{
		if (auto pEntity = reinterpret_cast<C_BaseEntity*>(ecx))
		{
			if (pEntity->GetClassID() == ECSClientClass::CCSPlayer)
			{
				if (auto pPlayer = pEntity->As<C_CSPlayer>()) {
					vel = pPlayer->m_vecVelocity();
					return;
				}
			}
		}
	}

	CALL_ORIGINAL(ecx, edx, vel);
}