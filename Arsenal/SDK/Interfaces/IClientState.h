#pragma once
#include "../Includes/inetchannel.h"

class CClientState
{
public:
	char pad_0x0000[0x10]; //0x0000
	INetChannel* m_NetChannel; //0x0010 
	char pad_0x0014[0x11C]; //0x0014
	__int32 m_nSignonState; //0x0130 
	char pad_0x0134[0x60]; //0x0134
	__int32 m_nCurrentSequence; //0x0194 
	__int32 unk1; //0x0198 
	__int32 unk2; //0x019C 
	__int32 m_nDeltaTick; //0x01A0 
	char pad_0x01A4[0x10C]; //0x01A4
	__int32 m_nMaxClients; //0x02B0 
	void* N00000583; //0x02B4 
	void* N00000582; //0x02B8 
	char pad_0x02BC[0x4854]; //0x02BC
	float m_flLastServerTickTime; //0x4B10 
	__int32 N0000196E; //0x4B14 
	__int32 oldtickcount; //0x4B18 
	float m_tickRemainder; //0x4B1C 
	float m_frameTime; //0x4B20 
	__int32 lastoutgoingcommand; //0x4B24 
	__int32 chokedcommands; //0x4B28 
	__int32 last_command_ack; //0x4B2C 
	__int32 command_ack; //0x4B30 
	char pad_0x4B34[0x50]; //0x4B34
	Vector viewangles; //0x4B84 
	char pad_0x4B90[0xCB0]; //0x4B90

	const char* GetLevelNameShort()
	{
		return (const char*)((unsigned int)this + 0x230);
	}
}; //Size=0x5840

namespace I { inline CClientState* ClientState = nullptr; }