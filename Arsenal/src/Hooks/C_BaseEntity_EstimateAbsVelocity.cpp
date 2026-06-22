#include "../SDK/SDK.h"

MAKE_SIGNATURE(C_BaseEntity_EstimateAbsVelocity, "client.dll", "48 89 5C 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 E8 ? ? ? ? 48 3B D8", 0x0);

MAKE_HOOK(C_BaseEntity_EstimateAbsVelocity, S::C_BaseEntity_EstimateAbsVelocity(), void,
	C_BaseEntity* rcx, Vector& vel)
{
	if (CFG::Visuals_NoInterpolation && rcx)
	{
		if (rcx->GetClassID() == ECSClassID::CCSPlayer)
		{
			if (const auto pPlayer = rcx->As<C_CSPlayer>())
			{
				vel = pPlayer->m_vecVelocity();
				return;
			}
		}
	}

	CALL_ORIGINAL(rcx, vel);
}