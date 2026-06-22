#include "../SDK/SDK.h"

#include "../Features/Backtrack/Backtrack.h"

MAKE_SIGNATURE(CNetChan_SendDatagram, "engine.dll", "40 55 57 41 56 48 8D AC 24", 0x0);

MAKE_HOOK(CNetChan_SendDatagram, S::CNetChan_SendDatagram(), int,
    CNetChannel* netChannel, bf_write* datagram)
{
	if (datagram)
		return CALL_ORIGINAL(netChannel, datagram);

	F::Backtrack.AdjustPing(netChannel);
	const int iReturn = CALL_ORIGINAL(netChannel, datagram);
	F::Backtrack.RestorePing(netChannel);
	return iReturn;
}
