#pragma once
#include "../../SDK/SDK.h"

class CFeatures_ESP
{
public:
	void Run();

private:
	bool GetDynamicBounds(C_BaseEntity* pEntity, int& x, int& y, int& w, int& h);

	void DrawPlayers(C_CSPlayer* pLocal);
	void DrawWorld();
};

namespace F { inline CFeatures_ESP ESP; }