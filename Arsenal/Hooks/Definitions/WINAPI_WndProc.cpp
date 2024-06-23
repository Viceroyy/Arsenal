#include "WINAPI_WndProc.h"
#include "../../SDK/Input/Input.h"
#include "../../Features/Menu/Menu.h"

LONG __stdcall WINAPI_HOOK::WINAPI_WndProc::Func(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

void WINAPI_HOOK::WINAPI_WndProc::Init()
{
	Original = (WNDPROC)SetWindowLongPtr(hwWindow = FindWindowA("Valve001", NULL), GWL_WNDPROC, (LONG_PTR)Func);
}