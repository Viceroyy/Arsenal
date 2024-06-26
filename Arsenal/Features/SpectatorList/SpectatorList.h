#pragma once
#include "../../SDK/SDK.h"

class CFeatures_SpectatorList
{
private:
	struct Spectator_t
	{
		std::wstring Name;
		std::wstring Mode;
		int Team;
		int Index;
	};

	std::vector<Spectator_t> Spectators;

public:
	int SpecListX = 30, SpecListY = 100;
	bool GetSpectators(C_CSPlayer* pLocal);
	void Run();
};

namespace F { inline CFeatures_SpectatorList SpectatorList; }