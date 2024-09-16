#include "../SDK/SDK.h"
#include "../Features/Materials/Materials.h"
#include "../Features/Outlines/Outlines.h"

MAKE_SIGNATURE(CMaterial_Uncache, "MaterialSystem.dll", "55 8B EC 81 EC ? ? ? ? 56 8B F1 8B 0D ? ? ? ? 57", 0x0);

MAKE_HOOK(CMaterial_Uncache, S::CMaterial_Uncache(), void, __fastcall,
	IMaterial* ecx, void* edx, bool bPreserveVars)
{
	//if (ecx && F::Materials.mMatList.contains(ecx))
		//return;

	if (ecx)
	{
		if (F::Materials.IsUsedMaterial(ecx) && !F::Materials.IsCleaningUp())
			return;
		
		if (F::Outlines.IsUsedMaterial(ecx) && !F::Outlines.IsCleaningUp())
			return;
	}

	CALL_ORIGINAL(ecx, edx, bPreserveVars);
}