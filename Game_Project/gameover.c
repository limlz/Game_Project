#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "mainmenu.h"
#include "money.h"

#include <stdio.h>

CP_Font myFont;
CP_Color buttons;
CP_Color button_text;
float mx, my;
char ScoreText[50];

void Game_Over_Init(void)
{
	myFont = CP_Font_Load("Assets/Exo2-Regular.ttf");
	button_text = CP_Color_Create(0, 0, 0, 255);
	buttons = CP_Color_Create(255, 102, 102, 255);
}

void Game_Over_Update(void)
{
	int offset = 200;
	CP_Graphics_ClearBackground(CP_Color_Create(96, 96, 96, 255));
	float center_x = CP_System_GetWindowWidth() * 0.5f;
	float button_y = CP_System_GetWindowHeight() * 0.5f;

	CP_Settings_Fill(buttons);
	CP_Settings_TextSize(70.0f);
	CP_Graphics_DrawRect(center_x, button_y + offset, 300, 200);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Graphics_DrawRect(center_x, button_y - offset, 300, 200);
	CP_Font_Set(myFont);
	CP_Settings_Fill(button_text);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("Retry?", center_x, button_y - offset);
	CP_Font_DrawText("Back to Menu", center_x, button_y + offset);



	// Have to find a way to link the final score to this score
	// Temp value i
	sprintf_s(ScoreText, sizeof(ScoreText), "Final Score : %d", get_total_earned());
	CP_Font_DrawText(ScoreText, 800, 100);




	if (CP_Input_MouseClicked()) {
		mx = CP_Input_GetMouseX();
		my = CP_Input_GetMouseY();
		if (IsAreaClicked(center_x, button_y - offset, 300, 200, mx, my)) {
			CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
		}
		else if (IsAreaClicked(center_x, button_y + offset, 300, 200, mx, my)) {
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}
}

void Game_Over_Exit(void)
{
	void CP_Font_Free(CP_Font myFont);
}