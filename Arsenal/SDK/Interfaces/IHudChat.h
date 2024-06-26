#pragma once
#include "../Includes/inetchannel.h"

class CHudChat
{
public:

	void Printf(const char* fmt, ...)
	{
		va_list va_alist;
		char buf[256];
		va_start(va_alist, fmt);
		_vsnprintf_s(buf, sizeof(buf), fmt, va_alist);
		va_end(va_alist);

		using Type = void(__cdecl*)(void*, int, int, const char*);

		U::VFunc.Get<Type>(this, 18)(this, 0, 0, buf);
	}
};

namespace I { inline CHudChat* HudChat = nullptr; }