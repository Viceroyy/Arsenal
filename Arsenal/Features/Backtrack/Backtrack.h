#pragma once

#include "../../SDK/SDK.h"
#include <deque>

//#pragma warning ( disable : 4091 )

class CIncomingSequence
{
public:
	int m_nInReliableState;
	int m_nSequenceNr;
	float m_flTime;

	CIncomingSequence(int iState, int iSequence, float flTime)
	{
		m_nInReliableState = iState;
		m_nSequenceNr = iSequence;
		m_flTime = flTime;
	}
};

struct BoneMatrix
{
	matrix3x4_t m_aBones[MAXSTUDIOBONES];
};

struct TickRecord
{
	float m_flSimTime = 0.f;
	Vector m_vOrigin = {};
	Vector m_vMins = {};
	Vector m_vMaxs = {};
	BoneMatrix m_BoneMatrix = {};
	bool m_bOnShot = false;
	Vector m_vBreak = {};
	bool m_bInvalid = false;
};

struct RecordWithFOV
{
	TickRecord record;
	float fov;
};

class CBacktrack
{
	void UpdateDatagram();
	void MakeRecords();
	void CleanRecords();

	std::unordered_map<C_BaseEntity*, std::deque<TickRecord>> m_mRecords = {};
	std::unordered_map<int, bool> m_mDidShoot = {};

	std::deque<CIncomingSequence> m_dSequences;
	int m_iLastInSequence = 0;
	int m_nOldInSequenceNr = 0;
	int m_nOldInReliableState = 0;
	int m_nLastInSequenceNr = 0;
	int m_nOldTickBase = 0;
	float m_flMaxUnlag = 1.f;

	float m_flFakeLatency = 0.f;
	float m_flFakeInterp = 0.015f;

public:
	void Store();
	//void Draw(C_CSPlayer* pLocal);
	std::optional<RecordWithFOV> GetBestRecordWithFOV(C_CSPlayer* pEntity, const Vector& vAngles, const Vector& vPos) const noexcept;
	std::optional<TickRecord> GetHitRecord(C_CSPlayer* pEntity, const Vector& vAngles, const Vector& vPos);
	void BacktrackToCrosshair(CUserCmd* pCmd);
	void Reset();

	bool GetRecords(C_BaseEntity* pEntity, std::vector<TickRecord*>& vReturn);
	std::vector<TickRecord*> GetValidRecords(std::vector<TickRecord*>& vRecords, C_CSPlayer* pLocal = nullptr, bool bDistance = false, float flTimeMod = 0.f);

	float GetReal(int iFlow = MAX_FLOWS, bool bNoFake = true);
	float GetWishFake();
	float GetFakeLatency();
	float GetFakeInterp();
	float GetWindow();

	void AdjustPing(CNetChannel* netChannel);
	void RestorePing(CNetChannel* netChannel);

	int m_iTickCount = 0;

	TickRecord m_tRecord = {}; // for temporary use
};

namespace F { inline CBacktrack Backtrack; }