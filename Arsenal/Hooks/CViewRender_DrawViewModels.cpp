#include "../SDK/SDK.h"

#include "../Features/Outlines/Outlines.h"

MAKE_SIGNATURE(CViewRender_DrawViewModels, "client.dll", "55 8B EC 81 EC ? ? ? ? 8B 15 ? ? ? ? 53 8B D9", 0x0);

MAKE_HOOK(CViewRender_DrawViewModels, S::CViewRender_DrawViewModels(), void, __fastcall, 
	void* ecx, void* edx, const CViewSetup& viewRender, bool drawViewmodel)
{
	CALL_ORIGINAL(ecx, edx, viewRender, drawViewmodel);

	F::Outlines.Run();
}