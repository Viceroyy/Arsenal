#include "../SDK/SDK.h"

MAKE_HOOK(ClientModeShared_GetViewmodelFOV, U::VFunc.Get<void*>(I::ClientMode, 32u), float)
{
	//if (CFG::Visuals_ViewmodelFOV == 70 || CFG::Visuals_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
	
	float fov = CALL_ORIGINAL();
	return fov;
	H::Draw.String(EFonts::DEBUG, H::Draw.m_nScreenW / 2, (H::Draw.m_nScreenH / 2) + 5, COLOR_BLACK, ALIGN_CENTER, std::format("GetViewmodelFOV: {}", std::to_string(fov)).c_str());
	
	//return CFG::Visuals_ViewmodelFOV;
}