#pragma once
#include "../../SDK/SDK.h"

struct Notification_t
{
	std::string m_sText;
	Color_t m_cColor;
	float m_flTime;
};

class CFeatures_Notifications
{
	std::vector<Notification_t> vNotifications;
	int iMaxNotifySize = 8;

public:
	void Add(const std::string& sText, Color_t cColor = { 255, 255, 255, 255 }, float flLifeTime = 5.f);
	void Draw();
};

namespace F { inline CFeatures_Notifications Notifications; }