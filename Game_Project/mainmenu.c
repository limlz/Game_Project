#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "settings.h"
#include "sounds.h"

#define OFFSET		100
#define MOVE_DOWN   200

CP_Font myFont;
CP_Color buttons;
CP_Color button_text;
float mx, my;

void Main_Menu_Init(void)
{
	myFont = CP_Font_Load("Assets/Exo2-Regular.ttf");
	button_text = CP_Color_Create(255, 255, 255, 255);
	buttons = CP_Color_Create(123, 183, 220 , 255);
	init_background_music();
	update_volumes();
}

void Main_Menu_Update(void)
{

	int play_pop = 0, exit_pop = 0, settings_pop = 0;
	// Get mouse x y
	mx = CP_Input_GetMouseX();
	my = CP_Input_GetMouseY();

	CP_Graphics_ClearBackground(CP_Color_Create(233, 239, 255, 255));

	float center_x = CP_System_GetWindowWidth() * 0.5f;
	float button_y = CP_System_GetWindowHeight() * 0.5f;

	// Interactive aspect of buttons
	if (IsAreaClicked(center_x, button_y - OFFSET + MOVE_DOWN, 300, 150, mx, my)) {
		play_pop = 10;
	}
	else if (IsAreaClicked(center_x, button_y + OFFSET + MOVE_DOWN, 300, 150, mx, my)) {
		exit_pop = 10;
	}
	else if (IsAreaClicked(120, CP_System_GetWindowHeight() - 120, 100, 100, mx, my)) {
		settings_pop = 10;
	}

	// Draw rectangles for buttons
	CP_Settings_Fill(buttons);
	CP_Settings_NoStroke();
	CP_Graphics_DrawRectAdvanced(center_x, button_y - OFFSET + MOVE_DOWN, 300 + play_pop, 150 + play_pop, 0, 50);
	CP_Graphics_DrawRectAdvanced(center_x, button_y + OFFSET + MOVE_DOWN, 300 + exit_pop, 150 + exit_pop, 0, 50);
	CP_Settings_Stroke(CP_Color_Create(255, 255, 255, 255));

	// Draw settings button
	CP_Graphics_DrawRectAdvanced(120, CP_System_GetWindowHeight() - 120, 100 + settings_pop, 100 + settings_pop, 0, 20);
	CP_Settings_Fill(button_text);
	CP_Graphics_DrawCircle(120, CP_System_GetWindowHeight() - 120, 50);

	// Draw text for buttons
	CP_Font_Set(myFont);
	CP_Settings_Fill(button_text);
	CP_Settings_TextSize(70.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("Play", center_x, button_y - OFFSET + MOVE_DOWN);
	CP_Font_DrawText("Exit", center_x, button_y + OFFSET + MOVE_DOWN);

	// Draw game logo
	CP_Settings_Fill(buttons);
	CP_Settings_TextSize(300.0f);
	CP_Font_DrawText("WEWASHPL8", center_x, button_y - 200);

	// Check for which buttons are clicked
	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(center_x, button_y - OFFSET + MOVE_DOWN, 300, 150, mx, my)) {
			CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
		}
		else if (IsAreaClicked(center_x, button_y + OFFSET + MOVE_DOWN, 300, 150, mx, my)) {
			CP_Engine_Terminate();
		}
		else if (IsAreaClicked(120, CP_System_GetWindowHeight() - 120, 100, 100, mx, my)) {
			CP_Engine_SetNextGameState(Settings_Init, Settings_Update, Settings_Exit);
		}
	}	
}

void Main_Menu_Exit(void)
{
	void CP_Font_Free(CP_Font myFont);
}