#include "../SDK/SDK.h"

MAKE_SIGNATURE(DSP_Process, "engine.dll", "55 8B EC 51 56 8B 75 ? 83 FE", 0x0);

MAKE_HOOK(DSP_Process, S::DSP_Process(), void, __cdecl, 
	unsigned int idsp, int* pbfront, int* pbrear, int* pbcenter, int sampleCount)
{
	if (CFG::Visuals_NoDSP)
		return;

	CALL_ORIGINAL(idsp, pbfront, pbrear, pbcenter, sampleCount);
}