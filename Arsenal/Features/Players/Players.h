#pragma once
#include <unordered_map>
#include <string>

struct PlayerPriority
{
	bool Ignored{};
	bool Cheater{};
	bool RetardLegit{};
};

class CPlayers
{
	struct Player
	{
		uint32_t SteamID = {};
		PlayerPriority Info = {};
	};

	std::unordered_map<uint32_t, PlayerPriority> m_Players;
	std::string m_LogPath;

public:
	void Parse();
	void Mark(int entindex, const PlayerPriority& info);
	bool GetInfo(int entindex, PlayerPriority& out);
	bool GetInfoGUID(const std::string& guid, PlayerPriority& out);
};

namespace F { inline CPlayers Players; }