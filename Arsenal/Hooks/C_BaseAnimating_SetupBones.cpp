//#include "../SDK/SDK.h"
//
//#include "../Features/Backtrack/Backtrack.h"
//
//MAKE_SIGNATURE(C_BaseAnimating_SetupBones, "client.dll", "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 33 C9 33 D2", 0x0);
//
//C_BaseEntity* GetRootMoveParent(C_BaseEntity* baseEnt)
//{
//	auto pEntity = baseEnt;
//	auto pParent = baseEnt->GetMoveParent();
//
//	auto its{ 0 };
//
//	while (pParent)
//	{
//		//if (its > 32) //XD
//		//{
//		//	break;
//		//}
//
//		//its++;
//
//		pEntity = pParent;
//		pParent = pEntity->GetMoveParent();
//	}
//
//	return pEntity;
//}
//
//MAKE_HOOK(C_BaseAnimating_SetupBones, S::C_BaseAnimating_SetupBones(), bool, __fastcall,
//	C_BaseAnimating* ecx, void* edx, matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
//{
//	if (!F::LagRecords.IsSettingUpBones())
//	{
//		const auto baseEnt = reinterpret_cast<C_BaseEntity*>(reinterpret_cast<uintptr_t>(ecx) - sizeof(uintptr_t));
//
//		if (baseEnt)
//		{
//			const auto owner = GetRootMoveParent(baseEnt);
//			const auto ent = owner ? owner : baseEnt;
//
//			if (ent->GetClassID() == ECSClientClass::CCSPlayer && ent != H::EntityCache.GetLocal())
//			{
//				if (pBoneToWorldOut)
//				{
//					if (const auto bones = ent->As<C_BaseAnimating>()->GetCachedBoneData())
//						std::memcpy(pBoneToWorldOut, bones->Base(), sizeof(matrix3x4_t) * std::min(nMaxBones, bones->Count()));
//				}
//
//				return true;
//			}
//		}
//	}
//
//	return CALL_ORIGINAL(ecx, edx, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
//}