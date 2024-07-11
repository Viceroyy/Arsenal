#pragma once

#include "../../SDK/SDK.h"

namespace WndProcHook
{
	inline HWND hwWindow;
	inline WNDPROC Original;
	LONG __stdcall Func(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Init();
	void Uninit();
}