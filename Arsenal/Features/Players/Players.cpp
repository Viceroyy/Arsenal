#include "Players.h"
#include "../../SDK/SDK.h"

struct Player
{
	hash::hash_t m_iSteamID{};
	PlayerManager::PlayerInfo m_Info{};
};

std::vector<Player> m_vecPlayers{};
std::string m_szLogPath{};

void PlayerManager::Parse()
{
	if (m_szLogPath.empty())
	{
		m_szLogPath = std::filesystem::current_path().string() + "\\Arsenal";

		if (!std::filesystem::exists(m_szLogPath))
		{
			std::filesystem::create_directories(m_szLogPath);
		}

		m_szLogPath += "\\players.json";

		if (!std::filesystem::exists(m_szLogPath))
		{
			std::ofstream file(m_szLogPath, std::ios::app);

			if (!file.is_open())
			{
				return;
			}

			file.close();
		}
	}

	if (!m_vecPlayers.empty())
	{
		return;
	}

	std::ifstream file(m_szLogPath);

	if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof())
	{
		return;
	}

	nlohmann::json j{};

	file >> j;

	for (const auto& item : j.items())
	{
		Player p
		{
			HASH_RT(item.key().c_str()),
			{
				j[item.key()]["ignored"].get<bool>(),
				j[item.key()]["cheater"].get<bool>(),
				j[item.key()]["retardlegit"].get<bool>()
			}
		};

		m_vecPlayers.push_back(p);
	}
}

void PlayerManager::Mark(int entindex, const PlayerInfo& info)
{
	if (entindex == I::EngineClient->GetLocalPlayer())
	{
		return;
	}

	player_info_t player_info{};

	if (!I::EngineClient->GetPlayerInfo(entindex, &player_info) || player_info.fakeplayer)
	{
		return;
	}

	Player* ptr{};

	auto steam_id{ HASH_RT(std::string_view(player_info.guid).data()) };

	for (auto& pl : m_vecPlayers)
	{
		if (pl.m_iSteamID != steam_id)
		{
			continue;
		}

		ptr = &pl;

		break;
	}

	if (!ptr)
	{
		m_vecPlayers.push_back({ steam_id, info });

		ptr = &m_vecPlayers.back();
	}

	ptr->m_Info = info;

	nlohmann::json j{};

	std::ifstream read_file(m_szLogPath);

	if (read_file.is_open() && read_file.peek() != std::ifstream::traits_type::eof())
	{
		read_file >> j;
	}

	read_file.close();

	std::ofstream file(m_szLogPath);

	if (!file.is_open())
	{
		return;
	}

	auto key{ std::string(player_info.guid) };

	j[key]["ignored"] = ptr->m_Info.m_bIgnored;
	j[key]["cheater"] = ptr->m_Info.m_bCheater;
	j[key]["retardlegit"] = ptr->m_Info.m_bRetardLegit;

	if (!ptr->m_Info.m_bIgnored && !ptr->m_Info.m_bCheater && !ptr->m_Info.m_bRetardLegit)
	{
		j.erase(std::string(player_info.guid));
	}

	file << std::setw(4) << j;

	file.close();
}

bool PlayerManager::GetInfo(int entindex, PlayerInfo& out)
{
	if (entindex == I::EngineClient->GetLocalPlayer())
	{
		return false;
	}

	player_info_t player_info{};

	if (!I::EngineClient->GetPlayerInfo(entindex, &player_info) || player_info.fakeplayer)
	{
		return false;
	}

	auto steam_id{ HASH_RT(std::string_view(player_info.guid).data()) };

	for (const auto& pl : m_vecPlayers)
	{
		if (pl.m_iSteamID != steam_id)
		{
			continue;
		}

		out = pl.m_Info;

		return true;
	}

	return false;
}

bool PlayerManager::GetInfoGUID(const std::string& guid, PlayerInfo& out)
{
	auto steam_id{ HASH_RT(guid.c_str()) };

	for (const auto& pl : m_vecPlayers)
	{
		if (pl.m_iSteamID != steam_id)
		{
			continue;
		}

		out = pl.m_Info;

		return true;
	}

	return false;
}