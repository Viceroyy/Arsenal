#include "../SDK/SDK.h"
#include "../SDK/Helpers/Input.h"
#include "../Features/Backtrack/Backtrack.h"
#include "../Features/WorldModulation/WorldModulation.h"

MAKE_HOOK(IBaseClientDLL_FrameStageNotify, U::VFunc.Get<void*>(I::BaseClientDLL, 35u), void,
	void* rcx, ClientFrameStage_t curStage)
{
	if (CFG::Visuals_NoInterpolation && curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_END)
	{
		if (auto pLocal = H::Entities.GetLocal())
		{
			for (auto pEntity : H::Entities.GetGroup(EGroupType::PLAYERS_ALL))
			{
				auto pPlayer = pEntity->As<C_CSPlayer>();
				if (!pPlayer || pPlayer == pLocal || pPlayer->deadflag())
					continue;

				pPlayer->SetInterpolation(0);
			}
		}
	}

	CALL_ORIGINAL(rcx, curStage);

	switch (curStage)
	{
	case FRAME_NET_UPDATE_START:
	{
		H::Entities.Clear();
		break;
	}
	case FRAME_NET_UPDATE_END:
	{
		H::Entities.Store();
		F::Backtrack.Store();
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