#pragma once
#include "../../SDK/SDK.h"

class CFeatures_Visual
{
public:
	void Run();

private:
	void SpreadCircle();
	void Crosshair();
};

namespace F { inline CFeatures_Visual Visual; }