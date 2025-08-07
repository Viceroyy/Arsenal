#include "../SDK/SDK.h"
#include <intrin.h>
#include "../Features/EnginePrediction/EnginePrediction.h"
#include "../Features/Misc/Misc.h"
#include "../Features/NoSpread/NoSpread.h"
#include "../Features/Backtrack/Backtrack.h"
#include "../Features/SequenceFreeze/SequenceFreeze.h"

MAKE_HOOK(ClientModeShared_CreateMove, U::VFunc.Get<void*>(I::ClientMode, 21u), bool,
	void* rcx, float input_sample_frametime, CUserCmd* cmd)
{
	if (!cmd || !cmd->command_number)
		return CALL_ORIGINAL(rcx, input_sample_frametime, cmd);

	bool* pSendPacket = reinterpret_cast<bool*>(uintptr_t(_AddressOfReturnAddress()) + 0x128);

	I::ClientPrediction->Update(I::ClientState->m_nDeltaTick, I::ClientState->m_nDeltaTick > 0, I::ClientState->last_command_ack, I::ClientState->lastoutgoingcommand + I::ClientState->chokedcommands);

	if (!CFG::Misc_AntiCheatCompatibility)
	{	// correct tick_count for fakeinterp / nointerp
		cmd->tick_count += TIME_TO_TICKS(F::Backtrack.GetFakeInterp());
		if (!CFG::Visuals_NoInterpolation && CFG::Visuals_NoLerp)
			cmd->tick_count -= TIME_TO_TICKS(G.Lerp);
	}

	auto pLocal = H::Entities.GetLocal();

	if (pLocal && pLocal->IsAlive())
	{
		F::Misc.Run(pLocal, cmd);
		F::SequenceFreeze.Run(pLocal, cmd, pSendPacket);

		auto pWeapon = H::Entities.GetWeapon();

		if (pWeapon)
		{
			F::EnginePrediction.Start(pLocal, cmd);
			{
				//F::NoSpread.Run(pLocal, pWeapon, cmd);
				F::Backtrack.BacktrackToCrosshair(cmd);
			}
			F::EnginePrediction.Finish(pLocal);
		}
	}
	return false;
}