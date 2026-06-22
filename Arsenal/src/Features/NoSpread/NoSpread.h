#pragma once

#include "../../SDK/SDK.h"
/*
class CNoSpread
{
private:
	int GetSeed(CUserCmd* pCmd);
	float CalcMantissaStep(float flV);
	std::string GetFormat(int iServerTime);

	bool m_bWaitingForPlayerPerf = false;
	int m_bSynced = 0;
	double m_dRequestTime = 0.0;
	float m_flServerTime = 0.f;
	double m_dTimeDelta = 0.0;
	std::deque<double> m_vTimeDeltas = {};

public:
	void Reset();
	bool ShouldRun(C_CSPlayer* pLocal, C_WeaponCSBase* pWeapon, CUserCmd* cmd);

	void AskForPlayerPerf();
	bool ParsePlayerPerf(std::string sMsg);

	void Run(C_CSPlayer* pLocal, C_WeaponCSBase* pWeapon, CUserCmd* pCmd);
	void Draw(C_CSPlayer* pLocal);

	int m_iSeed = 0;
	float m_flMantissaStep = 0.f;
};

namespace F { inline CNoSpread NoSpread; }*/