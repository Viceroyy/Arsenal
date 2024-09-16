#pragma once
#include "C_BaseAnimating.h"

class C_PlantedC4 : public C_BaseEntity
{
public:
	NETVAR(m_bBombTicking, bool, "CPlantedC4", "m_bBombTicking");
	NETVAR(m_flC4Blow, float, "CPlantedC4", "m_flC4Blow");
	NETVAR(m_flTimerLength, float, "CPlantedC4", "m_flTimerLength");
	NETVAR(m_flDefuseLength, float, "CPlantedC4", "m_flDefuseLength");
	NETVAR(m_flDefuseCountDown, float, "CPlantedC4", "m_flDefuseCountDown");
};