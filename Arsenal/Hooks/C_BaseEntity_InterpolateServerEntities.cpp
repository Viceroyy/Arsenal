#include "../SDK/SDK.h"

MAKE_SIGNATURE(C_BaseEntity_InterpolateServerEntities, "client.dll", "55 8B EC 83 EC ? 8B 0D ? ? ? ? 33 D2 89 55 ? 89 55 ? 89 55 ? 8B 41 ? 89 55 ? 85 C0 74 ? 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 68 ? ? ? ? 52 52 52 52 8D 4D ? 51 50 8B 40 ? FF D0 8B 45 ? 83 C4 ? 8B 0D ? ? ? ? 89 45 ? 8B 45 ? 89 45 ? 53", 0x0);

MAKE_HOOK(C_BaseEntity_InterpolateServerEntities, S::C_BaseEntity_InterpolateServerEntities(), void, __cdecl)
{
	if (CFG::Visuals_NoInterpolation)
	{
		static auto cl_extrapolate = U::ConVars.FindVar("cl_extrapolate");

		if (cl_extrapolate && cl_extrapolate->GetInt())
			cl_extrapolate->SetValue(0);
	}

	CALL_ORIGINAL();
}