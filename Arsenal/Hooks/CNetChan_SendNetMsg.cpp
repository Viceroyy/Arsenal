#include "../SDK/SDK.h"

#include "../Features/Backtrack/Backtrack.h"

//CUserCmd* GetUsercmd(int sequence_number)
//{
//	return &I::Input->m_pCommands[sequence_number % MULTIPLAYER_BACKUP];;
//}
//
//MAKE_SIGNATURE(WriteUsercmd, "client.dll", "55 8B EC 8B 45 ? 83 EC ? 8B 40", 0x0);
//
//bool WriteUsercmdDeltaToBuffer(bf_write* buf, int from, int to)
//{
//	CUserCmd nullcmd = {};
//	CUserCmd* f = nullptr;
//
//	if (from == -1)
//		f = &nullcmd;
//
//	else
//	{
//		f = GetUsercmd(from);
//
//		if (!f)
//			f = &nullcmd;
//	}
//
//	CUserCmd* t = GetUsercmd(to);
//
//	if (!t)
//		t = &nullcmd;
//
//	reinterpret_cast<void(__cdecl*)(bf_write*, CUserCmd*, CUserCmd*)>(S::WriteUsercmd())(buf, t, f);
//
//	return !buf->m_bOverflow;
//}

MAKE_SIGNATURE(CNetChan_SendNetMsg, "engine.dll", "55 8B EC 57 8B F9 8D 8F", 0x0);

MAKE_HOOK(CNetChan_SendNetMsg, S::CNetChan_SendNetMsg(), bool, __fastcall, 
	CNetChannel* netChannel, void* edx, INetMessage& msg, bool bForceReliable, bool bVoice)
{
	switch (msg.GetType())
	{
		/*case clc_Move:
		{
			const auto pMsg = reinterpret_cast<CLC_Move*>(&msg);

			{
				const int nLastOutGoingCommand = I::ClientState->lastoutgoingcommand;
				const int nChokedCommands = I::ClientState->chokedcommands;
				const int nNextCommandNr = nLastOutGoingCommand + nChokedCommands + 1;

				byte data[4000] = {};
				pMsg->m_DataOut.StartWriting(data, sizeof(data));
				pMsg->m_nNewCommands = std::clamp(1 + nChokedCommands, 0, 15);
				const int nExtraCommands = nChokedCommands + 1 - pMsg->m_nNewCommands;
				const int nCmdBackup = std::max(2, nExtraCommands);
				pMsg->m_nBackupCommands = std::clamp(nCmdBackup, 0, 7);

				const int nNumCmds = pMsg->m_nNewCommands + pMsg->m_nBackupCommands;
				int nFrom = -1;
				bool bOk = true;
				for (int nTo = nNextCommandNr - nNumCmds + 1; nTo <= nNextCommandNr; nTo++)
				{
					const bool bIsNewCmd = nTo >= nNextCommandNr - pMsg->m_nNewCommands + 1;
					bOk = bOk && I::BaseClientDLL->WriteUsercmdDeltaToBuffer(&pMsg->m_DataOut, nFrom, nTo, bIsNewCmd);
					nFrom = nTo;
				}

				if (bOk)
				{
					if (nExtraCommands > 0)
						netChannel->m_nChokedPackets -= nExtraCommands;

					CALL_ORIGINAL(netChannel, edx, reinterpret_cast<INetMessage&>(*pMsg), bForceReliable, bVoice);
				}
			}

			return true;
		}*/
		case clc_VoiceData:
		{
			bVoice = true;
			break;
		}
		case clc_RespondCvarValue:
		{
			if (CFG::Visuals_NoConvarQueries)
			{
				if (const auto pMsg = reinterpret_cast<std::uintptr_t*>(&msg))
				{
					if (const auto cvarName = reinterpret_cast<const char*>(pMsg[6]))
					{
						if (const auto pConVar = U::ConVars.FindVar(cvarName))
						{
							if (auto defaultValue = pConVar->m_pParent->m_pszDefaultValue)
							{
								pMsg[7] = std::uintptr_t(defaultValue);
								I::Cvar->ConsoleColorPrintf({ 175, 150, 255, 255 }, "[Arsenal] clc_RespondCvarValue: ", msg.ToString());
								break;
							}
						}
						return true; //	if we failed to manipulate the data, don't send it.
					}
				}
			}
			break;
		}
		case clc_FileCRCCheck:
		{
			return false;
		}
	}

	return CALL_ORIGINAL(netChannel, edx, msg, bForceReliable, bVoice);
}
