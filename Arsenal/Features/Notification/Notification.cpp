#include "Notification.h"

double PlatFloatTime()
{
	static auto fnPlatFloatTime = reinterpret_cast<double(*)()>(GetProcAddress(GetModuleHandleA("tier0.dll"), "Plat_FloatTime"));
	return fnPlatFloatTime();
}

void CFeatures_Notifications::Add(const std::string& sText, Color_t cColor, float flLifeTime)
{
	vNotifications.push_back({ sText, cColor, float(PlatFloatTime()) + flLifeTime });
}

void CFeatures_Notifications::Draw()
{
	if (vNotifications.size() > unsigned(iMaxNotifySize + 1))
		vNotifications.erase(vNotifications.begin());

	for (auto it = vNotifications.begin(); it != vNotifications.end();)
	{
		if (it->m_flTime <= PlatFloatTime())
			it = vNotifications.erase(it);
		else
			++it;
	}

	if (vNotifications.empty())
		return;

	int y = 2;
	const auto& fFont = EFonts::MENU;
	for (auto& tNotification : vNotifications)
	{
		int x = 2;
		int w, h; I::MatSystemSurface->GetTextSize(H::Draw.Get(EFonts::MENU).m_hFont, Util::ConvertUtf8ToWide(tNotification.m_sText).c_str(), w, h);
		w += 4; h += 4;

		const float flLife = std::min(tNotification.m_flTime - PlatFloatTime(), 5.0f - (tNotification.m_flTime - PlatFloatTime()));
		if (flLife < 0.1f)
			x -= U::Math.RemapValClamped(flLife, 0.1f, 0.f, 0.f, w);

		auto& cAccent = CFG::Menu_Accent_Primary, &cActive = CFG::Menu_Text, &cBackground = CFG::Menu_Background;
		H::Draw.Line(x, y, x, y + h, { cAccent.r, cAccent.g, cAccent.b, 255 });
		H::Draw.GradientRect(x + 1, y, w, h, { cBackground.r, cBackground.g, cBackground.b, 255 }, { cBackground.r, cBackground.g, cBackground.b, 0 }, true);
		H::Draw.String(fFont, x + 6, y + 2, { cActive.r, cActive.g, cActive.b, 255 }, ALIGN_TOPLEFT, tNotification.m_sText.c_str());

		y += h + 2;
	}
}