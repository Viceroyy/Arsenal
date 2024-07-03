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

#pragma region Misc

	CFGVAR(Misc_Bunnyhop, true);
	CFGVAR(Misc_AutoStrafe, true);
	CFGVAR(Misc_NoSpread, true);
	CFGVAR(Misc_NoRecoil, true);

#pragma endregion

#pragma region Visual

	CFGVAR(Visual_NoRecoil, true);
	CFGVAR(Visual_ClearScreenshots, true);
	CFGVAR(Visual_PlayerArrows_Enabled, true);
	CFGVAR(Visual_PlayerArrows_Offset, 25);
	CFGVAR(Visual_PlayerArrows_MaxDist, 1000.f);
	CFGVAR(Visual_DrawCrosshair, false);
	CFGVAR(Visual_DrawSpread, false);
	CFGVAR(Visual_FOV, 110);
	CFGVAR(Visual_ViewmodelFOV, 90);
	CFGVAR(Visual_SpectatorList, false);

#pragma endregion

#pragma region Colors

	CFGVAR(Colors_Relative, true);
	CFGVAR(Colors_Enemy, Color_t({ 250, 170, 10, 255 }));
	CFGVAR(Colors_Team, Color_t({ 252, 92, 101, 255 }));
	CFGVAR(Colors_TeamT, Color_t({ 240, 90, 90, 255 }));
	CFGVAR(Colors_TeamCT, Color_t({ 150, 200, 255, 255 }));
	CFGVAR(Colors_PlantedC4, Color_t({ 255, 219, 88, 255 }));

#pragma endregion
}