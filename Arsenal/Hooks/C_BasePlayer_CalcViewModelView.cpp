//#include "../SDK/SDK.h"
//
//MAKE_SIGNATURE(C_BasePlayer_CalcViewModelView, "client.dll", "55 8B EC 53 8B 5D ? 56 57 8B F9 33 F6 8D 49", 0x0);
//
//MAKE_HOOK(C_BasePlayer_CalcViewModelView, S::C_BasePlayer_CalcViewModelView(), void, __fastcall,
//	void* ecx, void* edx, C_BasePlayer* owner, const Vector& eyePosition, const Vector& eyeAngles)
//{
//	if (CFG::Visuals_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
//		return CALL_ORIGINAL(ecx, edx, owner, eyePosition, eyeAngles);
//
//	auto pLocal = H::EntityCache.GetLocal();
//
//	if (pLocal && !pLocal->deadflag())
//	{
//		Vector vForward = {}, vRight = {}, vUp = {};
//		U::Math.AngleVectors(eyeAngles, &vForward, &vRight, &vUp);
//
//		Vector vEyePosition = eyePosition + (
//			(vForward * CFG::Visuals_Viewmodel_OffsetX) +
//			(vRight * CFG::Visuals_Viewmodel_OffsetY) +
//			(vUp * CFG::Visuals_Viewmodel_OffsetZ)
//			);
//
//		//if (CFG::Visuals_Viewmodel_Roll)
//			//vEyePosition.z += CFG::Visuals_Viewmodel_Roll;
//
//		CALL_ORIGINAL(ecx, edx, owner, vEyePosition, eyeAngles);
//	}
//
//	else CALL_ORIGINAL(ecx, edx, owner, eyePosition, eyeAngles);
//
//	/*Vector vOffset = { float(CFG::Visuals_Viewmodel_OffsetX), float(CFG::Visuals_Viewmodel_OffsetY), float(CFG::Visuals_Viewmodel_OffsetZ) };
//	bool bOffset = !vOffset.IsZero();
//
//	if (!bOffset && !CFG::Visuals_Viewmodel_Roll && vEyeAngles.IsZero() || CFG::Visuals_ClearScreenshots && I::EngineClient->IsTakingScreenshot())
//		return CALL_ORIGINAL(ecx, edx, pOwner, vEyePosition, vEyeAngles);
//
//	Vector vNewEyePosition = vEyePosition;
//	if (bOffset)
//	{
//		Vector vForward = {}, vRight = {}, vUp = {};
//		U::Math.AngleVectors(vEyeAngles, &vForward, &vRight, &vUp);
//		vNewEyePosition += (vRight * vOffset.x) + (vForward * vOffset.y) + (vUp * vOffset.z);
//	}
//	if (CFG::Visuals_Viewmodel_Roll)
//		vEyeAngles.z += CFG::Visuals_Viewmodel_Roll;
//
//	CALL_ORIGINAL(ecx, edx, pOwner, vNewEyePosition, vEyeAngles);*/
//}