#include "../SDK/SDK.h"

MAKE_SIGNATURE(C_BaseEntity_InterpolateServerEntities, "client.dll", "4C 8B DC 41 54 41 55 48 81 EC", 0x0);

MAKE_HOOK(C_BaseEntity_InterpolateServerEntities, S::C_BaseEntity_InterpolateServerEntities(), void)
{
	if (CFG::Visuals_NoInterpolation)
	{
		static auto cl_extrapolate = U::ConVars.FindVar("cl_extrapolate");

		if (cl_extrapolate && cl_extrapolate->GetInt())
			cl_extrapolate->SetValue(0);
	}

	CALL_ORIGINAL();
}