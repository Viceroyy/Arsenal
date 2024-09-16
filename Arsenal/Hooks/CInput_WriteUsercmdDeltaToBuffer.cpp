//#include "../SDK/SDK.h"
//#include "../Features/Backtrack/Backtrack.h"
//#include "../Features/NetworkFix/NetworkFix.h"
//
//CUserCmd* GetUsercmd(int sequence_number)
//{
//	return &I::Input->m_pCommands[sequence_number % MULTIPLAYER_BACKUP];
//}
//
//MAKE_SIGNATURE(CInput_WriteUsercmdDeltaToBuffer, "client.dll", "55 8B EC 83 EC ? 33 C0 C7 45 ? ? ? ? ? 89 45 ? 88 45 ? 8B 45", 0x0);
//MAKE_SIGNATURE(WriteUsercmd, "client.dll", "55 8B EC 8B 45 ? 83 EC ? 8B 40", 0x0);
//
//MAKE_HOOK(CInput_WriteUsercmdDeltaToBuffer, S::CInput_WriteUsercmdDeltaToBuffer(), bool, __fastcall,
//	void* ecx, void* edx, bf_write* buf, int from, int to)
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
//	G.SeqNum = to;
//	CUserCmd* t = GetUsercmd(to);
//
//	if (!t)
//		t = &nullcmd;
//
//	reinterpret_cast<void(__cdecl*)(bf_write*, CUserCmd*, CUserCmd*)>(S::WriteUsercmd())(buf, t, f);
//
//	return !buf->m_bOverflow;
//}