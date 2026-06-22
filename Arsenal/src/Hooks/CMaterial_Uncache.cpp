//#include "../SDK/SDK.h"
//#include "../Features/Materials/Materials.h"
//
//MAKE_SIGNATURE(CMaterial_Uncache, "MaterialSystem.dll", "55 8B EC 81 EC ? ? ? ? 56 8B F1 8B 0D ? ? ? ? 57", 0x0);
//
//MAKE_HOOK(CMaterial_Uncache, S::CMaterial_Uncache(), void,
//	IMaterial* rcx, bool bPreserveVars)
//{
//	//if (rcx && F::Materials.mMatList.contains(rcx))
//		//return;
//
//	if (rcx)
//	{
//		if (F::Materials.IsUsedMaterial(rcx) && !F::Materials.IsCleaningUp())
//			return;
//	}
//
//	CALL_ORIGINAL(rcx, bPreserveVars);
//}