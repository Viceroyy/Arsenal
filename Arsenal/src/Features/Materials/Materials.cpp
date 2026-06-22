#include "Materials.h"
#include "../Backtrack/Backtrack.h"

void SetModelStencilForOutlines(C_BaseEntity* pEntity)
{
	IMatRenderContext* pRenderContext = I::MaterialSystem->GetRenderContext();

	if (!pRenderContext)
		return;

	const auto pLocal = H::Entities.GetLocal();

	if (!pLocal)
		return;

	pRenderContext->SetStencilEnable(false);
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

		m_pGlossy = I::MaterialSystem->CreateMaterial("seo_material_glossy", kv);
	}

	if (!m_pGlow)
	{
		auto* kv = new KeyValues("VertexLitGeneric");
		kv->SetString("$basetexture", "vgui/white_additive");
		kv->SetString("$bumpmap", "models/player/shared/shared_normal");
		kv->SetString("$envmap", "effects/saxxy_gold");
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

		m_pPlastic = I::MaterialSystem->CreateMaterial("seo_material_plastic", kv);
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

void CMaterials::RunLagRecords(const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo)
{
	static auto ModelRender_DrawModelExecute = U::Hooks.m_mHooks["IVModelRender_DrawModelExecute"];
	auto pRenderContext = I::MaterialSystem->GetRenderContext();
	if (!ModelRender_DrawModelExecute || !pRenderContext)
		return;

	if (!CFG::Materials_Players_Active || CFG::Materials_Players_Ignore_LagRecords)
		return;

	const auto pLocal = H::Entities.GetLocal();

	if (!pLocal)
		return;

	const auto pWeapon = H::Entities.GetWeapon();

	if (!pWeapon)
		return;

	m_bRenderingOriginalMat = false;

	I::RenderView->SetColorModulation(1.0f, 1.0f, 1.0f);

	I::ModelRender->ForcedMaterialOverride(CFG::Materials_Players_LagRecords_Style == 0 ? m_pFlat : m_pShaded);

	if (CFG::Materials_Players_No_Depth)
		pRenderContext->DepthRange(0.0f, 0.2f);

	auto drawModel = [&](Vector& vOrigin, const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4_t* pBoneToWorld, float flBlend)
		{
			float flOriginalBlend = I::RenderView->GetBlend();
			I::RenderView->SetBlend(flBlend * flOriginalBlend);
			ModelRender_DrawModelExecute->Call<void>(I::ModelRender, pState, pInfo, pBoneToWorld);
			I::RenderView->SetBlend(flOriginalBlend);
		};

	for (const auto pEntity : H::Entities.GetGroup(EGroupType::PLAYERS_ENEMIES))
	{
		if (!pEntity)
			continue;

		const auto pPlayer = pEntity->As<C_CSPlayer>();

		if (pPlayer->deadflag())
			continue;

		std::vector<TickRecord*> vRecords = {};
		if (!F::Backtrack.GetRecords(pEntity, vRecords))
			continue;
		
		vRecords = F::Backtrack.GetValidRecords(vRecords);
		if (!vRecords.size())
			continue;

		if (CFG::Materials_Players_LagRecords_Style == 0)
		{
			for (auto pRecord : vRecords)
			{
				if (!pRecord || !Util::IsOnScreen(pLocal, pRecord->m_vOrigin))
					continue;

				if (float flBlend = U::Math.RemapValClamped(pEntity->GetAbsOrigin().DistTo(pRecord->m_vOrigin), 1.f, 24.f, 0.f, 1.f))
				{
					//m_bRendering = true;
					drawModel(pRecord->m_vOrigin, pState, pInfo, pRecord->m_BoneMatrix.m_aBones, flBlend);
					//m_bRendering = false;
				}
			}
		}
		else
		{
			auto vLastRec = vRecords.back();

			if (!vLastRec || !Util::IsOnScreen(pLocal, vLastRec->m_vOrigin))
				continue;

			//m_bRendering = true;
			drawModel(vLastRec->m_vOrigin, pState, pInfo, vLastRec->m_BoneMatrix.m_aBones, 1.0f);
			//m_bRendering = false;
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
	{
		return;
	}

	const auto pRenderContext = I::MaterialSystem->GetRenderContext();

	if (!pRenderContext)
		return;

	const auto pLocal = H::Entities.GetLocal();

	if (!pLocal)
		return;

	m_pGlowSelfillumTint->SetVecValue(0.03f, 0.03f, 0.03f);

	//RunLagRecords();

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

		for (const auto pEntity : H::Entities.GetGroup(EGroupType::PLAYERS_ALL))
		{
			if (!pEntity)
				continue;

			const auto pPlayer = pEntity->As<C_CSPlayer>();

			if (pPlayer->deadflag())
				continue;

			const bool bIsLocal = pPlayer == pLocal;
			const bool bIsFriend = false;

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

	/*if (CFG::Materials_World_Active)
	{
		I::RenderView->SetColorModulation(1.0f, 1.0f, 1.0f);

		const auto pMaterial = GetMaterial(CFG::Materials_World_Material);

		if (pMaterial)
			I::ModelRender->ForcedMaterialOverride(pMaterial);

		if (CFG::Materials_World_Alpha < 1.0f)
			I::RenderView->SetBlend(CFG::Materials_World_Alpha);

		if (CFG::Materials_World_No_Depth)
			pRenderContext->DepthRange(0.0f, 0.2f);

		if (pMaterial)
			I::ModelRender->ForcedMaterialOverride(nullptr);

		if (CFG::Materials_World_Alpha < 1.0f)
			I::RenderView->SetBlend(1.0f);

		if (CFG::Materials_World_No_Depth)
			pRenderContext->DepthRange(0.0f, 1.0f);
	}*/
}

void CMaterials::CleanUp()
{
	m_bCleaningUp = true;

	if (m_pFlat)
	{
		m_pFlat->DecrementReferenceCount();
		m_pFlat = nullptr;
	}

	if (m_pShaded)
	{
		m_pShaded->DecrementReferenceCount();
		m_pShaded = nullptr;
	}

	if (m_pGlossy)
	{
		m_pGlossy->DecrementReferenceCount();
		m_pGlossy = nullptr;
	}

	if (m_pGlow)
	{
		m_pGlow->DecrementReferenceCount();
		m_pGlow = nullptr;
		m_pGlowEnvmapTint = nullptr;
		m_pGlowSelfillumTint = nullptr;
	}

	if (m_pPlastic)
	{
		m_pPlastic->DecrementReferenceCount();
		m_pPlastic = nullptr;
	}

	m_bCleaningUp = false;
}
