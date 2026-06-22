#include "Core.h"

#include "../SDK/SDK.h"
#include "../Utils/ConVars/ConVars.h"
#include "../Utils/Hooks/Hooks.h"
#include "../Utils/Interfaces/Interfaces.h"
#include "../Utils/Memory/Memory.h"
#include "../Utils/Signatures/Signatures.h"
#include "../Features/Notification/Notification.h"
#include "../Features/Players/Players.h"
#include "../Features/Materials/Materials.h"
#include "../Features/WorldModulation/WorldModulation.h"
#include "../Features/Commands/Commands.h"

#include <Psapi.h>
#include <fstream>
#include <format>

MAKE_SIGNATURE(GlobalVars, "engine.dll", "48 8B 0D ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B 15", 0x0);

static inline std::string GetProcessName(DWORD dwProcessID)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);
	if (!hProcess)
		return "";

	if (char buffer[MAX_PATH]; GetModuleBaseName(hProcess, nullptr, buffer, sizeof(buffer) / sizeof(char)))
	{
		CloseHandle(hProcess);
		return buffer;
	}

	CloseHandle(hProcess);
	return "";
}

void CCore::AppendFailText(const char* sMessage)
{
	if (m_vFailReasons.empty())
		m_vFailReasons.push_back(std::format("Built @ " __DATE__ ", " __TIME__ ", " __CONFIGURATION__));

	m_vFailReasons.push_back(sMessage);
	OutputDebugStringA(std::format("{}\n", sMessage).c_str());
}

void CCore::LogFailText()
{
	std::stringstream ssFailStream;
	for (const auto& reason : m_vFailReasons)
		ssFailStream << reason << "\n";

	try
	{
		std::ofstream file;
		file.open("Arsenal\\fail_log.txt", std::ios_base::app);
		file << ssFailStream.str() + "\n\n\n";
		file.close();

		ssFailStream << "\n";
		ssFailStream << "Ctrl + C to copy. \n";
		ssFailStream << "Logged to Arsenal\\fail_log.txt. ";
	}
	catch (...) {}

	MessageBoxA(0, ssFailStream.str().c_str(), "Failed to load", MB_OK | MB_ICONERROR);
}

void CCore::Load()
{
	U::ExceptionHandler.Initialize();

	m_nLoadStartTime = GetTickCount64();

	if (m_bUnload = m_bFailed = FNV1A::Hash32(GetProcessName(GetCurrentProcessId()).c_str()) != FNV1A::Hash32Const("hl2.exe"))
	{
		AppendFailText("Invalid process");
		return;
	}

	float flTime = 0.f;
	while (true)
	{
		auto uSignature = U::Memory.FindSignature("engine.dll", "48 8B 0D ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B 15");
		auto uDereference = uSignature ? *reinterpret_cast<uintptr_t*>(U::Memory.RelToAbs(uSignature)) : 0;
		auto hWindow = FindWindowW(L"Valve001", nullptr);
		if (uDereference && hWindow)
			break;

		Sleep(500), flTime += 0.5f;
		if (m_bUnload = m_bFailed = flTime >= 60.f)
		{
			AppendFailText(std::format("Failed to load in time:\n  {:#x} ({:#x})\n  {:#x}", uDereference, uSignature, uintptr_t(hWindow)).c_str());
			return;
		}
		if (m_bUnload = m_bFailed = U::KeyHandler.Down(VK_F11, true))
		{
			AppendFailText("Cancelled load");
			return;
		}
	}
	Sleep(500);

	if (!U::Signatures.Initialize())
	{
		m_bFailed = true;
		return;
	}

	{
		I::BaseClientDLL = U::Memory.FindInterface<IBaseClientDLL*>("client.dll", "VClient017");
		I::ClientEntityList = U::Interface.Get<IClientEntityList*>("client.dll", "VClientEntityList003");
		I::GameMovement = U::Interface.Get<IGameMovement*>("client.dll", "GameMovement001");
		I::ClientPrediction = U::Interface.Get<CPrediction*>("client.dll", "VClientPrediction001");
		I::EngineClient = U::Interface.Get<IVEngineClient*>("engine.dll", "VEngineClient014");
		I::ModelInfoClient = U::Interface.Get<IVModelInfoClient*>("engine.dll", "VModelInfoClient006");
		I::EngineVGui = U::Interface.Get<IEngineVGui*>("engine.dll", "VEngineVGui002");
		I::InputSystem = U::Interface.Get<IInputSystem*>("inputsystem.dll", "InputSystemVersion001");
		I::EngineTrace = U::Interface.Get<IEngineTrace*>("engine.dll", "EngineTraceClient003");
		I::RenderView = U::Interface.Get<IVRenderView*>("engine.dll", "VEngineRenderView014");
		I::ModelRender = U::Interface.Get<IVModelRender*>("engine.dll", "VEngineModel016");
		I::GameEventManager = U::Interface.Get<IGameEventManager2*>("engine.dll", "GAMEEVENTSMANAGER002");
		I::StudioRender = U::Interface.Get<IStudioRender*>("studiorender.dll", "VStudioRender025");
		I::MatSystemSurface = U::Interface.Get<IMatSystemSurface*>("vguimatsurface.dll", "VGUI_Surface030");
		I::MaterialSystem = U::Interface.Get<IMaterialSystem*>("MaterialSystem.dll", "VMaterialSystem082");
		I::Cvar = U::Interface.Get<ICvar*>("vstdlib.dll", "VEngineCvar004");
		I::ClientState = *reinterpret_cast<CClientState**>(U::Pattern.Find("engine.dll", "48 8D 0D ? ? ? ? E8 ? ? ? ? F3 0F 5E 05"));

		{
			const DWORD_PTR dwpFunc = U::VFunc.Get<DWORD_PTR>(I::BaseClientDLL, 10u);
			XASSERT(dwpFunc == NULL);

			I::ClientMode = *reinterpret_cast<ClientModeShared**>(*reinterpret_cast<DWORD*>(dwpFunc + 0x03) + dwpFunc + 0x07);
			I::GlobalVars = reinterpret_cast<CGlobalVarsBase*>(*reinterpret_cast<DWORD*>(S::GlobalVars() + 0xA) + S::GlobalVars() + 0xE);

			XASSERT(I::ClientMode == nullptr);
			XASSERT(I::GlobalVars == nullptr);
		}
	}

	U::ConVars.Initialize();
	H::Draw.Initialize();

	if (m_bUnload = m_bFailed2 = !U::Hooks.Initialize())
	{
		AppendFailText("Failed to initialize hooks");
		return;
	}
	F::Commands.Initialize();
	F::Players.Parse();

	Config::Load((std::filesystem::current_path().string() + "\\Arsenal\\default.json").c_str());
	F::Notifications.Add("Config default loaded!");

	I::MatSystemSurface->PlaySound("hl1/fvox/activated.wav");
	I::Cvar->ConsoleColorPrintf({ 175, 150, 255, 255 }, "[Arsenal] Loaded!\n");
}

void CCore::Loop()
{
	while (true)
	{
		bool bShouldUnload = U::KeyHandler.Down(VK_F11, true) && GetForegroundWindow() == FindWindowW(L"Valve001", nullptr) || m_bUnload;
		if (bShouldUnload)
			break;

		Sleep(15);
	}
}

void CCore::Unload()
{
	if (m_bFailed)
	{
		U::ExceptionHandler.Unload();
		LogFailText();
		return;
	}

	if (!bUnload)
	{
		G.Unload = true;

		CFG::Visuals_FOV = 90;
		CFG::Visuals_ViewmodelFOV = 70;

		m_bFailed2 = !U::Hooks.Unload() || m_bFailed2;
		U::ConVars.Unload();

		Sleep(250);

		F::Materials.CleanUp();
		F::WorldModulation.RestoreWorldModulation();
		H::Draw.Uninitialize();

		Sleep(250);
	}

	I::MatSystemSurface->PlaySound("hl1/fvox/deactivated.wav");
	I::Cvar->ConsoleColorPrintf({ 175, 150, 255, 255 }, "[Arsenal] Unloaded!\n");

	U::ExceptionHandler.Unload();

	if (m_bFailed2)
	{
		LogFailText();
		return;
	}
}
