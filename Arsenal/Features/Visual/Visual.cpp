#include "Visual.h"
#include "../CFG.h"

void CFeatures_Visual::Run()
{
	auto pLocal = H::EntityCache.GetLocal();
	if (!pLocal || pLocal->deadflag())
		return;

	SpreadCircle();
	Crosshair();
}

void CFeatures_Visual::SpreadCircle()
{
	if (!CFG::Visual_DrawSpread || CFG::Misc_NoSpread || I::EngineVGui->IsGameUIVisible())
		return;

	auto weapon = H::EntityCache.GetWeapon();
	if (!weapon)
		return;

	weapon->UpdateAccuracyPenalty();
	float Spread = weapon->GetSpread();
	float Inaccuracy = weapon->GetInaccuracy();
	float fSpreadDistance = ((Inaccuracy + Spread) * 320.0f / tanf(DEG2RAD(CFG::Visual_FOV) / 2));
	fSpreadDistance = (int)(fSpreadDistance * ((float)(H::Draw.m_nScreenH) / 480.0f));

	H::Draw.OutlinedCircle(H::Draw.m_nScreenW / 2, H::Draw.m_nScreenH / 2, fSpreadDistance, 360, COLOR_GREEN);
}

void CFeatures_Visual::Crosshair()
{
	if (!CFG::Visual_DrawCrosshair || I::EngineVGui->IsGameUIVisible())
		return;

	int CrosshairSize = 20;
	int HalfSize = CrosshairSize / 2;

	H::Draw.OutlinedRect((H::Draw.m_nScreenW / 2) - 1, (H::Draw.m_nScreenH / 2) - HalfSize + 1, 2, CrosshairSize - 2, COLOR_GREEN); // |
	H::Draw.OutlinedRect((H::Draw.m_nScreenW / 2) - HalfSize + 1, (H::Draw.m_nScreenH / 2) - 1, CrosshairSize - 2, 2, COLOR_GREEN); // -
}