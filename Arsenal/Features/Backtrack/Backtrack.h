#pragma once

#include "../../SDK/SDK.h"
#include <deque>

#pragma warning ( disable : 4091 )

class CIncomingSequence
{
public:
	int InReliableState;
	int SequenceNr;
	float CurTime;

	CIncomingSequence(int inState, int seqNr, float time)
	{
		InReliableState = inState;
		SequenceNr = seqNr;
		CurTime = time;
	}
};

using BoneMatrixes = struct
{
	float BoneMatrix[128][3][4];
};

struct TickRecord
{
	float flSimTime = 0.f;
	float flCreateTime = 0.f;
	int iTickCount = 0;
	bool bOnShot = false;
	BoneMatrixes BoneMatrix{};
	Vector vOrigin = {};
	bool bInvalid = false;
};

class CBacktrack
{
	// logic
	bool WithinRewind(const TickRecord& record);

	// utils
	void UpdateDatagram();
	void StoreNolerp();
	void MakeRecords();
	void CleanRecords();

	// data
	std::unordered_map<int, bool> mDidShoot;
	int iLastCreationTick = 0;

	// data - fake latency
	std::deque<CIncomingSequence> dSequences;
	int iLastInSequence = 0;

	bool bLastTickHeld = false;

public:
	float GetFake();
	float GetReal(int iFlow = -1);

	std::deque<TickRecord>* GetRecords(C_BaseEntity* pEntity);
	std::deque<TickRecord> GetValidRecords(std::deque<TickRecord>* pRecords, C_CSPlayer* pLocal = nullptr, bool bDistance = false);

	void Restart();
	void FrameStageNotify();
	void AdjustPing(CNetChannel* netChannel);

	std::optional<TickRecord> GetHitRecord(CUserCmd* pCmd, C_CSPlayer* pEntity, const Vector vAngles, const Vector vPos);
	void BacktrackToCrosshair(CUserCmd* pCmd);

	bool bFakeLatency = false;
	float flWishInterp = 0.015f;
	float flFakeInterp = 0.015f;
	std::unordered_map<C_BaseEntity*, std::deque<TickRecord>> mRecords;
	std::unordered_map<C_BaseEntity*, std::pair<int, matrix3x4_t[128]>> mBones;
	std::unordered_map<C_BaseEntity*, Vector> mEyeAngles;
	std::unordered_map<C_BaseEntity*, bool> mLagCompensation;

	bool bSettingUpBones = false;

	int iTickCount = 0;
	float flMaxUnlag = 1.f;
};

namespace F { inline CBacktrack Backtrack; }