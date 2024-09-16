//#include "../SDK/SDK.h"
//
//MAKE_SIGNATURE(CL_SendMove, "engine.dll", "55 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 8D 4D", 0x0);
//
//MAKE_HOOK(CL_SendMove, S::CL_SendMove(), void, __cdecl, 
//	void* ecx, void* edx)
//{
//	byte data[4000];
//
//	const int nextcommandnr = I::ClientState->lastoutgoingcommand + I::ClientState->chokedcommands + 1;
//
//	CLC_Move moveMsg;
//	moveMsg.m_DataOut.StartWriting(data, sizeof(data));
//	moveMsg.m_nNewCommands = std::clamp(1 + I::ClientState->chokedcommands, 0, 15);
//	const int extraCommands = I::ClientState->chokedcommands + 1 - moveMsg.m_nNewCommands;
//	const int backupCommands = std::max(2, extraCommands);
//	moveMsg.m_nBackupCommands = std::clamp(backupCommands, 0, 7);
//
//	const int numcmds = moveMsg.m_nNewCommands + moveMsg.m_nBackupCommands;
//
//	int from = -1;
//	bool bOK = true;
//	for (int to = nextcommandnr - numcmds + 1; to <= nextcommandnr; to++)
//	{
//		const bool isnewcmd = to >= nextcommandnr - moveMsg.m_nNewCommands + 1;
//		bOK = bOK && I::BaseClientDLL->WriteUsercmdDeltaToBuffer(&moveMsg.m_DataOut, from, to, isnewcmd);
//		from = to;
//	}
//
//	if (bOK)
//	{
//		if (extraCommands)
//		{
//			I::ClientState->m_NetChannel->m_nChokedPackets -= extraCommands;
//		}
//		U::VFunc.Get<bool(__thiscall*)(PVOID, INetMessage* msg, bool, bool)>(I::ClientState->m_NetChannel, 37u)(I::ClientState->m_NetChannel, &moveMsg, false, false);
//	}
//
//	/*const int nLastOutGoingCommand = I::ClientState->lastoutgoingcommand;
//	const int nChokedCommands = I::ClientState->chokedcommands;
//	const int nNextCommandNr = nLastOutGoingCommand + nChokedCommands + 1;
//
//	byte data[4000] = {}; CLC_Move moveMsg;
//	moveMsg.m_DataOut.StartWriting(data, sizeof(data));
//	moveMsg.m_nNewCommands = std::clamp(1 + nChokedCommands, 0, 15);
//	const int nExtraCommands = nChokedCommands + 1 - moveMsg.m_nNewCommands;
//	const int nCmdBackup = std::max(2, nExtraCommands);
//	moveMsg.m_nBackupCommands = std::clamp(nCmdBackup, 0, 7);
//
//	const int nNumCmds = moveMsg.m_nNewCommands + moveMsg.m_nBackupCommands;
//	int nFrom = -1;
//	bool bOk = true;
//	for (int nTo = nNextCommandNr - nNumCmds + 1; nTo <= nNextCommandNr; nTo++)
//	{
//		const bool bIsNewCmd = nTo >= nNextCommandNr - moveMsg.m_nNewCommands + 1;
//		bOk = bOk && I::BaseClientDLL->WriteUsercmdDeltaToBuffer(&moveMsg.m_DataOut, nFrom, nTo, bIsNewCmd);
//		nFrom = nTo;
//	}
//
//	if (bOk)
//	{
//		if (nExtraCommands > 0)
//			I::ClientState->m_NetChannel->m_nChokedPackets -= nExtraCommands;
//
//		U::VFunc.Get<bool(__thiscall*)(PVOID, INetMessage* msg, bool, bool)>(I::ClientState->m_NetChannel, 37u)(I::ClientState->m_NetChannel, &moveMsg, false, false);
//	}*/
//}