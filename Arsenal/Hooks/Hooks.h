#pragma once

#include "../SDK/SDK.h"

CREATE_HOOK(ClientModeShared_OverrideView, U::VFunc.Get<void*>(I::ClientMode, 16u), void, __fastcall, void* _this, void* _edx, CViewSetup* pSetup);
CREATE_HOOK(ClientModeShared_CreateMove, U::Offsets.ClientModeShared_CreateMove, bool, __fastcall, void* ecx, void* edx, float input_sample_frametime, CUserCmd* cmd);
CREATE_HOOK(ClientModeShared_GetViewmodelFOV, U::VFunc.Get<void*>(I::ClientMode, 32u), float, __fastcall);
//CREATE_HOOK(ClientModeShared_DoPostScreenSpaceEffects, U::VFunc.Get<void*>(I::ClientMode, 39u), bool, __fastcall, void* ecx, const CViewSetup* pSetup);
CREATE_HOOK(IEngineVGui_Paint, U::VFunc.Get<void*>(I::EngineVGui, 13u), void, __fastcall, void* ecx, void* edx, int mode);
CREATE_HOOK(IBaseClientDLL_FrameStageNotify, U::VFunc.Get<void*>(I::BaseClientDLL, 35u), void, __fastcall, void* ecx, void* edx, ClientFrameStage_t curStage);
CREATE_HOOK(IBaseClientDLL_LevelShutdown, U::VFunc.Get<void*>(I::BaseClientDLL, 7u), void, __fastcall, void* ecx, void* edx);
CREATE_HOOK(IBaseClientDLL_LevelInitPostEntity, U::VFunc.Get<void*>(I::BaseClientDLL, 6u), void, __fastcall, void* ecx, void* edx);
CREATE_HOOK(CPrediction_RunCommand, U::VFunc.Get<void*>(I::ClientPrediction, 17u), void, __fastcall, void* ecx, void* edx, C_BasePlayer* player, CUserCmd* ucmd, IMoveHelper* moveHelper);
CREATE_HOOK(CBasePlayer_CalcPlayerView, U::Offsets.C_BasePlayer_CalcPlayerView, void, __fastcall, C_BasePlayer* pThis, void* edx, Vector& eyeOrigin, Vector& eyeAngles, float& fov);
//CREATE_HOOK(ModelRender_DrawModelExecute, U::VFunc.Get<void*>(I::ModelRender, 19u), void, __fastcall, void* ecx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);

class CGlobal_Hooks
{
public:
	void Initialize();
	void Uninitialize();
};

namespace G { inline CGlobal_Hooks Hooks; }