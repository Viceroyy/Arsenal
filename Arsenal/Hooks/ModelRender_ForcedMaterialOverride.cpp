#include "../SDK/SDK.h"

#include "../Features/Materials/Materials.h"

MAKE_HOOK(ModelRender_ForcedMaterialOverride, U::VFunc.Get<void*>(I::ModelRender, 1u), void,
	void* rcx, IMaterial* mat, OverrideType_t type)
{
	if (F::Materials.IsRendering() && !F::Materials.IsRenderingOriginalMat() && !F::Materials.IsUsedMaterial(mat))
		return;

	CALL_ORIGINAL(rcx, mat, type);
}