#include "Events.h"

#include "../../Core/Core.h"

bool CEventListener::Initialize()
{
	static const char* szEvents[] = {
		"client_beginconnect", "client_connected", "client_disconnect", "game_newmap", "teamplay_round_start", "player_connect_client", "player_spawn", "player_hurt", "vote_cast", "item_pickup"
	};
	constexpr int nEventCount = sizeof(szEvents) / sizeof(szEvents[0]);

	for (int i = 0; i < nEventCount; i++)
	{
		const char* szEvent = szEvents[i];
		I::GameEventManager->AddListener(this, szEvent, false);

		if (!I::GameEventManager->FindListener(this, szEvent))
		{
			U::Core.AppendFailText(std::format("Failed to add listener: {}", szEvent).c_str());
			m_bFailed = true;
		}
	}

	return !m_bFailed;
}

void CEventListener::Unload()
{
	I::GameEventManager->RemoveListener(this);
}

void CEventListener::FireGameEvent(IGameEvent* pEvent)
{
	if (!pEvent)
		return;
}
