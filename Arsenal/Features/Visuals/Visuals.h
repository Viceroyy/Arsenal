#pragma once
#include "../../SDK/SDK.h"

class CFeatures_Visuals
{
public:
	void Run();

	void DoFOV(CViewSetup* pSetup);
	void DoThirdperson(CViewSetup* pSetup);
	void ManualNetwork(const StartSoundParams_t& params);

private:
	void SpreadCircle();
};

namespace F { inline CFeatures_Visuals Visuals; }