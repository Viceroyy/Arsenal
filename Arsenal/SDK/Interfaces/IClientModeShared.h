#pragma once
#include "../Includes/inetchannel.h"

class ClientModeShared
{
public:
	bool IsChatPanelOutOfFocus(void)
	{
		static auto CHudChat = U::VFunc.Get<uintptr_t(__thiscall*)(void*)>(this, 19)(this);

		if (CHudChat)
		{
			return *(float*)(CHudChat + 0xFC) == 0;
		}

		return false;
	}
};

namespace I { inline ClientModeShared* ClientMode = nullptr; }