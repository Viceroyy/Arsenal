#include "../SDK/SDK.h"

MAKE_SIGNATURE(C_BaseEntity_BaseInterpolatePart1, "client.dll", "55 8B EC 53 8B 5D ? 56 8B F1 C7 03", 0x0);

MAKE_HOOK(C_BaseEntity_BaseInterpolatePart1, S::C_BaseEntity_BaseInterpolatePart1(), int, __fastcall, 
	void* ecx, void* edx, float& currentTime, Vector& oldOrigin, Vector& oldAngles, Vector& oldVel, int& bNoMoreChanges)
{
	auto ShouldDisableInterp = [&]()
		{
			if (!CFG::Visuals_NoInterpolation)
				return false;

			auto pLocal = H::EntityCache.GetLocal();

			if (!pLocal)
				return false;

			auto pEntity = reinterpret_cast<C_BaseEntity*>(ecx);

			if (!pEntity)
				return false;

			if (pEntity->GetClassID() == ECSClientClass::CCSPlayer)
				return pEntity != pLocal;

			if (pEntity->GetClassID() == ECSClientClass::CBaseDoor)
				return true;

			return false;
		};

	if (ShouldDisableInterp())
	{
		bNoMoreChanges = 1;

		return 0;
	}

	return CALL_ORIGINAL(ecx, edx, currentTime, oldOrigin, oldAngles, oldVel, bNoMoreChanges);
}