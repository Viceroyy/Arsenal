#include "../SDK/SDK.h"
#include "../SDK/Input/Input.h"
#include "../Features/Backtrack/Backtrack.h"
#include "../Features/WorldModulation/WorldModulation.h"

MAKE_HOOK(IBaseClientDLL_FrameStageNotify, U::VFunc.Get<void*>(I::BaseClientDLL, 35u), void, __fastcall, 
	void* ecx, void* edx, ClientFrameStage_t curStage)
{
	if (CFG::Visuals_NoInterpolation && curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_END)
	{
		if (auto pLocal = H::EntityCache.GetLocal())
		{
			for (auto pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ALL))
			{
				auto pPlayer = pEntity->As<C_CSPlayer>();
				if (!pPlayer || pPlayer == pLocal || pPlayer->deadflag())
					continue;

				pPlayer->SetInterpolation(0);
			}
		}
	}

	CALL_ORIGINAL(ecx, edx, curStage);

	switch (curStage)
	{
		case FRAME_NET_UPDATE_START:
		{
			H::EntityCache.Clear();
			break;
		}
		case FRAME_NET_UPDATE_END:
		{
			H::EntityCache.Fill();

			if (auto pLocal = H::EntityCache.GetLocal())
			{
				for (auto pEntity : H::EntityCache.GetGroup(EGroupType::PLAYERS_ALL))
				{
					if (!pEntity || pEntity == pLocal)
						continue;

					auto pPlayer = pEntity->As<C_CSPlayer>();

					if (auto nDifference = std::clamp(TIME_TO_TICKS(pPlayer->m_flSimulationTime() - pPlayer->m_flOldSimulationTime()), 0, 22))
					{
						if (pPlayer->m_iTeamNum() != pLocal->m_iTeamNum() && !pPlayer->deadflag())
						{
							F::LagRecords.AddRecord(pPlayer);
						}
					}
				}
			}

			F::LagRecords.UpdateRecords();
			//F::Backtrack.FrameStageNotify();
			break;
		}
		case FRAME_RENDER_START:
		{
			if (G.Unload)
				break;

			H::Input.Update();

			F::WorldModulation.UpdateWorldModulation();
			if (CFG::Visuals_LowGraphics)
			{
				static ConVar* r_drawdetailprops = U::ConVars.FindVar("r_drawdetailprops");

				if (r_drawdetailprops && r_drawdetailprops->GetInt())
					r_drawdetailprops->SetValue(0);
			}
			break;
		}
	}
}