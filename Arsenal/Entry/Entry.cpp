#include "Entry.h"
#include "../Hooks/Definitions/WINAPI_WndProc.h"
#include "../Features/Notification/Notification.h"

//#include <iostream>
//#include <fstream>
//
//std::ofstream File;
//const char *szClassName;
//
//void DumpTable(RecvTable *pTable, int nDepth)
//{
//	if (!pTable)
//		return;
//
//	const char *Types[7] = { "int", "float", "Vector", "Vector2D", "const char *", "Array", "void *" };
//
//	if (nDepth == 0)
//		File << "class " << szClassName << "\n{\npublic:\n";
//
//	for (int n = 0; n < pTable->m_nProps; n++)
//	{
//		RecvProp *pProp = pTable->GetProp(n);
//
//		if (!pProp)
//			continue;
//
//		std::string_view sVarName(pProp->m_pVarName);
//
//		if (!sVarName.find("baseclass") || !sVarName.find("0") || !sVarName.find("1") || !sVarName.find("2"))
//			continue;
//
//		const char *szType = Types[pProp->GetType()];
//
//		if (sVarName.find("m_b") == 0 && pProp->GetType() == 0)
//			szType = "bool";
//
//		if (sVarName.find("m_vec") == 0)
//			szType = "Vector";
//
//		if (sVarName.find("m_h") == 0)
//			szType = "EHANDLE";
//
//		if (pProp->GetOffset())
//			File << "\tNETVAR(" << sVarName << ", " << szType << ", \"" << szClassName << "\", \"" << sVarName << "\");\n";
//
//		if (auto DataTable = pProp->GetDataTable())
//			DumpTable(DataTable, nDepth + 1);
//	}
//
//	if (nDepth == 0)
//		File << "};\n";
//}
//
//void DumpTables()
//{
//	File.open("NETVAR_DUMP_NEW.h");
//
//	for (ClientClass *pClass = I::BaseClientDLL->GetAllClasses(); pClass; pClass = pClass->m_pNext) {
//		szClassName = pClass->m_pNetworkName;
//		DumpTable(pClass->m_pRecvTable, 0);
//	}
//
//	File.close();
//}

void CGlobal_Entry::Load()
{
	while (!GetModuleHandleW(L"mss32.dll"))
		Sleep(100);

	U::Offsets.Initialize();

	//Interfaces
	{
		I::BaseClientDLL = U::Interface.Get<IBaseClientDLL*>("client.dll", "VClient017");
		I::ClientEntityList = U::Interface.Get<IClientEntityList*>("client.dll", "VClientEntityList003");
		I::GameMovement = U::Interface.Get<IGameMovement*>("client.dll", "GameMovement001");
		I::ClientPrediction = U::Interface.Get<CPrediction*>("client.dll", "VClientPrediction001");

		I::EngineClient = U::Interface.Get<IVEngineClient*>("engine.dll", "VEngineClient014");
		I::EngineVGui = U::Interface.Get<IEngineVGui*>("engine.dll", "VEngineVGui001");
		I::InputSystem = U::Interface.Get<IInputSystem*>("inputsystem.dll", "InputSystemVersion001");
		I::EngineTrace = U::Interface.Get<IEngineTrace*>("engine.dll", "EngineTraceClient003");
		I::RenderView = U::Interface.Get<IVRenderView*>("engine.dll", "VEngineRenderView014");
		I::ModelRender = U::Interface.Get<IVModelRender*>("engine.dll", "VEngineModel016");
		I::GameEventManager = U::Interface.Get<IGameEventManager2*>("engine.dll", "GAMEEVENTSMANAGER002");

		I::StudioRender = U::Interface.Get<IStudioRender*>("studiorender.dll", "VStudioRender025");

		I::MatSystemSurface = U::Interface.Get<IMatSystemSurface*>("vguimatsurface.dll", "VGUI_Surface030");

		I::Cvar = U::Interface.Get<ICvar*>("vstdlib.dll", "VEngineCvar004");

		//Other shenanigans
		{
			I::GlobalVars = *reinterpret_cast<CGlobalVarsBase**>(U::Offsets.GlobalVars + 0x8);
			XASSERT(I::GlobalVars == nullptr);

			I::ClientState = *reinterpret_cast<CClientState**>(U::Offsets.C_ClientState + 0x1);
			XASSERT(I::ClientState == nullptr);

			I::PredictionPlayer = *reinterpret_cast<C_BasePlayer***>(U::Offsets.PredictionPlayer + 0x2);
			XASSERT(I::PredictionPlayer == nullptr);
		}
	}

	//DumpTables();
	H::Draw.Initialize();

	if (I::EngineClient->IsInGame())
		g_Globals.m_nLocalIndex = I::EngineClient->GetLocalPlayer();

	G::Hooks.Initialize();
	WINAPI_HOOK::WINAPI_WndProc::Init();

	Config::Load((std::filesystem::current_path().string() + "\\Arsenal\\default.json").c_str());
	F::Notifications.Add("Config default loaded!");

	I::MatSystemSurface->PlaySound("bot\\owned.wav");
	I::Cvar->ConsoleColorPrintf({ 15, 150, 15, 255 }, "[Arsenal] Loaded!\n");
}

void CGlobal_Entry::Unload()
{
	G::Hooks.Uninitialize();
	SetWindowLongPtr(WINAPI_HOOK::WINAPI_WndProc::hwWindow, GWL_WNDPROC, (LONG_PTR)WINAPI_HOOK::WINAPI_WndProc::Original);

	Sleep(100);

	H::Draw.Uninitialize();

	Sleep(100);

	I::MatSystemSurface->PlaySound("bot\\that_was_it.wav");
	I::Cvar->ConsoleColorPrintf({ 220, 20, 60, 255 }, "[Arsenal] Unloaded!\n");
}
