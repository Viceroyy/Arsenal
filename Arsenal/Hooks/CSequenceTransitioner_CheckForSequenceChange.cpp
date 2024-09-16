#include "../SDK/SDK.h"

MAKE_SIGNATURE(CSequenceTransitioner_CheckForSequenceChange, "client.dll", "55 8B EC 53 8B 5D ? 57 8B F9 85 DB 0F 84", 0x0);

MAKE_HOOK(CSequenceTransitioner_CheckForSequenceChange, S::CSequenceTransitioner_CheckForSequenceChange(), void, __fastcall,
	void* ecx, void* edx, CStudioHdr* hdr, int nCurSequence, bool bForceNewSequence, bool bInterpolate)
{
	if (CFG::Visuals_NoInterpolation)
		bInterpolate = false;

	CALL_ORIGINAL(ecx, edx, hdr, nCurSequence, bForceNewSequence, bInterpolate);
}
