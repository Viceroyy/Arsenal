#include "ESP.h"
#include "../CFG.h"
#include "../Players/Players.h"
#include "../Backtrack/Backtrack.h"
#include "../../SDK/Entities/C_PlantedC4.h"

void CFeatures_ESP::Run()
{
	if (!CFG::ESP_Enabled || I::EngineVGui->IsGameUIVisible())
		return;

	if (auto pLocal = H::EntityCache.GetLocal())
	{
		DrawWorld();
		DrawPlayers(pLocal);
	}
}

void CFeatures_ESP::DrawPlayers(C_CSPlayer* pLocal)
{
	if (!CFG::ESP_Players_Enabled)
		return;

	auto pResource = H::EntityCache.GetPR();
	if (!pResource)
		return;

	for (auto pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ALL))
	{
		auto pPlayer = pEntity->As<C_CSPlayer>();
		if (pPlayer->deadflag())
			continue;

		bool bIsLocal = pPlayer == pLocal;
		if ((CFG::ESP_Players_Ignore_Local && bIsLocal) || (!I::Input->CAM_IsThirdPerson() && bIsLocal))
			continue;

		if (!bIsLocal)
		{
			if (CFG::ESP_Players_Ignore_Teammates && pPlayer->m_iTeamNum() == pLocal->m_iTeamNum())
				continue;

			if (CFG::ESP_Players_Ignore_Enemies && pPlayer->m_iTeamNum() != pLocal->m_iTeamNum())
				continue;
		}

		I::MatSystemSurface->DrawSetAlphaMultiplier(pPlayer->IsDormant() ? 0.7f : CFG::ESP_Alpha);

		int x = 0, y = 0, w = 0, h = 0;
		if (!GetDynamicBounds(pPlayer, x, y, w, h))
			continue;

		const int nIndex = pPlayer->entindex();
		int lOffset = 0, rOffset = 0, bOffset = 2, tOffset = 0;
		const EFonts& fFont = EFonts::ESP;
		const int iMaxHealth = pPlayer->GetMaxHealth(), iHealth = pPlayer->IsDormant() ? pResource->GetHealth(pPlayer->entindex()) : pPlayer->GetHealth();

		const Color_t clrTeam = Util::GetEntityColor(pLocal, pPlayer, CFG::Colors_Relative);

		if (CFG::ESP_Players_Box)
		{
			H::Draw.OutlinedRect(x, y, w, h, clrTeam);
			H::Draw.OutlinedRect(x - 1, y - 1, w + 2, h + 2, COLOR_BLACK); // Outline
			H::Draw.OutlinedRect(x + 1, y + 1, w - 2, h - 2, COLOR_BLACK); // Inline
		}

		if (CFG::ESP_Players_HealthBar)
		{
			float flRatio = std::clamp(float(iHealth) / iMaxHealth, 0.f, 1.f);
			Color_t cColor = Color_t(255, 0, 0, 255).Lerp(Color_t(0, 200, 125, 255), flRatio);
			H::Draw.RectPercent(x - 6, y, 2, h, flRatio, cColor, COLOR_BLACK, ALIGN_BOTTOM, true);

			if (iHealth > iMaxHealth)
			{
				const float flMaxOverheal = floorf(iMaxHealth / 10.f) * 5;
				flRatio = std::clamp((iHealth - iMaxHealth) / flMaxOverheal, 0.f, 1.f);
				cColor = Color_t(69, 170, 242, 255);
				H::Draw.RectPercent(x - 6, y, 2, h, flRatio, cColor, { 0, 0, 0, 0 }, ALIGN_BOTTOM, true);
			}

			lOffset += 5;
		}

		if (CFG::ESP_Players_HealthText)
		{
			H::Draw.String(fFont, x - 5 - lOffset, y + h - h * (float(std::min(iHealth, iMaxHealth)) / iMaxHealth) - 2, iHealth > iMaxHealth ? Color_t(69, 170, 242, 255) : COLOR_WHITE, ALIGN_TOPRIGHT, "%d HP", iHealth);
		}

		if (CFG::ESP_Players_ArmorBar && pPlayer->m_ArmorValue())
		{
			const float flMaxArmor = 100.f;
			const float flArmor = pPlayer->m_ArmorValue();
			float ratio = flArmor / flMaxArmor;

			H::Draw.RectPercent(x, y + h + 4, w, 2, ratio, Color_t(69, 170, 242, 255), COLOR_BLACK, ALIGN_LEFT, true);

			bOffset += 5;
		}

		if (CFG::ESP_Players_ArmorText && pPlayer->m_ArmorValue())
		{
			H::Draw.String(fFont, x + w + 4, y + h, COLOR_WHITE, ALIGN_TOPLEFT, "%d AP", pPlayer->m_ArmorValue());
		}

		if (CFG::ESP_Players_Money)
		{
			H::Draw.String(fFont, x + w + 4, y + rOffset, COLOR_GREEN, ALIGN_TOPLEFT, "$%d", pPlayer->m_iAccount());
			rOffset += H::Draw.GetFontHeight(fFont);
		}

		if (CFG::ESP_Players_WeaponText)
		{
			if (const auto& pWeapon = pPlayer->GetActiveCSWeapon())
			{
				H::Draw.String(fFont, x + (w / 2), y + h + bOffset, COLOR_WHITE, ALIGN_TOP, "%ls [%d/%d]", Util::GetWeaponName(pWeapon->GetWeaponID()).data(), pWeapon->m_iClip1(), pWeapon->GetMaxClip1());
				bOffset += H::Draw.GetFontHeight(fFont);
			}
		}

		/*if (!pPlayer->IsDormant() && pPlayer != pLocal)
		{
			if (F::Backtrack.mLagCompensation[pPlayer])
			{
				H::Draw.String(fFont, x + w + 4, y + rOffset, { 255, 95, 95, 255 }, ALIGN_TOPLEFT, "LAGCOMP");
				rOffset += H::Draw.GetFontHeight(fFont);
			}
		}

		const auto& pRecords = F::Backtrack.GetRecords(pEntity);
		auto vRecords = F::Backtrack.GetValidRecords(pRecords);
		if (vRecords.size())
		{
			auto vLastRec = vRecords.end() - 1;
			if (vLastRec != vRecords.end() && pEntity->GetAbsOrigin().DistTo(vLastRec->vOrigin) > 0.1f)
			{
				Vector2D vScreenPos;
				if (H::Draw.WorldPosToScreenPos(vLastRec->vOrigin, vScreenPos))
				{
					H::Draw.Circle(vScreenPos.x, vScreenPos.y, 2, 360, COLOR_GREEN);
				}
			}
		}*/

		player_info_t pi;
		if (CFG::ESP_Players_Name && I::EngineClient->GetPlayerInfo(nIndex, &pi))
		{
			tOffset += H::Draw.GetFontHeight(EFonts::NAME) + 2;
			H::Draw.String(EFonts::NAME, x + (w / 2), y - tOffset, COLOR_WHITE, ALIGN_TOP, Util::ConvertUtf8ToWide(pi.name).data());
		}
	}
	
	I::MatSystemSurface->DrawSetAlphaMultiplier(1.f);
}

void CFeatures_ESP::DrawWorld()
{
	if (!CFG::ESP_World_Enabled)
		return;

	const EFonts& fFont = EFonts::NAME;
	const int nTextTopOffset = (H::Draw.GetFontHeight(fFont) + 2);

	I::MatSystemSurface->DrawSetAlphaMultiplier(CFG::ESP_Alpha);

	if (!CFG::ESP_World_Ignore_PlantedC4)
	{
		for (auto pC4 : H::EntityCache.GetGroup(EGroupType::WORLD_C4PLANTED))
		{
			int x = 0, y = 0, w = 0, h = 0;
			if (!GetDynamicBounds(pC4, x, y, w, h))
				continue;

			if (CFG::ESP_World_Box)
			{
				H::Draw.OutlinedRect(x, y, w, h, CFG::Colors_PlantedC4);
				H::Draw.OutlinedRect(x - 1, y - 1, w + 2, h + 2, COLOR_BLACK); // Outline
				H::Draw.OutlinedRect(x + 1, y + 1, w - 2, h - 2, COLOR_BLACK); // Inline
			}

			if (CFG::ESP_World_Name)
			{
				H::Draw.String(fFont, x + (w / 2), y - nTextTopOffset, CFG::Colors_PlantedC4, ALIGN_TOP, "C4");
			}
		}
	}

	if (!CFG::ESP_World_Ignore_DroppedWeapons)
	{
		for (auto pWeapons : H::EntityCache.GetGroup(EGroupType::WORLD_WEAPONS))
		{
			int x = 0, y = 0, w = 0, h = 0;
			if (!GetDynamicBounds(pWeapons, x, y, w, h))
				continue;

			auto pWeapon = pWeapons->As<C_WeaponCSBase>();

			if (CFG::ESP_World_Box)
			{
				H::Draw.OutlinedRect(x, y, w, h, CFG::Colors_DroppedWeapons);
				H::Draw.OutlinedRect(x - 1, y - 1, w + 2, h + 2, COLOR_BLACK); // Outline
				H::Draw.OutlinedRect(x + 1, y + 1, w - 2, h - 2, COLOR_BLACK); // Inline
			}

			if (CFG::ESP_World_Name)
			{
				H::Draw.String(fFont, x + w / 2, y - nTextTopOffset, COLOR_WHITE, ALIGN_TOP, "%ls [%d/%d]", Util::GetWeaponName(pWeapon->GetWeaponID()).data(), pWeapon->m_iClip1(), pWeapon->GetMaxClip1());
			}
		}
	}

	I::MatSystemSurface->DrawSetAlphaMultiplier(1.f);
}

bool CFeatures_ESP::GetDynamicBounds(C_BaseEntity* pEntity, int& x, int& y, int& w, int& h)
{
	if (!pEntity)
		return false;

	matrix3x4_t transform;
	U::Math.AngleMatrix(pEntity->GetRenderAngles(), transform);
	U::Math.MatrixSetColumn(pEntity->GetRenderOrigin(), 3, transform);

	float flLeft, flRight, flTop, flBottom;
	if (!Util::IsOnScreen(pEntity, transform, &flLeft, &flRight, &flTop, &flBottom))
		return false;

	x = flLeft + (flRight - flLeft) / 8.f;
	y = flBottom;
	w = flRight - flLeft - (flRight - flLeft) / 8.f * 2.f;
	h = flTop - flBottom;

	if (pEntity->IsPlayer())
	{
		y -= 10.f;
		h += 10.f;
	}

	return !(x > H::Draw.m_nScreenW || x + w < 0 || y > H::Draw.m_nScreenH || y + h < 0);
}