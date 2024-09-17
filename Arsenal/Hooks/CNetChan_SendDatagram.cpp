#include "../SDK/SDK.h"

#include "../Features/Backtrack/Backtrack.h"

MAKE_SIGNATURE(CNetChan_SendDatagram, "engine.dll", "55 8B EC B8 ? ? ? ? E8 ? ? ? ? A1 ? ? ? ? 53 56 8B D9", 0x0);

MAKE_HOOK(CNetChan_SendDatagram, S::CNetChan_SendDatagram(), int, __fastcall, 
	CNetChannel* netChannel, void* edx, bf_write* datagram)
{
    if (!I::EngineClient->IsInGame() || !netChannel || datagram)
        return CALL_ORIGINAL(netChannel, edx, datagram);

    F::Backtrack.bFakeLatency = CFG::Misc_Backtrack_LatencyMode == 1 || CFG::Misc_Backtrack_LatencyMode == 2 && CFG::Misc_Backtrack_Latency;
    if (!F::Backtrack.bFakeLatency)
        return CALL_ORIGINAL(netChannel, edx, datagram);

    const int nInSequenceNr = netChannel->m_nInSequenceNr, nInReliableState = netChannel->m_nInReliableState;
    F::Backtrack.AdjustPing(netChannel);
    const int original = CALL_ORIGINAL(netChannel, edx, datagram);
    netChannel->m_nInSequenceNr = nInSequenceNr, netChannel->m_nInReliableState = nInReliableState;

    return original;
}
