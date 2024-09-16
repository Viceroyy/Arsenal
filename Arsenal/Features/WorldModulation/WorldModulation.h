#pragma once

#include "../../SDK/SDK.h"

class CWorldModulation
{
private:
	bool m_bWorldWasModulated = false;

private:
	void applyModulation(Color_t clr, bool world, bool sky);

public:
	void UpdateWorldModulation();
	void RestoreWorldModulation();

	inline void LevelShutdown() { m_bWorldWasModulated = false; }
};

namespace F { inline CWorldModulation WorldModulation; }