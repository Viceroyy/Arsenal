#include "SequenceFreeze.h"
#include "../../SDK/Input/Input.h"

void CFeatures_SequenceFreeze::Run(C_CSPlayer* pLocal, CUserCmd* cmd, bool pSendPacket)
{
	if (!pLocal || pLocal->deadflag())
		return;

	if (!I::MatSystemSurface->IsCursorVisible() && !I::EngineVGui->IsGameUIVisible())
	{
		if (H::Input.IsPressed(CFG::Misc_SequenceFreeze_Key))
			CFG::Misc_SequenceFreeze_Active = !CFG::Misc_SequenceFreeze_Active;
	}

	/*auto pNetChan = static_cast<CNetChannel*>(I::EngineClient->GetNetChannelInfo());
	if (!pNetChan)
		return;*/

	if (CFG::Misc_SequenceFreeze_Active)
	{
		/*pSendPacket = (G.SeqNum % 10) == 0;

		cmd->command_number += CFG::Misc_SequenceFreeze_Value * 150;
		I::ClientState->lastoutgoingcommand += CFG::Misc_SequenceFreeze_Value * 150;*/
		I::ClientState->m_NetChannel->m_nOutSequenceNr += CFG::Misc_SequenceFreeze_Value * 150;
	}
}