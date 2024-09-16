#include "../SDK/SDK.h"

#include "../Features/Materials/Materials.h"
#include "../Features/Outlines/Outlines.h"

MAKE_SIGNATURE(Panel_OnScreenSizeChanged, "client.dll", "55 8B EC 83 EC ? 53 8B D9 C7 45 ? ? ? ? ? 56", 0x0);

MAKE_HOOK(Panel_OnScreenSizeChanged, S::Panel_OnScreenSizeChanged(), void, __fastcall,
	void* ecx, void* edx, int nOldWidth, int OldHeight)
{
	CALL_ORIGINAL(ecx, edx, nOldWidth, OldHeight);

	H::Draw.Initialize();

	F::Materials.CleanUp();
	F::Outlines.CleanUp();
}