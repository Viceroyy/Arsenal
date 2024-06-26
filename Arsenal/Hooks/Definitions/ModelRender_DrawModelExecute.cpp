//#include "../Hooks.h"
//
//DEFINE_HOOK(ModelRender_DrawModelExecute, void, __fastcall, void* ecx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
//{
//	if (CFG::Visual_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
//		return Func.Original<FN>()(ecx, state, pInfo, pCustomBoneToWorld);
//
//	if (F::Chams.bRendering)
//		return F::Chams.RenderHandler(pState, pInfo, pBoneToWorld);
//	if (F::Glow.bRendering)
//		return F::Glow.RenderHandler(pState, pInfo, pBoneToWorld);
//
//	if (F::Chams.mEntities[pInfo.entity_index])
//		return;
//
//	const auto& pEntity = I::ClientEntityList->GetClientEntity(pInfo.entity_index);
//	if (pEntity && pEntity->GetClassID() == ETFClassID::CTFViewModel)
//	{
//		F::Glow.RenderViewmodel(pState, pInfo, pBoneToWorld);
//		if (F::Chams.RenderViewmodel(pState, pInfo, pBoneToWorld))
//			return;
//	}
//
//	CALL_ORIGINAL(ecx, pState, pInfo, pBoneToWorld);
//}