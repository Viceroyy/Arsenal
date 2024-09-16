#include "../SDK/SDK.h"

enum UserMessages
{
	CS_UM_Geiger = 0,
	CS_UM_Train = 1,
	CS_UM_HudText = 2,
	CS_UM_SayText = 3,
	CS_UM_SayText2 = 4,
	CS_UM_TextMsg = 5,
	CS_UM_HudMsg = 6,
	CS_UM_ResetHUD = 7,
	CS_UM_GameTitle = 8,
	CS_UM_ItemPickup = 9,
	CS_UM_ShowMenu = 10,
	CS_UM_Shake = 11,
	CS_UM_Fade = 12,
	CS_UM_VGUIMenu = 13,
	CS_UM_Rumble = 14,
	CS_UM_CloseCaption = 15,
	CS_UM_SendAudio = 16,
	CS_UM_RawAudio = 17,
	CS_UM_VoiceMask = 18,
	CS_UM_RequestState = 19,
	CS_UM_BarTime = 20,
	CS_UM_Damage = 21,
	CS_UM_RadioText = 22,
	CS_UM_HintText = 23,
	CS_UM_KeyHintText = 24,
	CS_UM_ReloadEffect = 25,
	CS_UM_PlayerAnimEvent = 26,
	CS_UM_AmmoDenied = 27,
	CS_UM_UpdateRadar = 28,
	CS_UM_KillCam = 29,
	CS_UM_MarkAchievement = 30,
	CS_UM_CallVoteFailed = 31,
	CS_UM_VoteStart = 32,
	CS_UM_VotePass = 33,
	CS_UM_VoteFailed = 34,
	CS_UM_VoteSetup = 35,
	CS_UM_SPHapWeapEvent = 36,
	CS_UM_HapDmg = 37,
	CS_UM_HapPunch = 38,
	CS_UM_HapSetDrag = 39,
	CS_UM_HapSetConst = 40,
	CS_UM_HapMeleeContact = 41,
	CS_UM_PlayerStatsUpdate_DEPRECATED = 42,
	CS_UM_AchievementEvent = 43,
	CS_UM_MatchEndConditions = 44,
	CS_UM_MatchStatsUpdate = 45,
	CS_UM_PlayerStatsUpdate = 46
};

MAKE_SIGNATURE(CUserMessages_DispatchUserMessage, "client.dll", "55 8B EC 8B 55 ? 83 EC ? 56 8B F1 85 D2 0F 88", 0x0);

MAKE_HOOK(CUserMessages_DispatchUserMessage, S::CUserMessages_DispatchUserMessage(), bool, __fastcall, 
	void* ecx, void* edx, UserMessages msg_type, bf_read& msg_data)
{
	if (CFG::Visuals_NoMOTD && msg_type == CS_UM_VGUIMenu)
	{
		const char* VGUIMenu_Name = *(const char**)(&msg_data);

		if (!strcmp(VGUIMenu_Name, "info"))
		{
			I::EngineClient->ClientCmd_Unrestricted("joingame");

			return true;
		}
	}

	if (CFG::Visuals_NoScreenEffects && (msg_type == CS_UM_Shake || msg_type == CS_UM_Fade || msg_type == CS_UM_Rumble))
		return true;

	return CALL_ORIGINAL(ecx, edx, msg_type, msg_data);
}