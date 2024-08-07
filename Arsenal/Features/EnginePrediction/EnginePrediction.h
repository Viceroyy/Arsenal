#pragma once

#include "../../SDK/SDK.h"

class CFeatures_EnginePrediction
{
public:
	void Start(C_CSPlayer* pLocal, CUserCmd* cmd);
	void Finish(C_CSPlayer* pLocal, CUserCmd* cmd);

private:
	int GetTickbase(C_CSPlayer* pLocal, CUserCmd* cmd);

private:
	CMoveData m_MoveData = { };
	float m_fOldCurrentTime = 0.0f;
	float m_fOldFrameTime = 0.0f;
	int m_nOldTickCount = 0;
};

namespace F { inline CFeatures_EnginePrediction EnginePrediction; }