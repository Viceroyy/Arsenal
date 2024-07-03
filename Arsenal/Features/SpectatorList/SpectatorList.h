#pragma once
#include "../../SDK/SDK.h"

class CFeatures_SpectatorList
{
private:
	struct Spectator_t
	{
		std::wstring Name;
		std::wstring Mode;
	};

	std::vector<Spectator_t> Spectators;

public:
	bool GetSpectators(C_CSPlayer* pLocal);
	void Run();
};

namespace F { inline CFeatures_SpectatorList SpectatorList; }