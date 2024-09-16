#include "Entry.h"
#include "../Hooks/WINAPI_WndProc.h"
#include "../Features/Notification/Notification.h"
#include "../Features/Players/Players.h"
#include "../Features/Materials/Materials.h"
#include "../Features/Outlines/Outlines.h"
#include "../Features/WorldModulation/WorldModulation.h"
#include "../Features/Commands/Commands.h"
#include "../Util/ConVars/ConVars.h"
#include <iostream>

MAKE_SIGNATURE(GlobalVars, "engine.dll", "A1 ? ? ? ? 8B 11 68", 0x0);
MAKE_SIGNATURE(C_ClientState, "engine.dll", "B9 ? ? ? ? E8 ? ? ? ? 83 3D ? ? ? ? ? 75 4A", 0x0);
MAKE_SIGNATURE(PredictionPlayer, "client.dll", "89 3D ? ? ? ? F3 0F", 0x0);

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
	U::Signatures.Initialize();

	//Interfaces
	{
		I::BaseClientDLL = U::Interface.Get<IBaseClientDLL*>("client.dll", "VClient017");
		I::Input = **reinterpret_cast<CInput***>((*reinterpret_cast<uintptr_t**>(I::BaseClientDLL))[15] + 0x2);
		I::ClientEntityList = U::Interface.Get<IClientEntityList*>("client.dll", "VClientEntityList003");
		I::GameMovement = U::Interface.Get<IGameMovement*>("client.dll", "GameMovement001");
		I::ClientPrediction = U::Interface.Get<CPrediction*>("client.dll", "VClientPrediction001");
		I::EngineClient = U::Interface.Get<IVEngineClient*>("engine.dll", "VEngineClient014");
		I::ModelInfoClient = U::Interface.Get<IVModelInfoClient*>("engine.dll", "VModelInfoClient006");
		I::EngineVGui = U::Interface.Get<IEngineVGui*>("engine.dll", "VEngineVGui001");
		I::InputSystem = U::Interface.Get<IInputSystem*>("inputsystem.dll", "InputSystemVersion001");
		I::EngineTrace = U::Interface.Get<IEngineTrace*>("engine.dll", "EngineTraceClient003");
		I::RenderView = U::Interface.Get<IVRenderView*>("engine.dll", "VEngineRenderView014");
		I::ModelRender = U::Interface.Get<IVModelRender*>("engine.dll", "VEngineModel016");
		I::GameEventManager = U::Interface.Get<IGameEventManager2*>("engine.dll", "GAMEEVENTSMANAGER002");
		I::StudioRender = U::Interface.Get<IStudioRender*>("studiorender.dll", "VStudioRender025");
		I::MatSystemSurface = U::Interface.Get<IMatSystemSurface*>("vguimatsurface.dll", "VGUI_Surface030");
		I::MaterialSystem = U::Interface.Get<IMaterialSystem*>("MaterialSystem.dll", "VMaterialSystem080");
		I::Cvar = U::Interface.Get<ICvar*>("vstdlib.dll", "VEngineCvar004");
		I::ClientMode = **(ClientModeShared***)(U::Pattern.Find("client.dll", "8B 0D ? ? ? ? 8B 01 5D FF 60 28 CC") + 0x2);
		I::HudChat = (CHudChat*)(((uintptr_t*)(I::ClientMode))[4]);

		//Other shenanigans
		{
			I::GlobalVars = *reinterpret_cast<CGlobalVarsBase**>(S::GlobalVars() + 0x8);
			XASSERT(I::GlobalVars == nullptr);

			I::ClientState = *reinterpret_cast<CClientState**>(S::C_ClientState() + 0x1);
			XASSERT(I::ClientState == nullptr);

			I::PredictionPlayer = *reinterpret_cast<C_BasePlayer***>(S::PredictionPlayer() + 0x2);
			XASSERT(I::PredictionPlayer == nullptr);
		}
	}

	U::ConVars.Initialize();
	//dumpAllTables();
	H::Draw.Initialize();

	U::Hooks.Initialize();
	F::Commands.Initialize();
	PlayerManager::Parse();

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
		F::Outlines.CleanUp();
		F::WorldModulation.RestoreWorldModulation();
		if (I::Input->CAM_IsThirdPerson())
		{
			auto pLocal = H::EntityCache.GetLocal();
			if (pLocal)
			{
				I::Input->CAM_ToFirstPerson();
				pLocal->ThirdPersonSwitch();
			}
		}
		H::Draw.Uninitialize();

		Sleep(250);
	}

	I::MatSystemSurface->PlaySound("hl1/fvox/deactivated.wav");
	I::Cvar->ConsoleColorPrintf({ 175, 150, 255, 255 }, "[Arsenal] Unloaded!\n");
}
