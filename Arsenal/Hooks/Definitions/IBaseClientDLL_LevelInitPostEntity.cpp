#include "../Hooks.h"
//#include "../../Features/Backtrack/Backtrack.h"

DEFINE_HOOK(IBaseClientDLL_LevelInitPostEntity, void, __fastcall, void* ecx, void* edx)
{
	//F::Backtrack.Restart();

	Func.Original<FN>()(ecx, edx);

	g_Globals.m_nLocalIndex = I::EngineClient->GetLocalPlayer();
}