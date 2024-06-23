//#pragma once
//#include "../../SDK/SDK.h"
//#include <deque>
//#include <unordered_map>
//
//#pragma warning ( disable : 4091 )
//
//using BoneMatrixes = struct
//{
//	float BoneMatrix[128][3][4];
//};
//
//struct TickRecord
//{
//	float flSimTime = 0.f;
//	float flCreateTime = 0.f;
//	int iTickCount = 0;
//	bool bOnShot = false;
//	BoneMatrixes BoneMatrix{};
//	Vector vOrigin = {};
//	bool bInvalid = false;
//};
//
//class CFeatures_Backtrack
//{
//	// logic
//	bool WithinRewind(const TickRecord& record);
//
//	// utils
//	void StoreNolerp();
//	void MakeRecords();
//	void CleanRecords();
//
//	// data
//	std::unordered_map<int, bool> mDidShoot;
//	int iLastCreationTick = 0;
//
//public:
//	float GetReal(int iFlow = -1);
//	std::deque<TickRecord>* GetRecords(C_BaseEntity* pEntity);
//	std::deque<TickRecord> GetValidRecords(std::deque<TickRecord>* pRecords, C_CSPlayer* pLocal = nullptr, bool bDistance = false);
//
//	void Restart();
//	void FrameStageNotify();
//
//	float flFakeInterp = 0.015f;
//	std::unordered_map<C_BaseEntity*, std::deque<TickRecord>> mRecords;
//	std::unordered_map<C_BaseEntity*, std::pair<int, matrix3x4_t[128]>> mBones;
//	std::unordered_map<C_BaseEntity*, Vector> mEyeAngles;
//	std::unordered_map<C_BaseEntity*, bool> mLagCompensation;
//
//	bool bSettingUpBones = false;
//
//	int iTickCount = 0;
//	float flMaxUnlag = 1.f;
//};
//
//namespace F { inline CFeatures_Backtrack Backtrack; }