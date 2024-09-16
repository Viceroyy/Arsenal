#include "../SDK/SDK.h"
#include "../Features/Backtrack/Backtrack.h"
#include "../Features/NetworkFix/NetworkFix.h"

MAKE_SIGNATURE(CHudCrosshair_GetDrawPosition, "client.dll", "55 8B EC 81 EC ? ? ? ? 53 56 E8", 0x0);

MAKE_HOOK(CHudCrosshair_GetDrawPosition, S::CHudCrosshair_GetDrawPosition(), void, __cdecl,
	float* pX, float* pY, bool* pbBehindCamera, Vector angleCrosshairOffset)
{
	if (!CFG::Visuals_Thirdperson_Crosshair || CFG::Visuals_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
		return CALL_ORIGINAL(pX, pY, pbBehindCamera, angleCrosshairOffset);

	auto pLocal = H::EntityCache.GetLocal();
	if (!pLocal)
		return CALL_ORIGINAL(pX, pY, pbBehindCamera, angleCrosshairOffset);

	bool bSet = false;

	if (!bSet && I::Input->CAM_IsThirdPerson())
	{
		const Vector viewangles = I::EngineClient->GetViewAngles();
		Vector vForward{};
		U::Math.AngleVectors(viewangles, &vForward);

		const Vector vStartPos = pLocal->GetEyePosition();
		const Vector vEndPos = (vStartPos + vForward * 8192);

		CGameTrace trace = {};
		CTraceFilterHitscan filter = {};
		filter.pSkip = pLocal;
		Util::Trace(vStartPos, vEndPos, MASK_SHOT, &filter, &trace);

		Vector2D vScreen;
		if (H::Draw.WorldPosToScreenPos(trace.endpos, vScreen))
		{
			if (pX) *pX = vScreen.x;
			if (pY) *pY = vScreen.y;
			if (pbBehindCamera) *pbBehindCamera = false;
			bSet = true;
		}
	}

	if (!bSet)
		CALL_ORIGINAL(pX, pY, pbBehindCamera, angleCrosshairOffset);
}