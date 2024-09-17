#pragma once
#include "../Util/Config/Config.h"

namespace CFG
{
#pragma region Menu

	CFGVAR_NOSAVE(Menu_Pos_X, 500);
	CFGVAR_NOSAVE(Menu_Pos_Y, 200);
	CFGVAR_NOSAVE(Menu_Width, 555);
	CFGVAR_NOSAVE(Menu_Height, 420);
	CFGVAR_NOSAVE(Menu_Drag_Bar_Height, 15);

	CFGVAR_NOSAVE(Menu_Spacing_X, 3);
	CFGVAR_NOSAVE(Menu_Spacing_Y, 3);

	CFGVAR_NOSAVE(Menu_Tab_Button_Width, 60);
	CFGVAR_NOSAVE(Menu_Tab_Button_Height, 18);

	CFGVAR_NOSAVE(Menu_CheckBox_Width, 10);
	CFGVAR_NOSAVE(Menu_CheckBox_Height, 10);

	CFGVAR_NOSAVE(Menu_Slider_Width, 100);
	CFGVAR_NOSAVE(Menu_Slider_Height, 6);

	CFGVAR_NOSAVE(Menu_InputKey_Width, 60);
	CFGVAR_NOSAVE(Menu_InputKey_Height, 14);

	CFGVAR_NOSAVE(Menu_InputText_Width, 150);
	CFGVAR_NOSAVE(Menu_InputText_Height, 30);

	CFGVAR_NOSAVE(Menu_Select_Width, 120);
	CFGVAR_NOSAVE(Menu_Select_Height, 14);

	CFGVAR_NOSAVE(Menu_ColorPicker_Preview_Width, 10);
	CFGVAR_NOSAVE(Menu_ColorPicker_Preview_Height, 10);

	CFGVAR(Menu_Text, Color_t({ 240, 240, 240, 255 }));
	CFGVAR(Menu_Text_Active, Color_t({ 220, 220, 220, 255 }));
	CFGVAR(Menu_Text_Inactive, Color_t({ 160, 160, 160, 255 }));
	CFGVAR(Menu_Text_Disabled, Color_t({ 100, 100, 100, 255 }));

	CFGVAR(Menu_Accent_Primary, Color_t({ 0, 100, 120, 255 }));
	CFGVAR(Menu_Accent_Secondary, Color_t({ 0, 100, 120, 255 }));
	CFGVAR(Menu_Background, Color_t({ 23, 23, 23, 250 }));

	CFGVAR(Menu_Snow, false);

#pragma endregion

#pragma region ESP

	CFGVAR(ESP_Enabled, true);
	CFGVAR(ESP_Alpha, 1.f);

	CFGVAR(ESP_Players_Enabled, true);
	CFGVAR(ESP_Players_Ignore_Local, false);
	CFGVAR(ESP_Players_Ignore_Enemies, false);
	CFGVAR(ESP_Players_Ignore_Teammates, false);
	CFGVAR(ESP_Players_Box, true);
	CFGVAR(ESP_Players_HealthText, true);
	CFGVAR(ESP_Players_HealthBar, true);
	CFGVAR(ESP_Players_ArmorText, true);
	CFGVAR(ESP_Players_ArmorBar, true);
	CFGVAR(ESP_Players_WeaponText, true);
	CFGVAR(ESP_Players_Name, true);
	CFGVAR(ESP_Players_Money, true);

	CFGVAR(ESP_World_Enabled, true);
	CFGVAR(ESP_World_Ignore_PlantedC4, false);
	CFGVAR(ESP_World_Ignore_DroppedWeapons, false);
	CFGVAR(ESP_World_Name, true);
	CFGVAR(ESP_World_Box, false);

#pragma endregion

#pragma region Materials

	CFGVAR(Materials_Active, true);

	CFGVAR(Materials_Players_Active, true);
	CFGVAR(Materials_Players_No_Depth, false);
	CFGVAR(Materials_Players_Alpha, 1.0f);
	CFGVAR(Materials_Players_Material, 0); //0 Original 1 Flat 2 Shaded 3 Glossy 4 Glow 5 Plastic
	CFGVAR(Materials_Players_Ignore_Local, false);
	CFGVAR(Materials_Players_Ignore_Friends, false);
	CFGVAR(Materials_Players_Ignore_Enemies, false);
	CFGVAR(Materials_Players_Ignore_Teammates, true);
	CFGVAR(Materials_Players_Ignore_LagRecords, false);
	CFGVAR(Materials_Players_LagRecords_Style, 1); //0 All 1 Last Only

	CFGVAR(Materials_World_Active, false);
	CFGVAR(Materials_World_No_Depth, false);
	CFGVAR(Materials_World_Alpha, 1.0f);
	CFGVAR(Materials_World_Material, 2); //0 Original 1 Flat 2 Shaded 3 Glossy 4 Glow 5 Plastic
	CFGVAR(Materials_World_Ignore_PlantedC4, false);
	CFGVAR(Materials_World_Ignore_DroppedWeapons, false);
	CFGVAR(Materials_World_Ignore_LocalProjectiles, false);
	CFGVAR(Materials_World_Ignore_EnemyProjectiles, false);
	CFGVAR(Materials_World_Ignore_TeammateProjectiles, false);

#pragma endregion

#pragma region Outlines

	CFGVAR(Outlines_Active, true);
	CFGVAR(Outlines_Style, 1); //0 Bloom 1 Crisp 2 Cartoony 3 Cartoony Alt
	CFGVAR(Outlines_Bloom_Amount, 5);

	CFGVAR(Outlines_Players_Active, true);
	CFGVAR(Outlines_Players_Alpha, 1.0f);
	CFGVAR(Outlines_Players_Ignore_Local, false);
	CFGVAR(Outlines_Players_Ignore_Friends, false);
	CFGVAR(Outlines_Players_Ignore_Enemies, false);
	CFGVAR(Outlines_Players_Ignore_Teammates, true);

	CFGVAR(Outlines_World_Active, true);
	CFGVAR(Outlines_World_Alpha, 1.0f);
	CFGVAR(Outlines_World_Ignore_PlantedC4, false);
	CFGVAR(Outlines_World_Ignore_DroppedWeapons, false);
	CFGVAR(Outlines_World_Ignore_LocalProjectiles, false);
	CFGVAR(Outlines_World_Ignore_EnemyProjectiles, false);
	CFGVAR(Outlines_World_Ignore_TeammateProjectiles, true);

#pragma endregion

#pragma region Misc

	CFGVAR(Misc_Bunnyhop, true);
	CFGVAR(Misc_AutoStrafe, true);
	CFGVAR(Misc_NoSpread, true);
	CFGVAR(Misc_NoRecoil, true);

	CFGVAR(Misc_Backtrack_PreferOnShot, false);
	CFGVAR(Misc_Backtrack_LatencyMode, 0); //0 Off 1 Optimized 2 Value
	CFGVAR(Misc_Backtrack_Latency, 0);

	CFGVAR(Misc_SequenceFreeze_Active, false);
	CFGVAR(Misc_SequenceFreeze_Key, 0);
	CFGVAR(Misc_SequenceFreeze_Value, 16);

#pragma endregion

#pragma region Visuals

	CFGVAR(Visuals_NoRecoil, true);
	CFGVAR(Visuals_NoInterpolation, true);
	CFGVAR(Visuals_NoMOTD, true);
	CFGVAR(Visuals_NoScreenEffects, true);
	CFGVAR(Visuals_NoDSP, true);
	CFGVAR(Visuals_NoPostProcessing, true);
	CFGVAR(Visuals_NoForcedAngles, true);
	CFGVAR(Visuals_NoRagdolls, true);
	CFGVAR(Visuals_NoConvarQueries, true);
	CFGVAR(Visuals_ClearScreenshots, true);
	CFGVAR(Visuals_PlayerArrows_Enabled, true);
	CFGVAR(Visuals_PlayerArrows_Offset, 25);
	CFGVAR(Visuals_PlayerArrows_MaxDist, 1000.f);
	CFGVAR(Visuals_DrawCrosshairOnSnipers, false);
	CFGVAR(Visuals_DrawSpread, false);
	CFGVAR(Visuals_FOV, 110);
	CFGVAR(Visuals_ViewmodelFOV, 90);
	CFGVAR(Visuals_LowGraphics, false);

	CFGVAR(Visuals_SpectatorList_Active, true);
	CFGVAR(Visuals_SpectatorList_Outline_Alpha, 1.0f);
	CFGVAR(Visuals_SpectatorList_Background_Alpha, 0.9f);
	CFGVAR(Visuals_SpectatorList_Pos_X, 400);
	CFGVAR(Visuals_SpectatorList_Pos_Y, 20);
	CFGVAR(Visuals_SpectatorList_Width, 200);

	CFGVAR(Visuals_Thirdperson_Active, false);
	CFGVAR(Visuals_Thirdperson_Key, 0);
	CFGVAR(Visuals_Thirdperson_Offset_Forward, 250.0f);
	CFGVAR(Visuals_Thirdperson_Offset_Right, 0.0f);
	CFGVAR(Visuals_Thirdperson_Offset_Up, 0.0f);
	CFGVAR(Visuals_Thirdperson_Crosshair, false);

	CFGVAR(Visuals_Remove_Fog, true);
	CFGVAR(Visuals_Remove_Sky_Fog, true);
	CFGVAR(Visuals_Night_Mode, 50.0f);
	CFGVAR(Visuals_World_Modulation_Mode, 0); //0 Night 1 Custom Colors
	CFGVAR(Visuals_World_Modulation_No_Sky_Change, false);
	CFGVAR(Visuals_Distance_Prop_Alpha, false);

	CFGVAR(Visuals_Viewmodel_OffsetX, 0);
	CFGVAR(Visuals_Viewmodel_OffsetY, 0);
	CFGVAR(Visuals_Viewmodel_OffsetZ, 0);
	CFGVAR(Visuals_Viewmodel_Roll, 0);

#pragma endregion

#pragma region Colors

	CFGVAR(Colors_Relative, true);
	CFGVAR(Colors_Enemy, Color_t({ 250, 170, 10, 255 }));
	CFGVAR(Colors_Team, Color_t({ 252, 92, 101, 255 }));
	CFGVAR(Colors_TeamT, Color_t({ 240, 90, 90, 255 }));
	CFGVAR(Colors_TeamCT, Color_t({ 150, 200, 255, 255 }));
	CFGVAR(Color_Friend, Color_t({ 38, 222, 129, 255 }));
	CFGVAR(Color_Cheater, Color_t({ 214, 48, 49, 255 }));
	CFGVAR(Color_RetardLegit, Color_t({ 48, 218, 70, 255 }));
	CFGVAR(Colors_PlantedC4, Color_t({ 255, 219, 88, 255 }));
	CFGVAR(Colors_DroppedWeapons, Color_t({ 255, 219, 88, 255 }));
	CFGVAR(Colors_World, Color_t({ 255, 255, 255, 255 }));
	CFGVAR(Colors_Sky, Color_t({ 255, 255, 255, 255 }));
	CFGVAR(Colors_Props, Color_t({ 255, 255, 255, 255 }));

#pragma endregion
}