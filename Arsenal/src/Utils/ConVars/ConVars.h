#pragma once
#include "../Macros/Macros.h"
#include "../Hash/Hash.h"
#include <unordered_map>
#include "../../SDK/Definitions/Interfaces/ICVar.h"

class CConVars
{
private:
	std::unordered_map<uint32_t, ConVar*> mCVarMap = {};
	std::unordered_map<ConCommandBase*, int> mFlagMap = {};

public:
	void Initialize();
	void Unload();
	ConVar* FindVar(const char* sCVar);
};

ADD_FEATURE_CUSTOM(CConVars, ConVars, U);
