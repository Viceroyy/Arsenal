#pragma once

#include "../../SDK/SDK.h"

namespace WINAPI_HOOK
{
	namespace WINAPI_WndProc
	{
		inline HWND hwWindow;
		inline WNDPROC Original;
		LONG __stdcall Func(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void Init();
	}
}