#pragma once
#include "Definitions/Definitions.h"
#include "Definitions/Main/CUserCmd.h"
#include "../Utils/Signatures/Signatures.h"
#include "../Utils/Memory/Memory.h"
#include <unordered_map>

class CGlobals
{
public:
	bool Unload = false;

	float Lerp = 0.015f;
	float FOV = 90.f;

	int SeqNum = 0;
};

inline CGlobals G;