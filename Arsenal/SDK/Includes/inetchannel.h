//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef INETCHANNEL_H
#define INETCHANNEL_H
#ifdef _WIN32
#pragma once
#endif

#include "inetchannelinfo.h"

#define	MAX_OSPATH		260			// max length of a filesystem pathname
#define net_NOP 0        // nop command used for padding
#define net_Disconnect 1 // disconnect, last message in connection
#define net_File 2       // file transmission message request/deny
#define net_Tick 3        // send last world tick
#define net_StringCmd 4   // a string command
#define net_SetConVar 5   // sends one/multiple convar settings
#define net_SignonState 6 // signals current signon state
#define clc_ClientInfo 8
#define	clc_Move 9
#define clc_VoiceData 10 
#define clc_BaselineAck 11
#define clc_ListenEvents 12
#define clc_RespondCvarValue 13
#define clc_FileCRCCheck 14
#define clc_SaveReplay 15
#define clc_CmdKeyValues 16
#define clc_FileMD5Check 17

class	IDemoRecorder;
class	INetChannel;
class	INetChannelHandler;
class	INetChannelInfo;
class	CUDPSocket;
class	INetworkMessageHandler;
typedef struct netpacket_s netpacket_t;

enum
{
	GENERIC = 0,	// must be first and is default group
	LOCALPLAYER,	// bytes for local player entity update
	OTHERPLAYERS,	// bytes for other players update
	ENTITIES,		// all other entity bytes
	SOUNDS,			// game sounds
	EVENTS,			// event messages
	USERMESSAGES,	// user messages
	ENTMESSAGES,	// entity messages
	VOICE,			// voice data
	STRINGTABLE,	// a stringtable update
	MOVE,			// client move cmds
	STRINGCMD,		// string command
	SIGNON,			// various signondata
	TOTAL,			// must be last and is not a real group
};

class INetMessage
{
public:
	virtual ~INetMessage() {};
	virtual void SetNetChannel(INetChannel* netchan) = 0; // netchannel this message is from/for
	virtual void SetReliable(bool state) = 0;             // set to true if it's a reliable message
	virtual bool Process(void) = 0; // calles the recently set handler to process this message
	virtual bool ReadFromBuffer(bf_read& buffer) = 0; // returns true if parsing was OK
	virtual bool WriteToBuffer(bf_write& buffer) = 0; // returns true if writing was OK
	virtual bool IsReliable(void) const = 0; // true, if message needs reliable handling
	virtual int GetType(void) const = 0;         // returns module specific header tag eg svc_serverinfo
	virtual int GetGroup(void) const = 0;        // returns net message group of this message
	virtual const char* GetName(void) const = 0; // returns network message name, eg "svc_serverinfo"
	virtual INetChannel* GetNetChannel(void) const = 0;
	virtual const char* ToString(void) const = 0; // returns a human readable string about message content
};

class CNetMessage : public INetMessage
{
public:
	CNetMessage()
	{
		m_bReliable = true;
		m_NetChannel = nullptr;
	}

	virtual ~CNetMessage() {};

	virtual int GetGroup() const { return GENERIC; }
	INetChannel* GetNetChannel() const { return m_NetChannel; }

	virtual void SetReliable(bool state) { m_bReliable = state; };
	virtual bool IsReliable() const { return m_bReliable; };
	virtual void SetNetChannel(INetChannel* netchan) { m_NetChannel = netchan; }
	virtual bool Process() { return false; }; // no handler set

protected:
	bool m_bReliable;          // true if message should be send reliable
	INetChannel* m_NetChannel; // netchannel this message is from/for
	byte pad0[8];
};

class CLC_Move : public CNetMessage
{
public:
	bool ReadFromBuffer(bf_read& buffer)
	{
		m_nNewCommands = buffer.ReadUBitLong(4);
		m_nBackupCommands = buffer.ReadUBitLong(3);
		m_nLength = buffer.ReadWord();
		m_DataIn = buffer;
		return buffer.SeekRelative(m_nLength);
	}
	bool WriteToBuffer(bf_write& buffer)
	{
		buffer.WriteUBitLong(GetType(), 6);
		m_nLength = m_DataOut.GetNumBitsWritten();

		buffer.WriteUBitLong(m_nNewCommands, 4);
		buffer.WriteUBitLong(m_nBackupCommands, 3);

		buffer.WriteWord(m_nLength);

		return buffer.WriteBits(m_DataOut.GetData(), m_nLength);
	}
	const char* ToString(void) const { return "CLC_Move"; }
	int GetType() const { return clc_Move; }
	const char* GetName() const { return "clc_Move"; }
	void* m_pMessageHandler;
	int GetGroup() const { return MOVE; }
	CLC_Move() { m_bReliable = false; }

public:
	int m_nBackupCommands;
	int m_nNewCommands;
	int m_nLength;
	bf_read m_DataIn;
	bf_write m_DataOut;
};

class NET_SetConVar : public CNetMessage
{
public:
	bool ReadFromBuffer(bf_read& buffer);
	bool WriteToBuffer(bf_write& buffer);
	const char* ToString() const;
	int GetType() const { return net_SetConVar; }
	const char* GetName() const { return "net_SetConVar"; }
	void* m_pMessageHandler;
	int GetGroup() const { return STRINGCMD; }

	NET_SetConVar() {}
	NET_SetConVar(const char* name, const char* value)
	{
		CVar_t cvar;
		strncpy_s(cvar.Name, name, MAX_OSPATH);
		strncpy_s(cvar.Value, value, MAX_OSPATH);
		ConVar = cvar;
	}

public:
	typedef struct CVar_s
	{
		char Name[MAX_OSPATH];
		char Value[MAX_OSPATH];
	} CVar_t;
	CVar_t ConVar;
};

class NET_SignonState : public CNetMessage
{
public:
	bool ReadFromBuffer(bf_read& buffer);
	bool WriteToBuffer(bf_write& buffer);
	const char* ToString() const;
	int GetType() const { return net_SignonState; }
	const char* GetName() const { return "net_SignonState"; }
	void* m_pMessageHandler;

	int	GetGroup() const { return SIGNON; }

	NET_SignonState() {};
	NET_SignonState(int state, int spawncount) { m_nSignonState = state; m_nSpawnCount = spawncount; };

public:
	int m_nSignonState;			// See SIGNONSTATE_ defines
	int m_nSpawnCount;			// server spawn count (session number)
};

class NET_Tick : public CNetMessage
{
public:
	bool ReadFromBuffer(bf_read& buffer);
	bool WriteToBuffer(bf_write& buffer);
	const char* ToString() const;
	int GetType() const { return net_Tick; }
	const char* GetName() const { return "net_Tick"; }
	void* m_pMessageHandler;

	NET_Tick()
	{
		m_bReliable = false;
		m_flHostFrameTime = 0;
		m_flHostFrameTimeStdDeviation = 0;
	};
	NET_Tick(int tick, float hostFrametime, float hostFrametime_stddeviation)
	{
		m_bReliable = false;
		m_nTick = tick;
		m_flHostFrameTime = hostFrametime;
		m_flHostFrameTimeStdDeviation = hostFrametime_stddeviation;
	};

public:
	int			m_nTick;
	float		m_flHostFrameTime;
	float		m_flHostFrameTimeStdDeviation;
};

class CLC_VoiceData : public CNetMessage
{
public:
	bool ReadFromBuffer(bf_read& buffer);
	bool WriteToBuffer(bf_write& buffer);
	const char* ToString() const;
	int GetType() const { return clc_VoiceData; }
	const char* GetName() const { return "clc_VoiceData"; }
	void* m_pMessageHandler;

	int GetGroup() const { return VOICE; }

	CLC_VoiceData() { m_bReliable = false; }

public:
	int m_nLength;
	bf_read m_DataIn;
	bf_write m_DataOut;
	uint64_t m_xuid;
};

class CLC_RespondCvarValue : public CNetMessage
{
public:
	bool ReadFromBuffer(bf_read& buffer);
	bool WriteToBuffer(bf_write& buffer);
	const char* ToString() const;
	int GetType() const { return clc_RespondCvarValue; }
	const char* GetName() const { return "clc_RespondCvarValue"; }
	void* m_pMessageHandler;

	int m_iCookie;

	const char* m_szCvarName;
	const char* m_szCvarValue;	// The sender sets this, and it automatically points it at m_szCvarNameBuffer when receiving.

	int	m_eStatusCode;

private:
	char m_szCvarNameBuffer[256];
	char m_szCvarValueBuffer[256];
};

class SVC_FixAngle : public CNetMessage
{
	SVC_FixAngle() { m_bReliable = false; };
	SVC_FixAngle(bool bRelative, Vector angle) { m_bReliable = false; m_bRelative = bRelative; m_Angle = angle; }

public:
	bool			m_bRelative;
	Vector			m_Angle;
};

class CLC_BaselineAck : public CNetMessage
{
public:
	bool ReadFromBuffer(bf_read& buffer);
	bool WriteToBuffer(bf_write& buffer);
	const char* ToString() const;
	int GetType() const { return clc_BaselineAck; }
	const char* GetName() const { return "clc_BaselineAck"; }
	void* m_pMessageHandler;

	CLC_BaselineAck() {};
	CLC_BaselineAck(int tick, int baseline) { m_nBaselineTick = tick; m_nBaselineNr = baseline; }

	int	GetGroup() const { return ENTITIES; }

public:
	int		m_nBaselineTick;	// sequence number of baseline
	int		m_nBaselineNr;		// 0 or 1 		
};

class NET_StringCmd
{

	char pad[16];
public:
	const char* m_szCommand;	// execute this command

private:
	char		m_szCommandBuffer[1024];	// buffer for received messages
};

typedef enum
{
	NA_NULL = 0,
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
} netadrtype_t;

typedef struct netadr_s
{
public:
	netadrtype_t type;
	unsigned char ip[4];
	unsigned short port;
} netadr_t;

enum ConnectionStatus_t
{
	CONNECTION_STATE_DISCONNECTED = 0,
	CONNECTION_STATE_CONNECTING,
	CONNECTION_STATE_CONNECTION_FAILED,
	CONNECTION_STATE_CONNECTED
};

class INetChannel : public INetChannelInfo
{
public:
	virtual	~INetChannel(void) {};

	virtual void	SetDataRate(float rate) = 0;
	virtual bool	RegisterMessage(INetMessage* msg) = 0;
	virtual bool	StartStreaming(unsigned int challengeNr) = 0;
	virtual void	ResetStreaming(void) = 0;
	virtual void	SetTimeout(float seconds) = 0;
	virtual void	SetDemoRecorder(IDemoRecorder* recorder) = 0;
	virtual void	SetChallengeNr(unsigned int chnr) = 0;

	virtual void	Reset(void) = 0;
	virtual void	Clear(void) = 0;
	virtual void	Shutdown(const char* reason) = 0;

	virtual void	ProcessPlayback(void) = 0;
	virtual bool	ProcessStream(void) = 0;
	virtual void	ProcessPacket(struct netpacket_s* packet, bool bHasHeader) = 0;

	virtual bool	SendNetMsg(INetMessage& msg, bool bForceReliable = false, bool bVoice = false) = 0;
#ifdef POSIX
	FORCEINLINE bool SendNetMsg(INetMessage const& msg, bool bForceReliable = false, bool bVoice = false) { return SendNetMsg(*((INetMessage*)&msg), bForceReliable, bVoice); }
#endif
	virtual bool	SendData(bf_write& msg, bool bReliable = true) = 0;
	virtual bool	SendFile(const char* filename, unsigned int transferID) = 0;
	virtual void	DenyFile(const char* filename, unsigned int transferID) = 0;
	virtual void	RequestFile_OLD(const char* filename, unsigned int transferID) = 0;	// get rid of this function when we version the 
	virtual void	SetChoked(void) = 0;
	virtual int		SendDatagram(bf_write* data) = 0;
	virtual bool	Transmit(bool onlyReliable = false) = 0;

	virtual const netadr_t& GetRemoteAddress(void) const = 0;
	virtual INetChannelHandler* GetMsgHandler(void) const = 0;
	virtual int				GetDropNumber(void) const = 0;
	virtual int				GetSocket(void) const = 0;
	virtual unsigned int	GetChallengeNr(void) const = 0;
	virtual void			GetSequenceData(int& nOutSequenceNr, int& nInSequenceNr, int& nOutSequenceNrAck) = 0;
	virtual void			SetSequenceData(int nOutSequenceNr, int nInSequenceNr, int nOutSequenceNrAck) = 0;

	virtual void	UpdateMessageStats(int msggroup, int bits) = 0;
	virtual bool	CanPacket(void) const = 0;
	virtual bool	IsOverflowed(void) const = 0;
	virtual bool	IsTimedOut(void) const = 0;
	virtual bool	HasPendingReliableData(void) = 0;

	virtual void	SetFileTransmissionMode(bool bBackgroundMode) = 0;
	virtual void	SetCompressionMode(bool bUseCompression) = 0;
	virtual unsigned int RequestFile(const char* filename) = 0;
	virtual float	GetTimeSinceLastReceived(void) const = 0;	// get time since last received packet in seconds

	virtual void	SetMaxBufferSize(bool bReliable, int nBytes, bool bVoice = false) = 0;

	virtual bool	IsNull() const = 0;
	virtual int		GetNumBitsWritten(bool bReliable) = 0;
	virtual void	SetInterpolationAmount(float flInterpolationAmount) = 0;
	virtual void	SetRemoteFramerate(float flFrameTime, float flFrameTimeStdDeviation) = 0;

	// Max # of payload bytes before we must split/fragment the packet
	virtual void	SetMaxRoutablePayloadSize(int nSplitSize) = 0;
	virtual int		GetMaxRoutablePayloadSize() = 0;

	virtual int		GetProtocolVersion() = 0;
};

class CNetChannel : public INetChannel
{
public:
	virtual ConnectionStatus_t GetConnectionState();
	virtual const netadr_t& GetRemoteAddress(void) const;

	ConnectionStatus_t m_ConnectionState;
	int m_nOutSequenceNr;
	int m_nInSequenceNr;
	int m_nOutSequenceNrAck;
	int m_nOutReliableState;
	int m_nInReliableState;
	int m_nChokedPackets;
	int m_PacketDrop;
	bf_write m_StreamReliable;
	byte m_ReliableDataBuffer[8 * 1024];
	CUtlVector<byte> m_ReliableDataBufferMP;
	bf_write m_StreamUnreliable;
	byte m_UnreliableDataBuffer[1400];
	CUDPSocket* m_pSocket;
	int m_StreamSocket;
	unsigned int m_MaxReliablePayloadSize;
	netadr_t remote_address;
	float last_received;
	float connect_time;
	int m_Rate;
	float m_fClearTime;
	float m_Timeout;
	char m_Name[32];
	INetworkMessageHandler* m_MessageHandler;
};

#endif // INETCHANNEL_H