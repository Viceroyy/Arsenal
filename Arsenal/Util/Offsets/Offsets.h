#pragma once

#include "../Pattern/Pattern.h"

class CUtil_Offsets
{
public:
	void Initialize();

public:
	DWORD ClientModeShared_CreateMove = 0x0;
	DWORD C_BasePlayer_CalcPlayerView = 0x0;
	DWORD C_BaseEntity_SetPredictionRandomSeed = 0x0;
	DWORD C_BasePlayer_UsingStandardWeaponsInVehicle = 0x0;
	DWORD C_BasePlayer_UpdateButtonState = 0x0;
	DWORD PredictionPlayer = 0x0;
	DWORD ClientTable = 0x0;

	DWORD C_ClientState = 0x0;
	DWORD GlobalVars = 0x0;

	DWORD StartDrawing = 0x0;
	DWORD FinishDrawing = 0x0;
};

namespace U { inline CUtil_Offsets Offsets; }