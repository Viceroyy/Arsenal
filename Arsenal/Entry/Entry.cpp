#include "Entry.h"
#include "../Hooks/WINAPI_WndProc.h"
#include "../Features/Notification/Notification.h"
#include "../Features/Players/Players.h"
#include "../Features/Materials/Materials.h"
#include "../Features/WorldModulation/WorldModulation.h"
#include "../Features/Commands/Commands.h"
#include "../Util/ConVars/ConVars.h"
#include <iostream>

MAKE_SIGNATURE(GlobalVars, "engine.dll", "48 8B 0D ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B 15", 0x0);
//MAKE_SIGNATURE(C_ClientState, "engine.dll", "48 8D 0D ? ? ? ? E8 ? ? ? ? F3 0F 5E 05", 0x0);

//// Constants for property types.
//constexpr std::array<std::string_view, 7> PROPERTY_TYPES = {
//	"int", "float", "Vector", "Vector2D", "const char *", "Array", "void *"
//};
//
//std::ofstream outputFile;
//std::string_view currentClassName;
//
//// Function to determine the property type based on the variable name.
//auto getPropertyType(std::string_view varName, int propType) -> std::string_view {
//	if (varName.starts_with("m_b") && propType == 0) return "bool";
//	if (varName.starts_with("m_vec")) return "Vector";
//	if (varName.starts_with("m_h")) return "EHANDLE";
//	return PROPERTY_TYPES[propType];
//}
//
//// Recursive function to dump a table's properties.
//void dumpTable(RecvTable* table, int depth) {
//	if (!table) return;
//
//	if (depth == 0) outputFile << "class " << currentClassName << "\n{\npublic:\n";
//
//	for (int i = 0; i < table->m_nProps; ++i) {
//		if (auto* prop = table->GetProp(i); prop) {
//			std::string_view varName(prop->m_pVarName);
//
//			// Skip properties that are base classes or follow certain patterns.
//			if (varName.starts_with("baseclass") || varName.find_first_of("012") != std::string_view::npos) continue;
//
//			auto propType = getPropertyType(varName, prop->GetType());
//			if (prop->GetOffset()) {
//				outputFile << "\tNETVAR(" << varName << ", " << propType
//					<< ", \"" << currentClassName << "\", \"" << varName << "\");\n";
//			}
//
//			// Recursively dump nested data tables.
//			if (auto dataTable = prop->GetDataTable()) {
//				dumpTable(dataTable, depth + 1);
//			}
//		}
//	}
//
//	if (depth == 0) outputFile << "};\n";
//}
//
//// Function to handle dumping all tables.
//void dumpAllTables() {
//	const auto filePath = std::filesystem::path("NETVAR_DUMP_NEW.h");
//
//	if (outputFile.open(filePath); !outputFile.is_open()) {
//		std::cerr << "Failed to open the file: " << filePath << '\n';
//		return;
//	}
//
//	for (auto* clientClass = I::BaseClientDLL->GetAllClasses(); clientClass; clientClass = clientClass->m_pNext) {
//		currentClassName = clientClass->m_pNetworkName;
//		dumpTable(clientClass->m_pRecvTable, 0);
//	}
//
//	outputFile.close();
//}

void CGlobal_Entry::Load()
{
	if (!U::Signatures.Initialize())
		return;

	//Interfaces
	{
		I::BaseClientDLL = U::Interface.Get<IBaseClientDLL*>("client.dll", "VClient017");
		//I::Input = **reinterpret_cast<CInput***>((*reinterpret_cast<uintptr_t**>(I::BaseClientDLL))[15] + 0x2);
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
		//I::ClientState = *reinterpret_cast<CClientState**>(U::Pattern.Find("engine.dll", "48 8D 0D ? ? ? ? E8 ? ? ? ? F3 0F 5E 05"));
		//I::ClientMode = **(ClientModeShared***)(U::Pattern.Find("client.dll", "8B 0D ? ? ? ? 8B 01 5D FF 60 28 CC") + 0x2);
		//I::HudChat = (CHudChat*)(((uintptr_t*)(I::ClientMode))[4]);

		//Other shenanigans
		{
			const DWORD_PTR dwpFunc = U::VFunc.Get<DWORD_PTR>(I::BaseClientDLL, 10u);
			XASSERT(dwpFunc == NULL);

			I::ClientMode = *reinterpret_cast<ClientModeShared**>(*reinterpret_cast<DWORD*>(dwpFunc + 0x03) + dwpFunc + 0x07);
			I::GlobalVars = reinterpret_cast<CGlobalVarsBase*>(*reinterpret_cast<DWORD*>(S::GlobalVars() + 0xA) + S::GlobalVars() + 0xE);

			XASSERT(I::ClientMode == nullptr);
			XASSERT(I::GlobalVars == nullptr);

			//I::ClientState = *reinterpret_cast<CClientState**>(S::C_ClientState());
			//XASSERT(I::ClientState == nullptr);
		}
	}

	U::ConVars.Initialize();
	//dumpAllTables();
	H::Draw.Initialize();

	U::Hooks.Initialize();
	F::Commands.Initialize();
	F::Players.Parse();

	Config::Load((std::filesystem::current_path().string() + "\\Arsenal\\default.json").c_str());
	F::Notifications.Add("Config default loaded!");

	I::MatSystemSurface->PlaySound("hl1/fvox/activated.wav");
	I::Cvar->ConsoleColorPrintf({ 175, 150, 255, 255 }, "[Arsenal] Loaded!\n");
}

void CGlobal_Entry::Unload()
{
	if (!bUnload)
	{
		G.Unload = true;

		CFG::Visuals_FOV = 90;
		CFG::Visuals_ViewmodelFOV = 70;

		U::Hooks.Unload();
		U::ConVars.Unload();

		Sleep(250);

		F::Materials.CleanUp();
		F::WorldModulation.RestoreWorldModulation();
		/*if (I::Input->CAM_IsThirdPerson())
		{
			auto pLocal = H::Entities.GetLocal();
			if (pLocal)
			{
				I::Input->CAM_ToFirstPerson();
				pLocal->ThirdPersonSwitch();
			}
		}*/
		H::Draw.Uninitialize();

		Sleep(250);
	}

	I::MatSystemSurface->PlaySound("hl1/fvox/deactivated.wav");
	I::Cvar->ConsoleColorPrintf({ 175, 150, 255, 255 }, "[Arsenal] Unloaded!\n");
}
