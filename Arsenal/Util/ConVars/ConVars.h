#pragma once
#include "../Hash/Hash.h"
#include <unordered_map>
#include "../../SDK/Interfaces/ICvar.h"

class CConVars
{
private:
	std::unordered_map<FNV1A_t, ConVar*> mCVarMap;
	std::unordered_map<ConCommandBase*, int> mFlagMap;

public:
	void Initialize();
	void Unload();
	ConVar* FindVar(const char* cvarname);
};

namespace U { inline CConVars ConVars; }