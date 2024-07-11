#include "WINAPI_WndProc.h"
#include "../../SDK/Input/Input.h"
#include "../../Features/Menu/Menu.h"

LONG __stdcall WndProcHook::Func(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (F::Menu.IsOpen() && H::Input.IsGameFocused())
	{
		if (GetAsyncKeyState(VK_SNAPSHOT) & 1)
			CallWindowProc(Original, hWnd, uMsg, wParam, lParam);

		I::InputSystem->ResetInputState();

		return 1;
	}

	return CallWindowProc(Original, hWnd, uMsg, wParam, lParam);
}

void WndProcHook::Init()
{
	Original = (WNDPROC)SetWindowLongPtr(hwWindow = FindWindowA("Valve001", NULL), GWLP_WNDPROC, (LONG_PTR)Func);
}

void WndProcHook::Uninit()
{
	SetWindowLongPtr(hwWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Original));
}