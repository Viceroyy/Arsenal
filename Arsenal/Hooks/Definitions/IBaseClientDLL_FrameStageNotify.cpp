#include "../Hooks.h"
#include "../../SDK/Input/Input.h"

DEFINE_HOOK(IBaseClientDLL_FrameStageNotify, void, __fastcall, void* ecx, void* edx, ClientFrameStage_t curStage)
{
	Func.Original<FN>()(ecx, edx, curStage);

	switch (curStage)
	{
		case ClientFrameStage_t::FRAME_NET_UPDATE_START:
		{
			H::EntityCache.Clear();
			break;
		}
		case ClientFrameStage_t::FRAME_NET_UPDATE_END:
		{
			H::EntityCache.Fill();
			g_Globals.m_bIsInGame = I::EngineClient->IsInGame();
			g_Globals.m_bIsGameUIVisible = I::EngineVGui->IsGameUIVisible();

			if (g_Globals.m_bIsInGame)
			{
				g_Globals.m_nMaxClients = I::EngineClient->GetMaxClients();
				g_Globals.m_nMaxEntities = I::ClientEntityList->GetMaxEntities();
			}
			break;
		}
		case ClientFrameStage_t::FRAME_RENDER_START:
		{
			H::Input.Update();
			break;
		}
	}
}