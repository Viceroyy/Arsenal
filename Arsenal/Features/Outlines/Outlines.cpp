#include "Outlines.h"

#include "../Materials/Materials.h"

#pragma warning (disable : 4244) //possible loss of data (int to float)

void COutlines::Initialize()
{
	if (!m_pMatGlowColor)
	{
		m_pMatGlowColor = I::MaterialSystem->FindMaterial("dev/glow_color", TEXTURE_GROUP_OTHER);
	}

	if (!m_pRtFullFrame)
	{
		m_pRtFullFrame = I::MaterialSystem->FindTexture("_rt_FullFrameFB", TEXTURE_GROUP_RENDER_TARGET);
	}

	if (!m_pRenderBuffer0)
	{
		m_pRenderBuffer0 = I::MaterialSystem->CreateNamedRenderTargetTextureEx(
			"seo_outline_buffer0",
			m_pRtFullFrame->GetActualWidth(),
			m_pRtFullFrame->GetActualHeight(),
			RT_SIZE_LITERAL,
			IMAGE_FORMAT_RGB888,
			MATERIAL_RT_DEPTH_SHARED,
			TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT | TEXTUREFLAGS_EIGHTBITALPHA,
			CREATERENDERTARGETFLAGS_HDR
		);

		m_pRenderBuffer0->IncrementReferenceCount();
	}

	if (!m_pRenderBuffer1)
	{
		m_pRenderBuffer1 = I::MaterialSystem->CreateNamedRenderTargetTextureEx(
			"seo_outline_buffer1",
			m_pRtFullFrame->GetActualWidth(),
			m_pRtFullFrame->GetActualHeight(),
			RT_SIZE_LITERAL,
			IMAGE_FORMAT_RGB888,
			MATERIAL_RT_DEPTH_SHARED,
			TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT | TEXTUREFLAGS_EIGHTBITALPHA,
			CREATERENDERTARGETFLAGS_HDR
		);

		m_pRenderBuffer1->IncrementReferenceCount();
	}

	if (!m_pMatHaloAddToScreen)
	{
		auto kv = new KeyValues("UnlitGeneric");
		kv->SetString("$dummy", "dummy");
		kv->SetString("$basetexture", "seo_outline_buffer0");
		kv->SetString("$additive", "1");
		m_pMatHaloAddToScreen = I::MaterialSystem->CreateMaterial("seo_outline_material", kv);
	}

	if (!m_pMatBlurX)
	{
		auto kv = new KeyValues("BlurFilterX");
		kv->SetString("$dummy", "dummy");
		kv->SetString("$basetexture", "seo_outline_buffer0");
		m_pMatBlurX = I::MaterialSystem->CreateMaterial("seo_outline_material_blurx", kv);
	}

	if (!m_pMatBlurY)
	{
		auto kv = new KeyValues("BlurFilterY");
		kv->SetString("$dummy", "dummy");
		kv->SetString("$basetexture", "seo_outline_buffer1");
		m_pMatBlurY = I::MaterialSystem->CreateMaterial("seo_outline_material_blury", kv);
		m_pBloomAmount = m_pMatBlurY->FindVar("$bloomamount", nullptr);
	}
}

void COutlines::DrawEntity(C_BaseEntity* pEntity, bool bModel)
{
	m_bRendering = true;

	if (!bModel)
		m_bRenderingOutlines = true;

	pEntity->DrawModel(bModel ? STUDIO_RENDER : (STUDIO_RENDER | STUDIO_NOSHADOWS));

	if (bModel)
		m_mapDrawnEntities[pEntity] = true;

	if (!bModel)
		m_bRenderingOutlines = false;

	m_bRendering = false;
}

void COutlines::RunModels()
{
	Initialize();

	if (!m_mapDrawnEntities.empty())
		m_mapDrawnEntities.clear();

	if (!m_vecOutlineEntities.empty())
		m_vecOutlineEntities.clear();

	if (!CFG::Outlines_Active || I::EngineVGui->IsGameUIVisible())
		return;

	int w = H::Draw.m_nScreenW, h = H::Draw.m_nScreenH;

	if (w < 1 || h < 1 || w > 4096 || h > 2160)
		return;

	auto pRC = I::MaterialSystem->GetRenderContext();

	if (!pRC)
		return;

	auto pLocal = H::EntityCache.GetLocal();

	if (!pLocal)
		return;

	ShaderStencilState_t StencilStateDisable = {};
	StencilStateDisable.m_bEnable = false;

	float flOriginalColor[3] = {};
	I::RenderView->GetColorModulation(flOriginalColor);
	float flOriginalBlend = I::RenderView->GetBlend();

	SetModelStencil(pRC);

	I::RenderView->SetBlend(1.0f);
	I::RenderView->SetColorModulation(1.0f, 1.0f, 1.0f);

	if (CFG::Outlines_Players_Active)
	{
		for (auto pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ALL))
		{
			if (!pEntity)
				continue;

			auto pPlayer = pEntity->As<C_CSPlayer>();

			if (pPlayer->deadflag())
				continue;

			bool bIsLocal = pPlayer == pLocal;
			bool bIsFriend = false;

			if (CFG::Outlines_Players_Ignore_Local && bIsLocal)
				continue;

			if (CFG::Outlines_Players_Ignore_Friends && bIsFriend)
				continue;

			if (!bIsLocal)
			{
				if (!bIsFriend)
				{
					if (CFG::Outlines_Players_Ignore_Teammates && pPlayer->m_iTeamNum() == pLocal->m_iTeamNum())
						continue;

					if (CFG::Outlines_Players_Ignore_Enemies && pPlayer->m_iTeamNum() != pLocal->m_iTeamNum())
						continue;
				}
			}

			if (!Util::IsOnScreen(pLocal, pPlayer))
				continue;

			auto Color = Util::GetEntityColor(pLocal, pPlayer, CFG::Colors_Relative);

			m_vecOutlineEntities.emplace_back(OutlineEntity_t{ pPlayer, Color, CFG::Outlines_Players_Alpha });

			if (!F::Materials.HasDrawn(pPlayer))
				DrawEntity(pPlayer, true);

			C_BaseEntity* pAttach = pPlayer->FirstMoveChild();

			for (int n = 0; n < 32; n++)
			{
				if (!pAttach)
					break;

				if (pAttach->ShouldDraw())
				{
					m_vecOutlineEntities.emplace_back(OutlineEntity_t{ pAttach, Color, CFG::Outlines_Players_Alpha });

					if (!F::Materials.HasDrawn(pAttach))
						DrawEntity(pAttach, true);
				}

				pAttach = pAttach->NextMovePeer();
			}
		}
	}

	if (CFG::Outlines_World_Active)
	{
		if (!CFG::Outlines_World_Ignore_PlantedC4)
		{
			auto Color = CFG::Colors_PlantedC4;

			for (auto pEntity : H::EntityCache.GetGroup(EGroupType::WORLD_C4PLANTED))
			{
				if (!pEntity || !pEntity->ShouldDraw() || !Util::IsOnScreen(pLocal, pEntity))
					continue;

				m_vecOutlineEntities.emplace_back(OutlineEntity_t{ pEntity, Color, CFG::Outlines_World_Alpha });

				if (!F::Materials.HasDrawn(pEntity))
					DrawEntity(pEntity, true);
			}
		}

		if (!CFG::Outlines_World_Ignore_DroppedWeapons)
		{
			auto Color = CFG::Colors_DroppedWeapons;

			for (auto pEntity : H::EntityCache.GetGroup(EGroupType::WORLD_WEAPONS))
			{
				if (!pEntity || !pEntity->ShouldDraw() || !Util::IsOnScreen(pLocal, pEntity))
					continue;

				m_vecOutlineEntities.emplace_back(OutlineEntity_t{ pEntity, Color, CFG::Outlines_World_Alpha });

				if (!F::Materials.HasDrawn(pEntity))
					DrawEntity(pEntity, true);
			}
		}

		/*bool bIgnoringAllProjectiles = CFG::Outlines_World_Ignore_LocalProjectiles
			&& CFG::Outlines_World_Ignore_EnemyProjectiles
			&& CFG::Outlines_World_Ignore_TeammateProjectiles;

		if (!bIgnoringAllProjectiles)
		{
			for (auto pEntity : H::EntityCache.GetGroup(EGroupType::PROJECTILES_ALL))
			{
				if (!pEntity || !pEntity->ShouldDraw())
					continue;

				bool bIsLocal = F::VisualUtils->IsEntityOwnedBy(pEntity, pLocal);

				if (CFG::Outlines_World_Ignore_LocalProjectiles && bIsLocal)
					continue;

				if (!bIsLocal)
				{
					if (CFG::Outlines_World_Ignore_EnemyProjectiles && pEntity->m_iTeamNum() != pLocal->m_iTeamNum())
						continue;

					if (CFG::Outlines_World_Ignore_TeammateProjectiles && pEntity->m_iTeamNum() == pLocal->m_iTeamNum())
						continue;
				}

				if (!Util::IsOnScreen(pLocal, pEntity))
					continue;

				auto Color = Util::GetEntityColor(pLocal, pEntity);

				m_vecOutlineEntities.emplace_back(OutlineEntity_t{ pEntity, Color, CFG::Outlines_World_Alpha });

				if (!F::Materials.HasDrawn(pEntity))
					DrawEntity(pEntity, true);
			}
		}*/
	}

	StencilStateDisable.SetStencilState(pRC);

	I::ModelRender->ForcedMaterialOverride(nullptr);
	I::RenderView->SetColorModulation(flOriginalColor);
	I::RenderView->SetBlend(flOriginalBlend);
}

void COutlines::Run()
{
	if (!CFG::Outlines_Active || I::EngineVGui->IsGameUIVisible())
		return;

	if (CFG::Visuals_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
		return;

	int w = H::Draw.m_nScreenW, h = H::Draw.m_nScreenH;

	if (w < 1 || h < 1 || w > 4096 || h > 2160)
		return;

	auto pRC = I::MaterialSystem->GetRenderContext();

	if (!pRC)
		return;

	if (CFG::Outlines_Style == 0)
		m_pBloomAmount->SetIntValue(CFG::Outlines_Bloom_Amount);

	ShaderStencilState_t StencilStateDisable = {};
	StencilStateDisable.m_bEnable = false;

	float flOriginalColor[3] = {};
	I::RenderView->GetColorModulation(flOriginalColor);
	float flOriginalBlend = I::RenderView->GetBlend();

	if (m_vecOutlineEntities.empty())
		return;

	I::ModelRender->ForcedMaterialOverride(m_pMatGlowColor);

	pRC->PushRenderTargetAndViewport();
	{
		pRC->SetRenderTarget(m_pRenderBuffer0);
		pRC->Viewport(0, 0, w, h);
		pRC->ClearColor4ub(0, 0, 0, 0);
		pRC->ClearBuffers(true, false, false);

		for (const auto& Entity : m_vecOutlineEntities) {
			I::RenderView->SetBlend(Entity.m_flAlpha);
			I::RenderView->SetColorModulation(static_cast<float>(Entity.m_Color.r) / 255.0f, static_cast<float>(Entity.m_Color.g) / 255.0f, static_cast<float>(Entity.m_Color.b) / 255.0f);
			DrawEntity(Entity.m_pEntity, false);
		}

		StencilStateDisable.SetStencilState(pRC);
	}
	pRC->PopRenderTargetAndViewport();

	if (CFG::Outlines_Style == 0)
	{
		pRC->PushRenderTargetAndViewport();
		{
			pRC->Viewport(0, 0, w, h);
			pRC->SetRenderTarget(m_pRenderBuffer1);
			pRC->DrawScreenSpaceRectangle(m_pMatBlurX, 0, 0, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h);
			pRC->SetRenderTarget(m_pRenderBuffer0);
			pRC->DrawScreenSpaceRectangle(m_pMatBlurY, 0, 0, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h);
		}
		pRC->PopRenderTargetAndViewport();
	}

	ShaderStencilState_t SEffect = {};
	SEffect.m_bEnable = true;
	SEffect.m_nWriteMask = 0x0;
	SEffect.m_nTestMask = 0xFF;
	SEffect.m_nReferenceValue = 0;
	SEffect.m_CompareFunc = STENCILCOMPARISONFUNCTION_EQUAL;
	SEffect.m_PassOp = STENCILOPERATION_KEEP;
	SEffect.m_FailOp = STENCILOPERATION_KEEP;
	SEffect.m_ZFailOp = STENCILOPERATION_KEEP;
	SEffect.SetStencilState(pRC);

	switch (CFG::Outlines_Style)
	{
	case 0:
	{
		pRC->DrawScreenSpaceRectangle(m_pMatHaloAddToScreen, 0, 0, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h);
		break;
	}

	case 1:
	{
		pRC->DrawScreenSpaceRectangle(m_pMatHaloAddToScreen, -1, -1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h);
		pRC->DrawScreenSpaceRectangle(m_pMatHaloAddToScreen, -1, 1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h);
		pRC->DrawScreenSpaceRectangle(m_pMatHaloAddToScreen, 1, -1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h);
		pRC->DrawScreenSpaceRectangle(m_pMatHaloAddToScreen, 1, 1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h);
		break;
	}

	case 2:
	{
		pRC->DrawScreenSpaceRectangle(m_pMatHaloAddToScreen, -2, -1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h);
		pRC->DrawScreenSpaceRectangle(m_pMatHaloAddToScreen, 2, 1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h);
		pRC->DrawScreenSpaceRectangle(m_pMatHaloAddToScreen, -2, -1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h);
		pRC->DrawScreenSpaceRectangle(m_pMatHaloAddToScreen, 2, 1, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h);
		break;
	}

	case 3:
	{
		pRC->DrawScreenSpaceRectangle(m_pMatHaloAddToScreen, -2, -2, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h);
		pRC->DrawScreenSpaceRectangle(m_pMatHaloAddToScreen, -2, 2, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h);
		pRC->DrawScreenSpaceRectangle(m_pMatHaloAddToScreen, 2, -2, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h);
		pRC->DrawScreenSpaceRectangle(m_pMatHaloAddToScreen, 2, 2, w, h, 0.0f, 0.0f, w - 1, h - 1, w, h);
		break;
	}

	default: break;
	}

	StencilStateDisable.SetStencilState(pRC);

	I::ModelRender->ForcedMaterialOverride(nullptr);
	I::RenderView->SetColorModulation(flOriginalColor);
	I::RenderView->SetBlend(flOriginalBlend);
}

void COutlines::CleanUp()
{
	m_bCleaningUp = true;

	/*if (m_pMatHaloAddToScreen)
	{
		m_pMatHaloAddToScreen->DecrementReferenceCount();
		m_pMatHaloAddToScreen->DeleteIfUnreferenced();
		m_pMatHaloAddToScreen = nullptr;
	}

	if (m_pRenderBuffer0)
	{
		m_pRenderBuffer0->DecrementReferenceCount();
		m_pRenderBuffer0->DeleteIfUnreferenced();
		m_pRenderBuffer0 = nullptr;
	}

	if (m_pRenderBuffer1)
	{
		m_pRenderBuffer1->DecrementReferenceCount();
		m_pRenderBuffer1->DeleteIfUnreferenced();
		m_pRenderBuffer1 = nullptr;
	}

	if (m_pMatBlurX)
	{
		m_pMatBlurX->DecrementReferenceCount();
		m_pMatBlurX->DeleteIfUnreferenced();
		m_pMatBlurX = nullptr;
	}

	if (m_pMatBlurY)
	{
		m_pMatBlurY->DecrementReferenceCount();
		m_pMatBlurY->DeleteIfUnreferenced();
		m_pMatBlurY = nullptr;
	}*/

	m_bCleaningUp = false;
}

void COutlines::SetModelStencil(IMatRenderContext* pRenderContext)
{
	ShaderStencilState_t State = {};
	State.m_bEnable = true;
	State.m_nReferenceValue = 1;
	State.m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
	State.m_PassOp = STENCILOPERATION_REPLACE;
	State.m_FailOp = STENCILOPERATION_KEEP;
	State.m_ZFailOp = STENCILOPERATION_REPLACE;
	State.SetStencilState(pRenderContext);
}