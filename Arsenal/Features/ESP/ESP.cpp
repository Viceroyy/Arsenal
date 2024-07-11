#include "ESP.h"
#include "../CFG.h"
#include "../Players/Players.h"

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
				if (auto weapon_name = Util::ConvertToUppercase(GetWeaponName(pWeapon->GetWeaponID())))
				{
					H::Draw.String(fFont, x + (w / 2), y + h + bOffset, COLOR_WHITE, ALIGN_TOP, weapon_name.get());
					bOffset += H::Draw.GetFontHeight(fFont);
				}
			}
		}

		player_info_t pi;
		if (CFG::ESP_Players_Name && I::EngineClient->GetPlayerInfo(nIndex, &pi))
		{
			tOffset += H::Draw.GetFontHeight(fFont) + 2;
			H::Draw.String(fFont, x + (w / 2), y - tOffset, COLOR_WHITE, ALIGN_TOP, Util::ConvertUtf8ToWide(pi.name).data());
		}
	}
	
	I::MatSystemSurface->DrawSetAlphaMultiplier(1.f);
}

void CFeatures_ESP::DrawWorld()
{
	if (!CFG::ESP_World_Enabled)
		return;

	const EFonts& fFont = EFonts::ESP_SMALL;
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

			auto pWeps = pWeapons->As<C_WeaponCSBase>();

			if (CFG::ESP_World_Box)
			{
				H::Draw.OutlinedRect(x, y, w, h, CFG::Colors_DroppedWeapons);
				H::Draw.OutlinedRect(x - 1, y - 1, w + 2, h + 2, COLOR_BLACK); // Outline
				H::Draw.OutlinedRect(x + 1, y + 1, w - 2, h - 2, COLOR_BLACK); // Inline
			}

			if (CFG::ESP_World_Name)
			{
				if (auto weapon_name = Util::ConvertToUppercase(GetWeaponName(pWeps->GetWeaponID())))
				{
					H::Draw.String(fFont, x + w / 2, y - nTextTopOffset, COLOR_WHITE, ALIGN_TOP, weapon_name.get());
				}
			}
		}
	}

	I::MatSystemSurface->DrawSetAlphaMultiplier(1.f);
}

bool CFeatures_ESP::GetDynamicBounds(C_BaseEntity* pEntity, int& x, int& y, int& w, int& h)
{
	if (!pEntity)
		return false;

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
	w = flRight - flLeft;
	h = flTop - flBottom;

	return !(x > H::Draw.m_nScreenW || x + w < 0 || y > H::Draw.m_nScreenH || y + h < 0);
}

std::uintptr_t get_rel32(std::uintptr_t address, std::uintptr_t offset, std::uintptr_t instruction_size)
{
	return address + *reinterpret_cast<std::uintptr_t*>(address + offset) + instruction_size;
}

std::wstring CFeatures_ESP::GetWeaponName(int wpnid)
{
	static auto function = reinterpret_cast<const char* (*)(int)>(
		get_rel32(U::Pattern.Find("client.dll", "E8 ? ? ? ? 50 FF 75 94"), 1, 5));

	if (!function(wpnid))
		return L"unknown";

	return Util::ConvertUtf8ToWide(function(wpnid));
}
