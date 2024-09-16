#include "Entry/Entry.h"

DWORD APIENTRY MainThread(LPVOID lpParam)
{
	while (!GetModuleHandleW(L"mss32.dll"))
		Sleep(2000);

	U::Entry.Load();

	while (!(GetAsyncKeyState(VK_F11) & 0x8000))
		Sleep(50);

	U::Entry.Unload();

	FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), EXIT_SUCCESS);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	static bool s_bAttached = false;

	if ((fdwReason == DLL_PROCESS_ATTACH) && !s_bAttached)
	{
		DisableThreadLibraryCalls(hinstDLL);
		if (const HANDLE hMain = CreateThread(nullptr, 0, MainThread, hinstDLL, 0, nullptr))
		{
			s_bAttached = true; CloseHandle(hMain);
		}
	}

	return TRUE;
}