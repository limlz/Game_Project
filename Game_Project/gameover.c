#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "mainmenu.h"
#include "money.h"

#define OFFSET		150

#include <stdio.h>

CP_Font myFont;
CP_Color buttons;
CP_Color button_text;
float mx, my;
char ScoreText[50];

void Game_Over_Init(void)
{
	myFont = CP_Font_Load("Assets/Exo2-Regular.ttf");
	button_text = CP_Color_Create(255, 255, 255, 255);
	buttons = CP_Color_Create(123, 183, 220, 255);
}

void Game_Over_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(233, 239, 255, 255));

	float center_x = CP_System_GetWindowWidth() * 0.5f;
	float button_y = CP_System_GetWindowHeight() * 0.5f;
	mx = CP_Input_GetMouseX();
	my = CP_Input_GetMouseY();
	int try_pop = 0;
	int menu_pop = 0;
	if (IsAreaClicked(center_x, button_y - OFFSET, 300, 200, mx, my)) {
		try_pop = 10;
		if (CP_Input_MouseClicked()) {
			CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
		}
	}
	else if (IsAreaClicked(center_x, button_y + OFFSET, 500, 200, mx, my)) {
		menu_pop = 10;
		if (CP_Input_MouseClicked()) {
			CP_Engine_SetNextGameState(Main_Menu_Exit, Main_Menu_Update, Main_Menu_Exit);
		}

	}

	// Draw rectangles for buttons
	CP_Settings_Fill(buttons);
	CP_Settings_NoStroke();
	CP_Graphics_DrawRectAdvanced(center_x, button_y - OFFSET, 300 + try_pop, 200 + try_pop, 0, 50);
	CP_Graphics_DrawRectAdvanced(center_x, button_y + OFFSET, 500 + menu_pop, 200 + menu_pop, 0, 50);
	CP_Settings_Stroke(CP_Color_Create(255, 255, 255, 255));

	// Draw text for buttons
	CP_Font_Set(myFont);
	CP_Settings_Fill(button_text);
	CP_Settings_TextSize(70.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("Try Again?", center_x, button_y - OFFSET);
	CP_Font_DrawText("Exit to Main Menu", center_x, button_y + OFFSET);

	// Draw score
	CP_Settings_Fill(buttons);
	sprintf_s(ScoreText, sizeof(ScoreText), "Final Score : %d", get_total_earned());
	CP_Font_DrawText(ScoreText, 800, 100);

	// Check for which buttons are clicked
}

void Game_Over_Exit(void)
{
	void CP_Font_Free(CP_Font myFont);
}