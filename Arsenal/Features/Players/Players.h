#pragma once
#include <string>

namespace PlayerManager
{
	struct PlayerInfo
	{
		bool m_bIgnored{};
		bool m_bCheater{};
		bool m_bRetardLegit{};
	};
	
	void Parse();
	void Mark(int entindex, const PlayerInfo& info);
	bool GetInfo(int entindex, PlayerInfo& out);
	bool GetInfoGUID(const std::string& guid, PlayerInfo& out);
}