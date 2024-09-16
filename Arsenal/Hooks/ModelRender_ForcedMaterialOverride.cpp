#include "../SDK/SDK.h"

#include "../Features/Materials/Materials.h"
#include "../Features/Outlines/Outlines.h"

MAKE_HOOK(ModelRender_ForcedMaterialOverride, U::VFunc.Get<void*>(I::ModelRender, 1u), void, __fastcall,
	void* ecx, void* edx, IMaterial* mat, OverrideType_t type)
{
	if (F::Materials.IsRendering() && !F::Materials.IsRenderingOriginalMat() && !F::Materials.IsUsedMaterial(mat))
		return;

	if (F::Outlines.IsRenderingOutlines() && !F::Outlines.IsUsedMaterial(mat))
		return;

	CALL_ORIGINAL(ecx, edx, mat, type);
}