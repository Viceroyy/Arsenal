#include "Visuals.h"
#include "../CFG.h"
#include "../../SDK/Input/Input.h"

void CFeatures_Visuals::Run()
{
	auto pLocal = H::Entities.GetLocal();
	if (!pLocal || pLocal->deadflag())
		return;

	SpreadCircle();
}

void CFeatures_Visuals::DoFOV(CViewSetup* pSetup)
{
	if (CFG::Visuals_ClearScreenshots && I::EngineClient->IsTakingScreenshot()) //CFG::Visuals_FOV == 90 || pSetup->fov < 90.f ||
		return;

	auto pLocal = H::Entities.GetLocal();
	if (!pLocal || Util::IsZoomed())
		return;

	H::Draw.String(EFonts::DEBUG, H::Draw.m_nScreenW / 2, H::Draw.m_nScreenH / 2, COLOR_BLACK, ALIGN_CENTER, std::format("pSetup->fov: {}, pSetup->fovViewmodel: {}", std::to_string(pSetup->fov), std::to_string(pSetup->fovViewmodel)).c_str());
	//pSetup->fov = CFG::Visuals_FOV;
}

void CFeatures_Visuals::DoThirdperson(CViewSetup* pSetup)
{
	/*auto pLocal = H::Entities.GetLocal();

	if (!pLocal || pLocal->deadflag())
		return;

	if (!I::MatSystemSurface->IsCursorVisible() && !I::EngineVGui->IsGameUIVisible())
	{
		if (H::Input.IsPressed(CFG::Visuals_Thirdperson_Key))
			CFG::Visuals_Thirdperson_Active = !CFG::Visuals_Thirdperson_Active;
	}

	bool bShouldDoTP = CFG::Visuals_Thirdperson_Active;

	if (bShouldDoTP)
	{
		I::Input->CAM_ToThirdPerson();
	}

	else
	{
		I::Input->CAM_ToFirstPerson();
	}

	pLocal->ThirdPersonSwitch();

	if (bShouldDoTP)
	{
		Vector vForward = {}, vRight = {}, vUp = {};
		U::Math.AngleVectors(pSetup->angles, &vForward, &vRight, &vUp);

		Vector vOffset = (vForward * CFG::Visuals_Thirdperson_Offset_Forward)
			- (vRight * CFG::Visuals_Thirdperson_Offset_Right)
			- (vUp * CFG::Visuals_Thirdperson_Offset_Up);

		Vector vDesiredOrigin = pSetup->origin - vOffset;

		Ray_t Ray = {};
		Ray.Init(pSetup->origin, vDesiredOrigin, { -10.0f, -10.0f, -10.0f }, { 10.0f, 10.0f, 10.0f });
		CTraceFilterWorldCustom Filter = {};
		trace_t Trace = {};
		I::EngineTrace->TraceRay(Ray, MASK_SOLID, &Filter, &Trace);

		pSetup->origin -= vOffset * Trace.fraction;
	}*/
}

/*void CFeatures_Visuals::ManualNetwork(const StartSoundParams_t& params)
{
	if (params.soundsource <= 0 || params.soundsource == I::EngineClient->GetLocalPlayer())
		return;

	auto pEntity = I::ClientEntityList->GetClientEntity(params.soundsource)->As<C_BaseEntity>();
	if (pEntity && pEntity->IsDormant() && pEntity->IsPlayer())
		G.DormancyMap[params.soundsource] = { params.origin, I::EngineClient->Time() };
}*/

void CFeatures_Visuals::SpreadCircle()
{
	if (!CFG::Visuals_DrawSpread || CFG::Misc_NoSpread || I::EngineVGui->IsGameUIVisible())
		return;

	auto weapon = H::Entities.GetWeapon();
	if (!weapon)
		return;

	weapon->UpdateAccuracyPenalty();
	float Spread = weapon->GetSpread();
	float Inaccuracy = weapon->GetInaccuracy();
	float fSpreadDistance = ((Inaccuracy + Spread) * 320.0f / tanf(DEG2RAD(CFG::Visuals_FOV) / 2));
	fSpreadDistance = (int)(fSpreadDistance * ((float)(H::Draw.m_nScreenH) / 480.0f));

	H::Draw.OutlinedCircle(H::Draw.m_nScreenW / 2, H::Draw.m_nScreenH / 2, fSpreadDistance, 360, COLOR_GREEN);
}