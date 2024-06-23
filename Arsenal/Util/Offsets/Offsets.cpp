#include "Offsets.h"

#define GETPATT(x, mod, patt) x = U::Pattern.Find(mod, patt); XASSERT(x == 0x0)

static constexpr auto CLIENT_DLL = "client.dll";
static constexpr auto ENGINE_DLL = "engine.dll";
static constexpr auto SERVER_DLL = "server.dll";
static constexpr auto VSTDLIB_DLL = "vstdlib.dll";
static constexpr auto VGUI2_DLL = "vgui2.dll";
static constexpr auto MATSURFACE_DLL = "vguimatsurface.dll";
static constexpr auto MATSYSTEM_DLL = "MaterialSystem.dll";
static constexpr auto STUDIORENDER_DLL = "studiorender.dll";
static constexpr auto TIER0_DLL = "tier0.dll";
static constexpr auto VPHYSICS_DLL = "vphysics.dll";

void CUtil_Offsets::Initialize()
{
	GETPATT(ClientModeShared_CreateMove, CLIENT_DLL, "55 8B EC E8 ? ? ? ? 8B C8 85 C9 75 06");
	GETPATT(C_BasePlayer_CalcPlayerView, CLIENT_DLL, "55 8B EC 83 EC 18 53 56 8B F1 8B");
	GETPATT(C_BaseEntity_SetPredictionRandomSeed, CLIENT_DLL, "55 8B EC 8B 45 08 85 C0 75 0C");
	GETPATT(C_BasePlayer_UsingStandardWeaponsInVehicle, CLIENT_DLL, "56 57 8B F9 8B B7 ? ? ? ? 83 FE FF 74 4D");
	GETPATT(C_BasePlayer_UpdateButtonState, CLIENT_DLL, "55 8B EC 8B 81 ? ? ? ? 8B D0");
	GETPATT(PredictionPlayer, CLIENT_DLL, "89 3D ? ? ? ? F3 0F");
	GETPATT(ClientTable, CLIENT_DLL, "8B 0D ? ? ? ? 8B 01 5D FF 60 28 CC");

	GETPATT(C_ClientState, ENGINE_DLL, "B9 ? ? ? ? E8 ? ? ? ? 83 3D ? ? ? ? ? 75 4A");
	GETPATT(GlobalVars, ENGINE_DLL, "A1 ? ? ? ? 8B 11 68");

	GETPATT(StartDrawing, MATSURFACE_DLL, "55 8B EC 64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 64 89 25 ? ? ? ? 83 EC 14");
	GETPATT(FinishDrawing, MATSURFACE_DLL, "55 8B EC 6A FF 68 ? ? ? ? 64 A1 ? ? ? ? 50 64 89 25 ? ? ? ? 51 56 6A 00");
}