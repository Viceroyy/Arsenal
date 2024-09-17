#include "C_CSPlayer.h"
#include "../../Features/Players/Players.h"

MAKE_SIGNATURE(C_CSPlayer_HasPlayerAsFriend, "client.dll", "55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? 85 C0", 0x0);

bool C_CSPlayer::HasPlayerAsFriend()
{
	if (!this)
		return false;

	auto result{ reinterpret_cast<bool(__thiscall*)(C_CSPlayer*, C_CSPlayer*)>(S::C_CSPlayer_HasPlayerAsFriend())(this, this) };

	if (!result)
	{
		PlayerPriority info{};
		F::Players.GetInfo(entindex(), info);

		return info.Ignored;
	}

	return result;
}