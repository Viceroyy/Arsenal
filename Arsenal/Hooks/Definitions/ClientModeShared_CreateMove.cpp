#include "../Hooks.h"

#include "../../Features/EnginePrediction/EnginePrediction.h"
#include "../../Features/Misc/Misc.h"
#include "../../Features/NoSpread/NoSpread.h"

DEFINE_HOOK(ClientModeShared_CreateMove, bool, __fastcall, void* ecx, void* edx, float input_sample_frametime, CUserCmd* cmd)
{
	if (!cmd || !cmd->command_number)
		return Func.Original<FN>()(ecx, edx, input_sample_frametime, cmd);

	//I::ClientPrediction->Update(I::cl->m_nDeltaTick, I::ClientState->m_nDeltaTick > 0, I::ClientState->last_command_ack, I::ClientState->lastoutgoingcommand + I::ClientState->chokedcommands);

	auto pLocal = H::EntityCache.GetLocal();
	auto pWeapon = H::EntityCache.GetWeapon();

	F::Misc.Run(pLocal, cmd);

	F::EnginePrediction.Start(cmd);
	{
		F::NoSpread.Run(pLocal, pWeapon, cmd);
	}
	F::EnginePrediction.Finish();

	return false;
}