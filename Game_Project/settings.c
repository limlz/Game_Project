#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "settings.h"
#include "mainmenu.h"

int volume_level = 0;
CP_Color button_text, buttons;
float mousex = 0, mousey = 0;

void Settings_Init(void)
{
	button_text = CP_Color_Create(255, 255, 255, 255);
	buttons = CP_Color_Create(123, 183, 220, 255);
}

void Settings_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(233, 239, 255, 255));

	// Draw settings logo
	CP_Settings_Fill(buttons);
	CP_Settings_TextSize(150.0f);
	CP_Font_DrawText("Settings", CP_System_GetWindowWidth()/2, 50);

	CP_Settings_TextSize(60.0f);
	CP_Font_DrawText("SFX Volume", CP_System_GetWindowWidth() / 2, 230);
	sfx_volume_setting_bar(300);

	CP_Settings_TextSize(60.0f);
	CP_Font_DrawText("Music Volume", CP_System_GetWindowWidth() / 2, 380);
	music_volume_setting_bar(450);

	// Draw settings button
	CP_Graphics_DrawRectAdvanced(120, CP_System_GetWindowHeight() - 120, 100, 100, 0, 20);
	CP_Settings_Fill(button_text);
	CP_Graphics_DrawCircle(120, CP_System_GetWindowHeight() - 120, 50);
	if (CP_Input_MouseClicked()) {
		mousex = CP_Input_GetMouseX();
		mousey = CP_Input_GetMouseY();
		if (IsAreaClicked(120, CP_System_GetWindowHeight() - 120, 100, 100, mousex, mousey)) {
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}
}

void Settings_Exit(void)
{
	
}