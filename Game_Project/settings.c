#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "settings.h"
#include "mainmenu.h"

int volume_level = 0;
CP_Color button_text;
float mousex = 0, mousey = 0;

void Settings_Init(void)
{
	button_text = CP_Color_Create(255, 255, 255, 255);

}

void Settings_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(233, 239, 255, 255));

	volume_level = setting_bar(200);

	// Draw settings button
	CP_Graphics_DrawRectAdvanced(120, CP_System_GetWindowHeight() - 120, 100, 100, 0, 20);
	CP_Settings_Fill(button_text);
	CP_Graphics_DrawCircle(120, CP_System_GetWindowHeight() - 120, 50);
	if (CP_Input_MouseClicked()) {
		mousex = CP_Input_GetMouseX();
		mousey = CP_Input_GetMouseY();
		if (IsAreaClicked(120, CP_System_GetWindowHeight() - 120, 100, 100, mousex, mousey)) {
			CP_Engine_SetNextGameState(Main_Menu_Exit, Main_Menu_Update, Main_Menu_Exit);
		}
	}
}

void Settings_Exit(void)
{
	
}