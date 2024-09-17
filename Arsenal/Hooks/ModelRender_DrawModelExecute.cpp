#include "../SDK/SDK.h"

#include "../Features/Materials/Materials.h"
#include "../Features/Outlines/Outlines.h"

MAKE_HOOK(ModelRender_DrawModelExecute, U::VFunc.Get<void*>(I::ModelRender, 19u), void, __fastcall,
	void* ecx, void* edx, const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4_t* pBoneToWorld)
{
	if (CFG::Visuals_LowGraphics)
		*const_cast<int*>(&pState.m_lod) = 7;

	const auto pClientEntity = I::ClientEntityList->GetClientEntity(pInfo.entity_index);

	if (pClientEntity)
	{
		if (pClientEntity->GetClassID() == ECSClientClass::CDynamicProp)
		{
			if (CFG::Visuals_World_Modulation_Mode == 0)
			{
				if (auto flNightMode = CFG::Visuals_Night_Mode)
				{
					auto col{ static_cast<byte>(U::Math.RemapValClamped(flNightMode, 0.0f, 100.0f, 255.0f, 50.0f)) };

					I::RenderView->SetColorModulation({ col, col, col, static_cast<byte>(255) });
				}
			}
			else
			{
				I::RenderView->SetColorModulation(CFG::Colors_Props);
			}

			CALL_ORIGINAL(ecx, edx, pState, pInfo, pBoneToWorld);

			I::RenderView->SetColorModulation({ 255, 255, 255, 255 });

			return;
		}

		if (!I::EngineClient->IsTakingScreenshot())
		{
			const auto pEntity = pClientEntity->As<C_BaseEntity>();

			if (!F::Materials.IsRendering() && !F::Outlines.IsRendering() && (F::Outlines.HasDrawn(pEntity) || F::Materials.HasDrawn(pEntity)))
				return;
		}
	}

	CALL_ORIGINAL(ecx, edx, pState, pInfo, pBoneToWorld);
}