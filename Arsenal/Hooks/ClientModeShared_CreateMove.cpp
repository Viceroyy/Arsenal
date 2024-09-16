#include "../SDK/SDK.h"
#include <intrin.h>
#include "../Features/EnginePrediction/EnginePrediction.h"
#include "../Features/Misc/Misc.h"
#include "../Features/NoSpread/NoSpread.h"
#include "../Features/Backtrack/Backtrack.h"
#include "../Features/SequenceFreeze/SequenceFreeze.h"

MAKE_SIGNATURE(ClientModeShared_CreateMove, "client.dll", "55 8B EC E8 ? ? ? ? 8B C8 85 C9 75 06", 0x0);

MAKE_HOOK(ClientModeShared_CreateMove, S::ClientModeShared_CreateMove(), bool, __fastcall, 
	void* ecx, void* edx, float input_sample_frametime, CUserCmd* cmd)
{
	if (!cmd || !cmd->command_number)
		return CALL_ORIGINAL(ecx, edx, input_sample_frametime, cmd);

	auto ebp = *reinterpret_cast<uintptr_t*>(uintptr_t(_AddressOfReturnAddress()) - sizeof(uintptr_t));
	bool* pSendPacket = reinterpret_cast<bool*>(ebp - 0x1);

	I::ClientPrediction->Update(I::ClientState->m_nDeltaTick, I::ClientState->m_nDeltaTick > 0, I::ClientState->last_command_ack, I::ClientState->lastoutgoingcommand + I::ClientState->chokedcommands);

	//cmd->tick_count += TICKS_TO_TIME(F::Backtrack.flFakeInterp) - (CFG::Visuals_NoInterpolation ? 0 : TICKS_TO_TIME(G.Lerp));

	auto pLocal = H::EntityCache.GetLocal();
	auto pWeapon = H::EntityCache.GetWeapon();

	F::Misc.Run(pLocal, cmd);
	F::SequenceFreeze.Run(pLocal, cmd, pSendPacket);

	F::EnginePrediction.Start(pLocal, cmd);
	{
		F::NoSpread.Run(pLocal, pWeapon, cmd);
		//F::Backtrack.BacktrackToCrosshair(cmd);
	}
	F::EnginePrediction.Finish(pLocal, cmd);

	return false;
}