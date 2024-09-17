#include "Materials.h"
#include "../Backtrack/Backtrack.h"

void SetModelStencilForOutlines(C_BaseEntity* pEntity)
{
	IMatRenderContext* pRenderContext = I::MaterialSystem->GetRenderContext();

	if (!pRenderContext)
		return;

	const auto pLocal = H::EntityCache.GetLocal();

	if (!pLocal)
		return;

	auto IsEntGoingToBeGlowed = [&]()
		{
			if (pEntity->GetClassID() == ECSClientClass::CCSPlayer)
			{
				if (!CFG::Outlines_Players_Active)
					return false;

				const auto pPlayer = pEntity->As<C_CSPlayer>();

				const bool bIsLocal = pPlayer == pLocal;
				const bool bIsFriend = pPlayer->HasPlayerAsFriend();

				if (CFG::Outlines_Players_Ignore_Local && bIsLocal)
					return false;

				if (CFG::Outlines_Players_Ignore_Friends && bIsFriend)
					return false;

				if (!bIsLocal && !bIsFriend)
				{
					if (CFG::Outlines_Players_Ignore_Teammates && pPlayer->m_iTeamNum() == pLocal->m_iTeamNum())
						return false;

					if (CFG::Outlines_Players_Ignore_Enemies && pPlayer->m_iTeamNum() != pLocal->m_iTeamNum())
						return false;
				}
			}

			return true;
		};

	if (!IsEntGoingToBeGlowed())
	{
		pRenderContext->SetStencilEnable(false);
	}

	else
	{
		ShaderStencilState_t state = {};
		state.m_bEnable = true;
		state.m_nReferenceValue = 1;
		state.m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
		state.m_PassOp = STENCILOPERATION_REPLACE;
		state.m_FailOp = STENCILOPERATION_KEEP;
		state.m_ZFailOp = STENCILOPERATION_REPLACE;
		state.SetStencilState(pRenderContext);
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
		if (const auto proxies = kv->FindKey("Proxies", true)) { proxies->FindKey("invis", true); }

		m_pFlat = I::MaterialSystem->CreateMaterial("seo_material_flat", kv);
	}

	if (!m_pShaded)
	{
		auto* kv = new KeyValues("VertexLitGeneric");
		kv->SetString("$basetexture", "vgui/white_additive");
		kv->SetString("$bumpmap", "models/player/shared/shared_normal");
		kv->SetString("$selfillum", "1");
		kv->SetString("$selfillumFresnel", "1");
		kv->SetString("$selfillumFresnelMinMaxExp", "[0.1 0.5 2]");
		kv->SetString("$cloakPassEnabled", "1");
		kv->SetString("$nodecal", "1");
		kv->SetString("$model", "1");
		if (const auto proxies = kv->FindKey("Proxies", true)) { proxies->FindKey("invis", true); }

		m_pShaded = I::MaterialSystem->CreateMaterial("seo_material_shaded", kv);
	}

	if (!m_pGlossy)
	{
		auto* kv = new KeyValues("VertexLitGeneric");
		kv->SetString("$basetexture", "vgui/white_additive");
		kv->SetString("$bumpmap", "models/player/shared/shared_normal");
		kv->SetString("$envmap", "cubemaps/cubemap_sheen002");
		kv->SetString("$envmapfresnel", "1");
		kv->SetString("$phong", "1");
		kv->SetString("$phongfresnelranges", "[0 1 2]");
		kv->SetString("$selfillum", "1");
		kv->SetString("$selfillumFresnel", "1");
		kv->SetString("$selfillumFresnelMinMaxExp", "[0 0.1 1]");
		kv->SetString("$cloakPassEnabled", "1");
		kv->SetString("$nodecal", "1");
		kv->SetString("$model", "1");
		if (const auto proxies = kv->FindKey("Proxies", true)) { proxies->FindKey("invis", true); }

		m_pGlossy = I::MaterialSystem->CreateMaterial("seo_material_glossy", kv);
	}

	if (!m_pGlow)
	{
		auto* kv = new KeyValues("VertexLitGeneric");
		kv->SetString("$basetexture", "vgui/white_additive");
		kv->SetString("$bumpmap", "models/player/shared/shared_normal");
		kv->SetString("$envmap", "effects/saxxy_gold");
		//kv->SetString("$envmap", "skybox/sky_dustbowl_01");
		kv->SetString("$envmapfresnel", "1");
		kv->SetString("$phong", "1");
		kv->SetString("$phongfresnelranges", mat_hdr_level->GetInt() > 1 ? "[0 0.05 0.1]" : "[0 1 2]");
		kv->SetString("$selfillum", "1");
		kv->SetString("$selfillumFresnel", "1");
		kv->SetString("$selfillumFresnelMinMaxExp", "[0.4999 0.5 0]");
		kv->SetString("$envmaptint", "[0 0 0]");
		kv->SetString("$selfillumtint", "[0.03 0.03 0.03]");
		kv->SetString("$cloakPassEnabled", "1");
		kv->SetString("$nodecal", "1");
		kv->SetString("$model", "1");
		if (const auto proxies = kv->FindKey("Proxies", true)) { proxies->FindKey("invis", true); }

		m_pGlow = I::MaterialSystem->CreateMaterial("seo_material_glow", kv);
		m_pGlowEnvmapTint = m_pGlow->FindVar("$envmaptint", nullptr);
		m_pGlowSelfillumTint = m_pGlow->FindVar("$selfillumtint", nullptr);
	}

	if (!m_pPlastic)
	{
		auto* kv = new KeyValues("VertexLitGeneric");
		kv->SetString("$basetexture", "models/player/shared/ice_player");
		kv->SetString("$bumpmap", "models/player/shared/shared_normal");
		kv->SetString("$phong", "1");
		kv->SetString("$phongexponent", "10");
		kv->SetString("$phongboost", "1");
		kv->SetString("$phongfresnelranges", "[0 0 0]");
		kv->SetString("$basemapalphaphongmask", "1");
		kv->SetString("$phongwarptexture", "models/player/shared/ice_player_warp");
		kv->SetString("$cloakPassEnabled", "1");
		kv->SetString("$nodecal", "1");
		kv->SetString("$model", "1");
		if (const auto proxies = kv->FindKey("Proxies", true)) { proxies->FindKey("invis", true); }

		m_pPlastic = I::MaterialSystem->CreateMaterial("seo_material_plastic", kv);
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
		kv->SetString("$bumpmap", "models/player/shared/shared_normal");
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
	//const auto pRenderContext = I::MaterialSystem->GetRenderContext();

	//if (!pRenderContext || !CFG::Materials_Players_Active || CFG::Materials_Players_Ignore_LagRecords)
	//	return;

	//const auto pLocal = H::EntityCache.GetLocal();

	//if (!pLocal)
	//	return;

	//const auto pWeapon = H::EntityCache.GetWeapon();

	//if (!pWeapon)
	//	return;

	//m_bRenderingOriginalMat = false;

	//I::RenderView->SetColorModulation(1.0f, 1.0f, 1.0f);

	//I::ModelRender->ForcedMaterialOverride(CFG::Materials_Players_LagRecords_Style == 0 ? m_pFlatNoInvis : m_pShadedNoInvis);

	//if (CFG::Materials_Players_No_Depth)
	//	pRenderContext->DepthRange(0.0f, 0.2f);

	//for (const auto pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ENEMIES))
	//{
	//	if (!pEntity)
	//		continue;

	//	const auto pPlayer = pEntity->As<C_CSPlayer>();

	//	if (pPlayer->deadflag())
	//		continue;

	//	auto pRecords = F::Backtrack.GetRecords(pEntity);
	//	auto vRecords = F::Backtrack.GetValidRecords(pRecords);
	//	if (!vRecords.size())
	//		continue;

	//	if (CFG::Materials_Players_LagRecords_Style == 0)
	//	{
	//		for (auto& record : vRecords)
	//		{
	//			if (pEntity->GetAbsOrigin().DistTo(record.vOrigin) < 0.1f || !Util::IsOnScreen(pLocal, record.vOrigin))
	//				continue;

	//			//I::RenderView->SetBlend(U::Math.RemapValClamped(static_cast<float>(vRecords.), 1.0f, static_cast<float>(nRecords), 0.1f, 0.001f));

	//			I::ModelRender->ForcedMaterialOverride(material ? material : nullptr);
	//			ModelRender_DrawModelExecute->Original<void(__thiscall*)(void*, void*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*)>()(ecx, edx, pState, pInfo, pBoneToWorld);

	//			F::LagRecordMatrixHelper->Set(pRecord);
	//			m_bRendering = true;
	//			const float flOldInvisibility = pPlayer->m_flInvisibility();
	//			pPlayer->m_flInvisibility() = 0.0f;
	//			pPlayer->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);
	//			pPlayer->m_flInvisibility() = flOldInvisibility;
	//			m_bRendering = false;
	//			F::LagRecordMatrixHelper->Restore();
	//		}
	//	}
	//	else
	//	{
	//		const auto pRecord = F::LagRecords->GetRecord(pPlayer, nRecords - 1, true);

	//		if (!pRecord || !F::VisualUtils->IsOnScreenNoEntity(pLocal, pRecord->AbsOrigin) || !F::LagRecords->DiffersFromCurrent(pRecord))
	//			continue;

	//		I::RenderView->SetBlend(1.0f);

	//		F::LagRecordMatrixHelper->Set(pRecord);
	//		m_bRendering = true;
	//		const float flOldInvisibility = pPlayer->m_flInvisibility();
	//		pPlayer->m_flInvisibility() = 0.0f;
	//		pPlayer->DrawModel(STUDIO_RENDER | STUDIO_NOSHADOWS);
	//		pPlayer->m_flInvisibility() = flOldInvisibility;
	//		m_bRendering = false;
	//		F::LagRecordMatrixHelper->Restore();
	//	}
	//}

	//I::ModelRender->ForcedMaterialOverride(nullptr);

	//if (CFG::Materials_Players_No_Depth)
	//	pRenderContext->DepthRange(0.0f, 1.0f);

	//I::RenderView->SetBlend(1.0f);
}

void CMaterials::Run()
{
	Initialize();

	if (!m_mapDrawnEntities.empty())
		m_mapDrawnEntities.clear();

	if (!CFG::Materials_Active || I::EngineVGui->IsGameUIVisible())
		return;

	if (CFG::Visuals_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
	{
		return;
	}

	const auto pRenderContext = I::MaterialSystem->GetRenderContext();

	if (!pRenderContext)
		return;

	const auto pLocal = H::EntityCache.GetLocal();

	if (!pLocal)
		return;

	m_pGlowSelfillumTint->SetVecValue(0.03f, 0.03f, 0.03f);

	RunLagRecords();

	auto GetMaterial = [&](int nIndex) -> IMaterial* {
		//don't forget to change me if more materials are added!
		m_bRenderingOriginalMat = nIndex == 0 || nIndex > 5;

		switch (nIndex)
		{
		case 0: return nullptr;
		case 1: return m_pFlat;
		case 2: return m_pShaded;
		case 3: return m_pGlossy;
		case 4: return m_pGlow;
		case 5: return m_pPlastic;
		default: return nullptr;
		}
		};

	if (CFG::Materials_Players_Active)
	{
		I::RenderView->SetColorModulation(1.0f, 1.0f, 1.0f);

		const auto pMaterial = GetMaterial(CFG::Materials_Players_Material);

		if (pMaterial)
			I::ModelRender->ForcedMaterialOverride(pMaterial);

		if (CFG::Materials_Players_Alpha < 1.0f)
			I::RenderView->SetBlend(CFG::Materials_Players_Alpha);

		if (CFG::Materials_Players_No_Depth)
			pRenderContext->DepthRange(0.0f, 0.2f);

		for (const auto pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ALL))
		{
			if (!pEntity)
				continue;

			const auto pPlayer = pEntity->As<C_CSPlayer>();

			if (pPlayer->deadflag())
				continue;

			const bool bIsLocal = pPlayer == pLocal;
			const bool bIsFriend = pPlayer->HasPlayerAsFriend();

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

			const auto entColor = Util::GetEntityColor(pLocal, pPlayer, CFG::Colors_Relative);

			if (pMaterial && pMaterial != m_pGlow)
				I::RenderView->SetColorModulation(entColor);

			if (pMaterial == m_pGlow)
				m_pGlowEnvmapTint->SetVecValue(static_cast<float>(entColor.r) / 255.0f, static_cast<float>(entColor.g) / 255.0f, static_cast<float>(entColor.b) / 255.0f);

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

		const auto pMaterial = GetMaterial(CFG::Materials_World_Material);

		if (pMaterial)
			I::ModelRender->ForcedMaterialOverride(pMaterial);

		if (CFG::Materials_World_Alpha < 1.0f)
			I::RenderView->SetBlend(CFG::Materials_World_Alpha);

		if (CFG::Materials_World_No_Depth)
			pRenderContext->DepthRange(0.0f, 0.2f);

		if (!CFG::Materials_World_Ignore_PlantedC4)
		{
			const auto color = CFG::Colors_PlantedC4;

			for (const auto pEntity : H::EntityCache.GetGroup(EGroupType::WORLD_C4PLANTED))
			{
				if (!pEntity || !pEntity->ShouldDraw() || !Util::IsOnScreen(pLocal, pEntity))
					continue;

				if (pMaterial && pMaterial != m_pGlow)
					I::RenderView->SetColorModulation(color);

				if (pMaterial == m_pGlow)
					m_pGlowEnvmapTint->SetVecValue(static_cast<float>(color.r) / 255.0f, static_cast<float>(color.g) / 255.0f, static_cast<float>(color.b) / 255.0f);

				DrawEntity(pEntity);
			}
		}

		if (!CFG::Materials_World_Ignore_DroppedWeapons)
		{
			const auto color = CFG::Colors_DroppedWeapons;

			for (const auto pEntity : H::EntityCache.GetGroup(EGroupType::WORLD_WEAPONS))
			{
				if (!pEntity || !pEntity->ShouldDraw() || !Util::IsOnScreen(pLocal, pEntity))
					continue;

				if (pMaterial && pMaterial != m_pGlow)
					I::RenderView->SetColorModulation(color);

				if (pMaterial == m_pGlow)
					m_pGlowEnvmapTint->SetVecValue(static_cast<float>(color.r) / 255.0f, static_cast<float>(color.g) / 255.0f, static_cast<float>(color.b) / 255.0f);

				DrawEntity(pEntity);
			}
		}

		/*const bool bIgnoringAllProjectiles = CFG::Materials_World_Ignore_LocalProjectiles
			&& CFG::Materials_World_Ignore_EnemyProjectiles
			&& CFG::Materials_World_Ignore_TeammateProjectiles;

		if (!bIgnoringAllProjectiles)
		{
			for (const auto pEntity : H::EntityCache.GetGroup(EGroupType::PROJECTILES_ALL))
			{
				if (!pEntity || !pEntity->ShouldDraw())
					continue;

				const bool bIsLocal = F::VisualUtils->IsEntityOwnedBy(pEntity, pLocal);

				if (CFG::Materials_World_Ignore_LocalProjectiles && bIsLocal)
					continue;

				if (!bIsLocal)
				{
					if (CFG::Materials_World_Ignore_EnemyProjectiles && pEntity->m_iTeamNum() != pLocal->m_iTeamNum())
						continue;

					if (CFG::Materials_World_Ignore_TeammateProjectiles && pEntity->m_iTeamNum() == pLocal->m_iTeamNum())
						continue;
				}

				if (!Util::IsOnScreen(pLocal, pEntity))
					continue;

				const auto color = Util::GetEntityColor(pLocal, pEntity);

				if (pMaterial && pMaterial != m_pGlow)
					I::RenderView->SetColorModulation(color);

				if (pMaterial == m_pGlow)
					m_pGlowEnvmapTint->SetVecValue(ColorUtils::ToFloat(color.r), ColorUtils::ToFloat(color.g), ColorUtils::ToFloat(color.b));

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

	if (m_pFlat)
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

	if (m_pGlossy)
	{
		m_pGlossy->DecrementReferenceCount();
		m_pGlossy->DeleteIfUnreferenced();
		m_pGlossy = nullptr;
	}

	if (m_pGlow)
	{
		m_pGlow->DecrementReferenceCount();
		m_pGlow->DeleteIfUnreferenced();
		m_pGlow = nullptr;
		m_pGlowEnvmapTint = nullptr;
		m_pGlowSelfillumTint = nullptr;
	}

	if (m_pPlastic)
	{
		m_pPlastic->DecrementReferenceCount();
		m_pPlastic->DeleteIfUnreferenced();
		m_pPlastic = nullptr;
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
	}

	m_bCleaningUp = false;
}
