#include "../SDK/SDK.h"

#include "../Features/Materials/Materials.h"

//MAKE_SIGNATURE(Panel_OnScreenSizeChanged, "client.dll", "48 89 5C 24 ? 55 56 57 41 56 41 57 48 83 EC ? 48 8B 1D", 0x0);

MAKE_HOOK(Panel_OnScreenSizeChanged, U::VFunc.Get<void*>(I::MatSystemSurface, 111u), void,
	void* rcx, int nOldWidth, int OldHeight)
{
	CALL_ORIGINAL(rcx, nOldWidth, OldHeight);

	H::Draw.Initialize();

	F::Materials.CleanUp();
}