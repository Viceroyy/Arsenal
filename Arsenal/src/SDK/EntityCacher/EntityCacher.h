#pragma once
#include "../Helpers/Draw/Draw.h"
#include <unordered_map>
#include <deque>

enum struct EGroupType
{
	GROUP_INVALID = -1,
	PLAYERS_ALL, PLAYERS_ENEMIES, PLAYERS_TEAMMATES,
	WORLD_C4PLANTED, WORLD_WEAPONS
};

struct DormantData
{
	Vector Location;
	float LastUpdate = 0.f;
};

struct VelFixRecord
{
	Vector m_vecOrigin;
	float m_flSimulationTime;
};

class CEntities
{
	C_CSPlayer* m_pLocal = nullptr;
	C_WeaponCSBase* m_pLocalWeapon = nullptr;
	C_CSPlayerResource* m_pPlayerResource = nullptr;

	std::unordered_map<EGroupType, std::vector<C_BaseEntity*>> m_mGroups = {};

	std::unordered_map<int, float> m_mSimTimes = {}, m_mOldSimTimes = {}, m_mDeltaTimes = {}, m_mLagTimes = {};
	std::unordered_map<int, int> m_mChokes = {}, m_mSetTicks = {};
	std::unordered_map<int, std::pair<bool, matrix3x4_t[MAXSTUDIOBONES]>> m_mBones = {};
	std::unordered_map<int, Vector> m_mOldAngles = {}, m_mEyeAngles = {};
	std::unordered_map<int, bool> m_mLagCompensation = {};
	std::unordered_map<int, DormantData> m_mDormancy = {};
	std::unordered_map<int, Vector> m_mAvgVelocities = {};
	std::unordered_map<int, uint32_t> m_mModels = {};
	std::unordered_map<int, std::deque<VelFixRecord>> m_mOrigins = {};

	bool m_bSettingUpBones = false;

public:
	void Store();
	void Clear(bool bShutdown = false);
	void ManualNetwork(const StartSoundParams_t& params);

	C_CSPlayer* GetLocal();
	C_WeaponCSBase* GetWeapon();
	C_CSPlayerResource* GetPR();

	const std::vector<C_BaseEntity*>& GetGroup(const EGroupType& Group);

	float GetSimTime(int iIndex);
	float GetOldSimTime(int iIndex);
	float GetDeltaTime(int iIndex);
	float GetLagTime(int iIndex);
	int GetChoke(int iIndex);
	matrix3x4_t* GetBones(int iIndex);
	Vector GetEyeAngles(int iIndex);
	Vector GetPingAngles(int iIndex);
	bool GetLagCompensation(int iIndex);
	void SetLagCompensation(int iIndex, bool bLagComp);
	bool GetDormancy(int iIndex);
	Vector* GetAvgVelocity(int iIndex);
	void SetAvgVelocity(int iIndex, Vector vAvgVelocity);
	uint32_t GetModel(int iIndex);
	std::deque<VelFixRecord>* GetOrigins(int iIndex);

	bool IsSettingUpBones();
};

namespace H { inline CEntities Entities; }