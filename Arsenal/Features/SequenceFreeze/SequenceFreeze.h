#pragma once
#include "../../SDK/SDK.h"

class CFeatures_SequenceFreeze
{
public:
	void Run(C_CSPlayer* pLocal, CUserCmd* cmd, bool pSendPacket);

};

namespace F { inline CFeatures_SequenceFreeze SequenceFreeze; }