#pragma once
#include "../../Util/Math/Math.h"
#include <unordered_map>

struct DormantData
{
	Vector Location;
	float LastUpdate = 0.f;
};

class CGlobals
{
public:
	bool Unload = false;

	float Lerp = 0.015f;
	float FOV = 90.f;

	int SeqNum = 0;

	std::unordered_map<int, DormantData> DormancyMap = {};
};

inline CGlobals G;