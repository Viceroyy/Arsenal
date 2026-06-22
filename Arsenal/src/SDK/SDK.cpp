#include "SDK.h"

void SDK::Output(const char* sFunction, const char* sLog, Color_t tColor,
	int iTo, int iMessageBox,
	const char* sLeft, const char* sRight)
{
	if (sLog)
	{
		if (iTo & OUTPUT_CONSOLE)
		{
			I::CVar->ConsoleColorPrintf(tColor, "%s%s%s ", sLeft, sFunction, sRight);
			I::CVar->ConsoleColorPrintf({}, "%s\n", sLog);
		}
		if (iTo & OUTPUT_DEBUG)
			OutputDebugString(std::format("{}{}{} {}\n", sLeft, sFunction, sRight, sLog).c_str());
		if (iMessageBox != -1)
			MessageBox(nullptr, sLog, sFunction, iMessageBox);
	}
	else
	{
		if (iTo & OUTPUT_CONSOLE)
			I::CVar->ConsoleColorPrintf(tColor, "%s\n", sFunction);
		if (iTo & OUTPUT_DEBUG)
			OutputDebugString(std::format("{}\n", sFunction).c_str());
		if (iMessageBox != -1)
			MessageBox(nullptr, "", sFunction, iMessageBox);
	}
}

bool SDK::IsGameWindowInFocus()
{
	static auto CSSWindow = FindWindowA("Valve001", NULL);
	return GetForegroundWindow() == CSSWindow;
}
