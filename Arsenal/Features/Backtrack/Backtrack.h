#pragma once

#include "../../SDK/SDK.h"
#include <deque>

struct LagRecord_t
{
	C_CSPlayer* m_pPlayer = nullptr;
	matrix3x4_t m_BoneMatrix[128] = {};
	float m_flSimulationTime = -1.0f;
	Vector m_vAbsOrigin = {};
	Vector m_vVecOrigin = {};
	Vector m_vAbsAngles = {};
	Vector m_vEyeAngles = {};
	Vector m_vVelocity = {};
	Vector m_vCenter = {};
	int m_nFlags = 0;
	float m_flFeetYaw = 0.0f;
};

class CLagRecords
{
private:
	std::unordered_map<C_CSPlayer*, std::deque<LagRecord_t>> m_LagRecords = {};
	bool m_bSettingUpBones = false;

	void EraseRecord(C_CSPlayer* pPlayer, int nRecord);
	void EraseAllRecords(C_CSPlayer* pPlayer);

private:
	bool IsSimulationTimeValid(float flCurSimTime, float flCmprSimTime);

public:
	void AddRecord(C_CSPlayer* pPlayer);
	const LagRecord_t* GetRecord(C_CSPlayer* pPlayer, int nRecord, bool bSafe = false);
	bool HasRecords(C_CSPlayer* pPlayer, int* pTotalRecords = nullptr);
	void UpdateRecords();
	bool DiffersFromCurrent(const LagRecord_t* pRecord);
	inline bool IsSettingUpBones() { return m_bSettingUpBones; }
};

namespace F { inline CLagRecords LagRecords; }

class CLagRecordMatrixHelper
{
private:
	C_CSPlayer* m_pPlayer = nullptr;
	Vector m_vAbsOrigin = {};
	Vector m_vAbsAngles = {};
	matrix3x4_t m_BoneMatrix[128] = {};

	bool m_bSuccessfullyStored = false;

public:
	void Set(const LagRecord_t* pRecord);
	void Restore();
};

namespace F { inline CLagRecordMatrixHelper LagRecordMatrixHelper; }