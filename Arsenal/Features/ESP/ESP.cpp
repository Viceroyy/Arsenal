#include "ESP.h"
#include "../CFG.h"

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

	I::MatSystemSurface->DrawSetAlphaMultiplier(CFG::ESP_Alpha);

	for (auto pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ALL))
	{
		auto pPlayer = pEntity->As<C_CSPlayer>();
		if (pPlayer->deadflag())
			continue;

		bool bIsLocal = pPlayer == pLocal;
		if (CFG::ESP_Players_Ignore_Local && bIsLocal)
			continue;

		if (!bIsLocal)
		{
			if (CFG::ESP_Players_Ignore_Teammates && pPlayer->m_iTeamNum() == pLocal->m_iTeamNum())
				continue;

			if (CFG::ESP_Players_Ignore_Enemies && pPlayer->m_iTeamNum() != pLocal->m_iTeamNum())
				continue;
		}

		int x = 0, y = 0, w = 0, h = 0;
		if (!GetDynamicBounds(pPlayer, x, y, w, h))
			continue;

		const int nIndex = pPlayer->entindex();
		int lOffset = 0, rOffset = 0, bOffset = 2, tOffset = 0;
		const EFonts& fFont = EFonts::ESP_SMALL;
		const int iHealth = pPlayer->GetHealth();
		const int iMaxHealth = pPlayer->GetMaxHealth();

		const Color_t clrTeam = Util::GetTeamColor(pPlayer->m_iTeamNum(), pLocal->m_iTeamNum(), CFG::Colors_Relative);

		if (CFG::ESP_Players_Box)
		{
			H::Draw.OutlinedRect(x, y, w, h, clrTeam);

			//Outline
			H::Draw.OutlinedRect(x - 1, y - 1, w + 2, h + 2, COLOR_BLACK);

			//Inline
			H::Draw.OutlinedRect(x + 1, y + 1, w - 2, h - 2, COLOR_BLACK);
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
				H::Draw.String(fFont, x + (w / 2), y + h + bOffset, COLOR_WHITE, ALIGN_TOP, "%s", GetWeaponName(pWeapon->GetWeaponID()));
				bOffset += H::Draw.GetFontHeight(fFont);
			}
		}

		player_info_t pi;
		if (CFG::ESP_Players_Name && I::EngineClient->GetPlayerInfo(nIndex, &pi))
		{
			tOffset += H::Draw.GetFontHeight(fFont) + 2;
			H::Draw.String(fFont, x + (w / 2), y - tOffset, COLOR_GREY, ALIGN_TOP, Util::ConvertUtf8ToWide(pi.name).data());
		}
	}
	
	I::MatSystemSurface->DrawSetAlphaMultiplier(1.f);
}

void CFeatures_ESP::DrawWorld()
{
	if (!CFG::ESP_World_Enabled)
		return;

	const EFonts& fFont = EFonts::ESP_SMALL;
	const int nTextTopOffset = H::Draw.GetFontHeight(fFont) * (5 / 4);

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

				//Outline
				H::Draw.OutlinedRect(x - 1, y - 1, w + 2, h + 2, COLOR_BLACK);

				//Inline
				H::Draw.OutlinedRect(x + 1, y + 1, w - 2, h - 2, COLOR_BLACK);
			}

			if (CFG::ESP_World_Name)
				H::Draw.String(fFont, x + w / 2, y - nTextTopOffset, CFG::Colors_PlantedC4, ALIGN_TOP, "C4");
		}
	}

	//if (!CFG::ESP_World_Ignore_DroppedWeapons)
	//{
	//	for (auto pWeapons : H::EntityCache.GetGroup(EGroupType::WORLD_WEAPONS))
	//	{
	//		int x = 0, y = 0, w = 0, h = 0;
	//		if (!GetDynamicBounds(pWeapons, x, y, w, h))
	//			continue;

	//		if (CFG::ESP_World_Box)
	//		{
	//			H::Draw.OutlinedRect(x, y, w, h, CFG::Colors_PlantedC4);

	//			//Outline
	//			H::Draw.OutlinedRect(x - 1, y - 1, w + 2, h + 2, COLOR_BLACK);

	//			//Inline
	//			H::Draw.OutlinedRect(x + 1, y + 1, w - 2, h - 2, COLOR_BLACK);
	//		}

	//		if (CFG::ESP_World_Name)
	//			H::Draw.String(fFont, x + w / 2, y - nTextTopOffset, COLOR_RED, ALIGN_TOP, "Weapon");
	//	}
	//}

	I::MatSystemSurface->DrawSetAlphaMultiplier(1.f);
}

bool CFeatures_ESP::GetDynamicBounds(C_BaseEntity* pEntity, int& x, int& y, int& w, int& h)
{
	Vector vMins = pEntity->m_vecMins(), vMaxs = pEntity->m_vecMaxs();

	auto& transform = const_cast<matrix3x4_t&>(pEntity->RenderableToWorldTransform());
	if (pEntity && pEntity->entindex() == I::EngineClient->GetLocalPlayer())
	{
		Vector vAngles = I::EngineClient->GetViewAngles();
		vAngles.x = vAngles.z = 0.f;
		U::Math.AngleMatrix(vAngles, transform);
		U::Math.MatrixSetColumn(pEntity->GetAbsOrigin(), 3, transform);
	}

	float flLeft, flRight, flTop, flBottom;
	if (!Util::IsOnScreen(pEntity, transform, &flLeft, &flRight, &flTop, &flBottom))
		return false;

	x = flLeft + (flRight - flLeft) / 8.f;
	y = flBottom;
	w = flRight - flLeft - (flRight - flLeft) / 8.f * 2.f;
	h = flTop - flBottom;

	return !(x > H::Draw.m_nScreenW || x + w < 0 || y > H::Draw.m_nScreenH || y + h < 0);
}

const char* CFeatures_ESP::GetWeaponName(int wpnid)
{
	switch (wpnid)
	{
		case WEAPON_P228: return "P228"; break;
		case WEAPON_GLOCK: return "GLOCK"; break;
		case WEAPON_SCOUT: return "SCOUT"; break;
		case WEAPON_HEGRENADE: return "HE GRENADE"; break;
		case WEAPON_XM1014: return "XM1014"; break;
		case WEAPON_C4: return "C4"; break;
		case WEAPON_MAC10: return "MAC10"; break;
		case WEAPON_AUG: return "AUG"; break;
		case WEAPON_SMOKEGRENADE: return "SMOKE GRENADE"; break;
		case WEAPON_ELITE: return "ELITE"; break;
		case WEAPON_FIVESEVEN: return "FIVE-SEVEN"; break;
		case WEAPON_UMP45: return "UMP45"; break;
		case WEAPON_SG550: return "SG550"; break;
		case WEAPON_GALIL: return "GALIL"; break;
		case WEAPON_FAMAS: return "FAMAS"; break;
		case WEAPON_USP: return "USP"; break;
		case WEAPON_AWP: return "AWP"; break;
		case WEAPON_MP5NAVY: return "MP5"; break;
		case WEAPON_M249: return "M249"; break;
		case WEAPON_M3: return "M3"; break;
		case WEAPON_M4A1: return "M4A1"; break;
		case WEAPON_TMP: return "TMP"; break;
		case WEAPON_G3SG1: return "G3SG1"; break;
		case WEAPON_FLASHBANG: return "FLASHBANG"; break;
		case WEAPON_DEAGLE: return "DEAGLE"; break;
		case WEAPON_SG552: return "SG552"; break;
		case WEAPON_AK47: return "AK47"; break;
		case WEAPON_KNIFE: return "KNIFE"; break;
		case WEAPON_P90: return "P90"; break;
	}
	return "UNKNOWN";
}