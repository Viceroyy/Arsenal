#pragma once
#include "../Macros/Macros.h"
#include "../../SDK/SDK.h"
#include <chrono>

class Timer
{
private:
	float m_flLast = 0.f;

public:
	Timer();
	bool Check(float flS) const;
	bool Run(float flS);
	inline void Update()
	{
		m_flLast = Util::PlatFloatTime();
	}
};
