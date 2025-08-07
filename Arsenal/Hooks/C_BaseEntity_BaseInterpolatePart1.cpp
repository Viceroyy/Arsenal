#include "../SDK/SDK.h"

MAKE_SIGNATURE(C_BaseEntity_BaseInterpolatePart1, "client.dll", "48 89 5C 24 ? 56 57 41 55 41 56 41 57 48 83 EC ? 4C 8B BC 24", 0x0);

MAKE_HOOK(C_BaseEntity_BaseInterpolatePart1, S::C_BaseEntity_BaseInterpolatePart1(), int,
	void* rcx, float& currentTime, Vector& oldOrigin, Vector& oldAngles, Vector& oldVel, int& bNoMoreChanges)
{
	auto ShouldDisableInterp = [&]()
		{
			if (!CFG::Visuals_NoInterpolation)
				return false;

			auto pLocal = H::Entities.GetLocal();

			if (!pLocal)
				return false;

			auto pEntity = reinterpret_cast<C_BaseEntity*>(rcx);

			if (!pEntity)
				return false;

			if (pEntity->GetClassID() == ECSClassID::CCSPlayer)
				return pEntity != pLocal;

			if (pEntity->GetClassID() == ECSClassID::CBaseDoor)
				return true;

			return false;
		};

	if (ShouldDisableInterp())
	{
		bNoMoreChanges = 1;
		return 0;
	}

	return CALL_ORIGINAL(rcx, currentTime, oldOrigin, oldAngles, oldVel, bNoMoreChanges);
}