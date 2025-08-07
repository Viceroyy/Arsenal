#include "Entry/Entry.h"

DWORD WINAPI MainThread(LPVOID lpParam)
{
	while (!GetModuleHandleW(L"ServerBrowser.dll"))
		Sleep(2000);

	U::Entry.Load();

	while (!(GetAsyncKeyState(VK_F11) & 0x8000))
		Sleep(50);

	U::Entry.Unload();

	FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), EXIT_SUCCESS);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		if (const auto hMainThread = CreateThread(nullptr, 0, MainThread, hinstDLL, 0, nullptr))
			CloseHandle(hMainThread);
	}

	return TRUE;
}