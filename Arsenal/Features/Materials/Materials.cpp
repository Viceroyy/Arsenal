#include "Materials.h"

#include "../Backtrack/Backtrack.h"

void SetModelStencilForOutlines(C_BaseEntity* pEntity)
{
	IMatRenderContext* pRenderContext = I::MaterialSystem->GetRenderContext();

	if (!pRenderContext)
		return;

	auto pLocal = H::EntityCache.GetLocal();

	if (!pLocal)
		return;

	auto IsEntGoingToBeGlowed = [&]()
		{
			if (pEntity->GetClassID() == ECSClientClass::CCSPlayer)
			{
				if (!CFG::Outlines_Players_Active)
					return false;

				auto pPlayer = pEntity->As<C_CSPlayer>();

				bool bIsLocal = pPlayer == pLocal;
				bool bIsFriend = false;

				if (CFG::Outlines_Players_Ignore_Local && bIsLocal)
					return false;

				if (CFG::Outlines_Players_Ignore_Friends && bIsFriend)
					return false;

				if (!bIsLocal)
				{
					if (!bIsFriend)
					{
						if (CFG::Outlines_Players_Ignore_Teammates && pPlayer->m_iTeamNum() == pLocal->m_iTeamNum())
							return false;

						if (CFG::Outlines_Players_Ignore_Enemies && pPlayer->m_iTeamNum() != pLocal->m_iTeamNum())
							return false;
					}
				}
			}

			return true;
		};

	if (!IsEntGoingToBeGlowed())
		pRenderContext->SetStencilEnable(false);

	else
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
}

void CMaterials::Initialize()
{
	static ConVar* mat_hdr_level = I::Cvar->FindVar("mat_hdr_level");

	if (!m_pFlat)
	{
		auto* kv = new KeyValues("VertexLitGeneric");
		kv->SetString("$dummy", "dummy");
		kv->SetString("$basetexture", "vgui/white_additive");
		kv->SetString("$bumpmap", "vgui/white_additive");
		kv->SetString("$selfillum", "1");
		kv->SetString("$selfillumFresnel", "1");
		kv->SetString("$selfillumFresnelMinMaxExp", "[0.4999 0.5 1]");
		kv->SetString("$cloakPassEnabled", "1");
		kv->SetString("$nodecal", "1");
		kv->SetString("$model", "1");
		if (auto proxies = kv->FindKey("Proxies", true)) { proxies->FindKey("invis", true); }
		m_pFlat = I::MaterialSystem->CreateMaterial("seo_material_flat", kv);
	}

	if (!m_pShaded)
	{
		auto* kv = new KeyValues("VertexLitGeneric");
		kv->SetString("$basetexture", "vgui/white_additive");
		//kv->SetString("$bumpmap", "models/player/shared/shared_normal");
		kv->SetString("$selfillum", "1");
		kv->SetString("$selfillumFresnel", "1");
		kv->SetString("$selfillumFresnelMinMaxExp", "[0.1 0.5 2]");
		kv->SetString("$cloakPassEnabled", "1");
		kv->SetString("$nodecal", "1");
		kv->SetString("$model", "1");
		if (auto proxies = kv->FindKey("Proxies", true)) { proxies->FindKey("invis", true); }
		m_pShaded = I::MaterialSystem->CreateMaterial("seo_material_shaded", kv);
	}

	if (!m_pFlatNoInvis)
	{
		auto* kv = new KeyValues("VertexLitGeneric");
		kv->SetString("$basetexture", "vgui/white_additive");
		kv->SetString("$bumpmap", "vgui/white_additive");
		kv->SetString("$selfillum", "1");
		kv->SetString("$selfillumFresnel", "1");
		kv->SetString("$selfillumFresnelMinMaxExp", "[0.4999 0.5 1]");
		kv->SetString("$nodecal", "1");
		kv->SetString("$model", "1");
		m_pFlatNoInvis = I::MaterialSystem->CreateMaterial("seo_material_flat_no_invis", kv);
	}

	if (!m_pShadedNoInvis)
	{
		auto* kv = new KeyValues("VertexLitGeneric");
		kv->SetString("$basetexture", "vgui/white_additive");
		//kv->SetString("$bumpmap", "models/player/shared/shared_normal");
		kv->SetString("$selfillum", "1");
		kv->SetString("$selfillumFresnel", "1");
		kv->SetString("$selfillumFresnelMinMaxExp", "[0.1 0.5 2]");
		kv->SetString("$nodecal", "1");
		kv->SetString("$model", "1");
		m_pShadedNoInvis = I::MaterialSystem->CreateMaterial("seo_material_shaded_no_invis", kv);
	}
}

void CMaterials::DrawEntity(C_BaseEntity* pEntity)
{
	SetModelStencilForOutlines(pEntity);

	m_bRendering = true;

	pEntity->DrawModel(STUDIO_RENDER);

	m_mapDrawnEntities[pEntity] = true;

	m_bRendering = false;
}

void CMaterials::RunLagRecords()
{
	auto pRenderContext = I::MaterialSystem->GetRenderContext();

	if (!pRenderContext || !CFG::Materials_Players_Active || CFG::Materials_Players_Ignore_LagRecords)
		return;

	auto pLocal = H::EntityCache.GetLocal();

	if (!pLocal)
		return;

	auto pWeapon = H::EntityCache.GetWeapon();

	if (!pWeapon)
		return;

	m_bRenderingOriginalMat = false;

	I::RenderView->SetColorModulation(1.0f, 1.0f, 1.0f);

	I::ModelRender->ForcedMaterialOverride(CFG::Materials_Players_LagRecords_Style == 0 ? m_pFlatNoInvis : m_pShadedNoInvis);

	if (CFG::Materials_Players_No_Depth)
		pRenderContext->DepthRange(0.0f, 0.2f);

	for (auto pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ENEMIES))
	{
		if (!pEntity)
			continue;

		auto pPlayer = pEntity->As<C_CSPlayer>();

		if (pPlayer->deadflag())
			continue;

		int nRecords = 0;

		if (!F::LagRecords.HasRecords(pPlayer, &nRecords))
			continue;

		if (nRecords <= 0)
			continue;

		if (CFG::Materials_Players_LagRecords_Style == 0)
		{
			for (int n = 1; n < nRecords; n++)
			{
				auto pRecord = F::LagRecords.GetRecord(pPlayer, n, true);

				if (!pRecord || !Util::IsOnScreen(pLocal, pRecord->m_vAbsOrigin) || !F::LagRecords.DiffersFromCurrent(pRecord))
					continue;

				I::RenderView->SetBlend(U::Math.RemapValClamped(static_cast<float>(n), 1.0f, static_cast<float>(nRecords), 0.1f, 0.001f));

				F::LagRecordMatrixHelper.Set(pRecord);
				m_bRendering = true;
				pPlayer->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);
				m_bRendering = false;
				F::LagRecordMatrixHelper.Restore();
			}
		}

		else
		{
			auto pRecord = F::LagRecords.GetRecord(pPlayer, nRecords - 1, true);

			if (!pRecord || !Util::IsOnScreen(pLocal, pRecord->m_vAbsOrigin) || !F::LagRecords.DiffersFromCurrent(pRecord))
				continue;

			I::RenderView->SetBlend(1.0f);

			F::LagRecordMatrixHelper.Set(pRecord);
			m_bRendering = true;
			pPlayer->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);
			m_bRendering = false;
			F::LagRecordMatrixHelper.Restore();
		}
	}

	I::ModelRender->ForcedMaterialOverride(nullptr);

	if (CFG::Materials_Players_No_Depth)
		pRenderContext->DepthRange(0.0f, 1.0f);

	I::RenderView->SetBlend(1.0f);
}

void CMaterials::Run()
{
	Initialize();

	if (!m_mapDrawnEntities.empty())
		m_mapDrawnEntities.clear();

	if (!CFG::Materials_Active || I::EngineVGui->IsGameUIVisible())
		return;

	if (CFG::Visuals_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
		return;

	auto pRenderContext = I::MaterialSystem->GetRenderContext();

	if (!pRenderContext)
		return;

	auto pLocal = H::EntityCache.GetLocal();

	if (!pLocal)
		return;

	RunLagRecords();

	auto GetMaterial = [&](int nIndex) -> IMaterial*
		{
			//don't forget to change me if more materials are added!
			m_bRenderingOriginalMat = nIndex == 0 || nIndex > 5;

			switch (nIndex)
			{
			case 0: return nullptr;
			case 1: return m_pFlat;
			case 2: return m_pShaded;
			default: return nullptr;
			}
		};

	if (CFG::Materials_Players_Active)
	{
		I::RenderView->SetColorModulation(1.0f, 1.0f, 1.0f);

		auto pMaterial = GetMaterial(CFG::Materials_Players_Material);

		if (pMaterial)
			I::ModelRender->ForcedMaterialOverride(pMaterial);

		if (CFG::Materials_Players_Alpha < 1.0f)
			I::RenderView->SetBlend(CFG::Materials_Players_Alpha);

		if (CFG::Materials_Players_No_Depth)
			pRenderContext->DepthRange(0.0f, 0.2f);

		for (auto pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ALL))
		{
			if (!pEntity)
				continue;

			auto pPlayer = pEntity->As<C_CSPlayer>();

			if (pPlayer->deadflag())
				continue;

			bool bIsLocal = pPlayer == pLocal;
			bool bIsFriend = false;

			if (CFG::Materials_Players_Ignore_Local && bIsLocal)
				continue;

			if (CFG::Materials_Players_Ignore_Friends && bIsFriend)
				continue;

			if (!bIsLocal)
			{
				if (!bIsFriend)
				{
					if (CFG::Materials_Players_Ignore_Teammates && pPlayer->m_iTeamNum() == pLocal->m_iTeamNum())
						continue;

					if (CFG::Materials_Players_Ignore_Enemies && pPlayer->m_iTeamNum() != pLocal->m_iTeamNum())
						continue;
				}
			}

			if (!Util::IsOnScreen(pLocal, pPlayer))
				continue;

			auto Color = Util::GetEntityColor(pLocal, pPlayer, CFG::Colors_Relative);

			if (pMaterial)
				I::RenderView->SetColorModulation(Color);

			DrawEntity(pPlayer);

			C_BaseEntity* pAttach = pPlayer->FirstMoveChild();

			for (int n = 0; n < 32; n++)
			{
				if (!pAttach)
					break;

				if (pAttach->ShouldDraw())
					DrawEntity(pAttach);

				pAttach = pAttach->NextMovePeer();
			}
		}

		if (pMaterial)
			I::ModelRender->ForcedMaterialOverride(nullptr);

		if (CFG::Materials_Players_Alpha < 1.0f)
			I::RenderView->SetBlend(1.0f);

		if (CFG::Materials_Players_No_Depth)
			pRenderContext->DepthRange(0.0f, 1.0f);
	}

	if (CFG::Materials_World_Active)
	{
		I::RenderView->SetColorModulation(1.0f, 1.0f, 1.0f);

		auto pMaterial = GetMaterial(CFG::Materials_World_Material);

		if (pMaterial)
			I::ModelRender->ForcedMaterialOverride(pMaterial);

		if (CFG::Materials_World_Alpha < 1.0f)
			I::RenderView->SetBlend(CFG::Materials_World_Alpha);

		if (CFG::Materials_World_No_Depth)
			pRenderContext->DepthRange(0.0f, 0.2f);

		if (!CFG::Materials_World_Ignore_PlantedC4)
		{
			auto Color = CFG::Colors_PlantedC4;

			for (auto pEntity : H::EntityCache.GetGroup(EGroupType::WORLD_C4PLANTED))
			{
				if (!pEntity || !pEntity->ShouldDraw() || !Util::IsOnScreen(pLocal, pEntity))
					continue;

				if (pMaterial)
					I::RenderView->SetColorModulation(Color);

				DrawEntity(pEntity);
			}
		}

		if (!CFG::Materials_World_Ignore_DroppedWeapons)
		{
			auto Color = CFG::Colors_DroppedWeapons;

			for (auto pEntity : H::EntityCache.GetGroup(EGroupType::WORLD_WEAPONS))
			{
				if (!pEntity || !pEntity->ShouldDraw() || !Util::IsOnScreen(pLocal, pEntity))
					continue;

				if (pMaterial)
					I::RenderView->SetColorModulation(Color);

				DrawEntity(pEntity);
			}
		}

		/*bool bIgnoringAllProjectiles = CFG::Materials_World_Ignore_LocalProjectiles
			&& CFG::Materials_World_Ignore_EnemyProjectiles
			&& CFG::Materials_World_Ignore_TeammateProjectiles;

		if (!bIgnoringAllProjectiles)
		{
			for (auto pEntity : H::EntityCache.GetGroup(EGroupType::PROJECTILES_ALL))
			{
				if (!pEntity || !pEntity->ShouldDraw())
					continue;

				bool bIsLocal = F::VisualUtils->IsEntityOwnedBy(pEntity, pLocal);

				if (CFG::Materials_World_Ignore_LocalProjectiles && bIsLocal)
					continue;

				if (!bIsLocal)
				{
					if (CFG::Materials_World_Ignore_EnemyProjectiles && pEntity->m_iTeamNum() != pLocal->m_iTeamNum())
						continue;

					if (CFG::Materials_World_Ignore_TeammateProjectiles && pEntity->m_iTeamNum() == pLocal->m_iTeamNum())
						continue;
				}

				if (!F::VisualUtils->IsOnScreen(pLocal, pEntity))
					continue;

				auto Color = F::VisualUtils->GetEntityColor(pLocal, pEntity);

				if (pMaterial && pMaterial != m_pGlow)
					I::RenderView->SetColorModulation(Color);

				if (pMaterial == m_pGlow)
					m_pGlowEnvmapTint->SetVecValue(ColorUtils::ToFloat(Color.r), ColorUtils::ToFloat(Color.g), ColorUtils::ToFloat(Color.b));

				DrawEntity(pEntity);
			}
		}*/

		if (pMaterial)
			I::ModelRender->ForcedMaterialOverride(nullptr);

		if (CFG::Materials_World_Alpha < 1.0f)
			I::RenderView->SetBlend(1.0f);

		if (CFG::Materials_World_No_Depth)
			pRenderContext->DepthRange(0.0f, 1.0f);
	}
}

void CMaterials::CleanUp()
{
	m_bCleaningUp = true;

	/*if (m_pFlat)
	{
		m_pFlat->DecrementReferenceCount();
		m_pFlat->DeleteIfUnreferenced();
		m_pFlat = nullptr;
	}

	if (m_pShaded)
	{
		m_pShaded->DecrementReferenceCount();
		m_pShaded->DeleteIfUnreferenced();
		m_pShaded = nullptr;
	}

	if (m_pFlatNoInvis)
	{
		m_pFlatNoInvis->DecrementReferenceCount();
		m_pFlatNoInvis->DeleteIfUnreferenced();
		m_pFlatNoInvis = nullptr;
	}

	if (m_pShadedNoInvis)
	{
		m_pShadedNoInvis->DecrementReferenceCount();
		m_pShadedNoInvis->DeleteIfUnreferenced();
		m_pShadedNoInvis = nullptr;
	}*/

	m_bCleaningUp = false;
}