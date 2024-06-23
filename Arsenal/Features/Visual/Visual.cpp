#include "Visual.h"
#include "../CFG.h"

void CFeatures_Visual::Run()
{
	auto pLocal = H::EntityCache.GetLocal();
	if (!pLocal || pLocal->deadflag())
		return;

	SpreadCircle();
}

void CFeatures_Visual::SpreadCircle()
{
	if (!CFG::Visual_DrawSpread)
		return;

	auto weapon = H::EntityCache.GetWeapon();
	if (!weapon)
		return;

	const auto accuracy = weapon->GetInaccuracy() * 550.f; //3000

	H::Draw.OutlinedCircle(H::Draw.m_nScreenW / 2, H::Draw.m_nScreenH / 2, accuracy, 360, COLOR_RED);
}