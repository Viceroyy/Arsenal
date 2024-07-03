#include "SpectatorList.h"

bool CFeatures_SpectatorList::GetSpectators(C_CSPlayer* pLocal)
{
	Spectators.clear();

	for (auto pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ALL))
	{
		auto pPlayer = pEntity->As<C_CSPlayer>();

		if (!pPlayer->deadflag() || pPlayer->m_hObserverTarget().Get() != pLocal)
			continue;

		std::wstring szMode;
		switch (pPlayer->m_iObserverMode())
		{
			case OBS_MODE_DEATHCAM:		szMode = L"Deathcam"; break;
			case OBS_MODE_FREEZECAM:	szMode = L"Freezecam"; break;
			case OBS_MODE_FIXED:		szMode = L"Fixed"; break;
			case OBS_MODE_FIRSTPERSON:	szMode = L"1st"; break;
			case OBS_MODE_THIRDPERSON:	szMode = L"3rd"; break;
			case OBS_MODE_ROAMING:		szMode = L"Roaming"; break;
			default: continue;
		}

		player_info_t pi{};
		if (I::EngineClient->GetPlayerInfo(pPlayer->entindex(), &pi))
		{
			Spectators.push_back({ Util::ConvertUtf8ToWide(pi.name), szMode });
		}
	}

	return !Spectators.empty();
}

void CFeatures_SpectatorList::Run()
{
	if (!CFG::Visual_SpectatorList)
		return;

	auto pLocal = H::EntityCache.GetLocal();
	if (!pLocal || pLocal->deadflag() || !GetSpectators(pLocal))
		return;

	int x = H::Draw.m_nScreenW / 2;
	int iconOffset = 0;
	int y = H::Draw.m_nScreenH / 4;

	EAlign align = ALIGN_TOP;
	if (x <= (100 + 50))
	{
		x -= 42;
		align = ALIGN_TOPLEFT;
	}
	else if (x >= H::Draw.m_nScreenW - (100 + 50))
	{
		x += 42;
		align = ALIGN_TOPRIGHT;
	}

	const auto& fFont = EFonts::MENU_OTHER;

	H::Draw.String(fFont, x, y, CFG::Menu_Accent_Primary, align, L"Spectating You:");
	for (const auto& Spectator : Spectators)
	{
		y += H::Draw.GetFontHeight(fFont) + 3;

		Color_t color = CFG::Menu_Text_Active;
		if (Spectator.Mode == std::wstring{ L"1st" })
			color = { 255, 200, 127, 255 };
		H::Draw.String(fFont, x + iconOffset, y, color, align,
			L"%ls - %ls", Spectator.Name.data(), Spectator.Mode.data());
	}
}
