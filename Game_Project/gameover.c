#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "mainmenu.h"
#include "money.h"

#define OFFSET		150

#include <stdio.h>

CP_Font montserrat_light;
CP_Font title_font;
CP_Color button_blue;
CP_Color white;
float mx, my;
char score_text[50];

void Game_Over_Init(void)
{
	title_font = CP_Font_Load("Assets/SuperWater.ttf");
	montserrat_light = CP_Font_Load("Assets/MontserratLight.ttf");
	white = CP_Color_Create(255, 255, 255, 255);
	button_blue = CP_Color_Create(123, 183, 220, 255);
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
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}

	}

	// Draw rectangles for button_blue
	CP_Settings_Fill(button_blue);
	CP_Settings_NoStroke();
	CP_Graphics_DrawRectAdvanced(center_x, button_y - OFFSET, 300.0f + try_pop, 200.0f + try_pop, 0, 50);
	CP_Graphics_DrawRectAdvanced(center_x, button_y + OFFSET, 500.0f + menu_pop, 200.0f + menu_pop, 0, 50);
	CP_Settings_Stroke(CP_Color_Create(255, 255, 255, 255));

	// Draw text for button_blue
	CP_Font_Set(montserrat_light);
	CP_Settings_Fill(white);
	CP_Settings_TextSize(60.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("Try Again?", center_x, button_y - OFFSET);
	CP_Font_DrawText("Exit to Main Menu", center_x, button_y + OFFSET);

	// Draw score
	CP_Font_Set(title_font);
	CP_Settings_TextSize(80.0f);
	CP_Settings_Fill(button_blue);
	sprintf_s(score_text, sizeof(score_text), "Final Score : %d", GetTotalEarned());
	CP_Font_DrawText(score_text, 800, 100);

	// Check for which button_blue are clicked
}

void Game_Over_Exit(void)
{
	CP_Font_Free(montserrat_light);
	CP_Font_Free(title_font);
	ResetMoney();
}