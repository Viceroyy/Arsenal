#pragma once
#include "../../SDK/SDK.h"

class CFeatures_PlayerArrows
{
private:
	void DrawArrowTo(const Vector& vecFromPos, const Vector& vecToPos, Color_t color);

public:
	void Run();
};

namespace F { inline CFeatures_PlayerArrows PlayerArrows; }