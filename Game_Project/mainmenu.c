#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "settings.h"
#include "sounds.h"
#include "plate.h"
#include "bubbles.h"
#include "credits.h"

#define OFFSET		100
#define MOVE_DOWN   200

CP_Font myFont;
CP_Font titleFont;
CP_Font subFont;
CP_Color ButtonBlue;
CP_Color White;
CP_Color plate_outer;
CP_Color plate_inner;
float mx, my;
float sponge_arc = 20.0f;
int dir = 1;


void Main_Menu_Init(void)
{
	myFont = CP_Font_Load("Assets/Exo2-Regular.ttf");
	titleFont = CP_Font_Load("Assets/SuperWater.ttf");
	subFont = CP_Font_Load("Assets/MontserratBlackItalic.otf");
	White = CP_Color_Create(255, 255, 255, 255);
	ButtonBlue = CP_Color_Create(123, 183, 220 , 255);
	plate_outer = CP_Color_Create(230, 230, 230, 255);
	plate_inner = CP_Color_Create(210, 210, 210, 255);
	init_background_music();
	update_volumes();
	Bubbles_Init();
	change_plate();
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

	// Interactive aspect of ButtonBlue
	if (IsAreaClicked(center_x, button_y - OFFSET + MOVE_DOWN, 300, 150, mx, my)) {
		play_pop = 10;
		Bubbles_Manual(mx, my);
		if (CP_Input_MouseClicked()) {
			CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
		}
	}
	else if (IsAreaClicked(center_x, button_y + OFFSET + MOVE_DOWN, 300, 150, mx, my)) {
		exit_pop = 10;
		Bubbles_Manual(mx, my);
		if (CP_Input_MouseClicked()) {
			CP_Engine_Terminate();
		}
	}
	else if (IsAreaClicked(120.0, CP_System_GetWindowHeight() - 120.0f, 100.0f, 100.0f, mx, my)) {
		settings_pop = 10;
		Bubbles_Manual(mx, my);
		if (CP_Input_MouseClicked()) {
			CP_Engine_SetNextGameState(Settings_Init, Settings_Update, Settings_Exit);
		}
	}

	Bubbles_Manual(1520.0f, 300.0f);
	
	// UI decor - randomised plates, utilising change_plate() function in plate.c
	CP_Settings_Fill(plate_outer);
	CP_Graphics_DrawCircle((float)CP_System_GetWindowWidth() / 2, 1050.0f, 1200.0f);
	CP_Settings_Fill(plate_inner);
	CP_Graphics_DrawCircle((float)CP_System_GetWindowWidth() / 2, 1050.f, 900.0f);

	// Draw rectangles for ButtonBlue
	CP_Settings_Fill(ButtonBlue);
	CP_Settings_NoStroke();
	CP_Graphics_DrawRectAdvanced(center_x, button_y - OFFSET + MOVE_DOWN, 300.0f + play_pop, 150.0f + play_pop, 0.0f, 50.0f);
	CP_Graphics_DrawRectAdvanced(center_x, button_y + OFFSET + MOVE_DOWN, 300.0f + exit_pop, 150.0f + exit_pop, 0.0f, 50.0f);

	// Draw settings button
	CP_Graphics_DrawRectAdvanced(120, CP_System_GetWindowHeight() - 120.0f, 100.0f + settings_pop, 100.0f + settings_pop, 0.0f, 20.0f);
	CP_Settings_Fill(White);
	CP_Graphics_DrawCircle(120, CP_System_GetWindowHeight() - 120.0f, 50.0f);

	// Draw text for ButtonBlue
	CP_Font_Set(myFont);
	CP_Settings_Fill(White);
	CP_Settings_TextSize(70.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("Play", center_x, button_y - OFFSET + MOVE_DOWN);
	CP_Font_DrawText("Exit", center_x, button_y + OFFSET + MOVE_DOWN);

	// UI decor - text shadow
	CP_Font_Set(titleFont);
	CP_Settings_Fill(White);
	CP_Settings_TextSize(250.0f);
	CP_Font_DrawText("WEWASHPL8", center_x + 15, button_y - 215);

	// Draw game logo
	CP_Font_Set(titleFont);
	CP_Settings_Fill(ButtonBlue);
	CP_Settings_TextSize(250.0f);
	CP_Font_DrawText("WEWASHPL8", center_x, button_y - 200);

	// Draw Subtext below Game Title
	CP_Font_Set(subFont);
	CP_Settings_Fill(ButtonBlue);
	CP_Settings_TextSize(70.0f);
	CP_Font_DrawText("Be the best dish-washer in town!", center_x, button_y - 70);

	// UI decor - wobble sponge :D
	if (dir == 1) {
		sponge_arc += 30.0f * CP_System_GetDt();
	} else if (dir == 0) {
		sponge_arc -= 30.0f * CP_System_GetDt();
	}

	if (sponge_arc >= 90.0f) {
		dir = 0;
	} else if (sponge_arc <= 20.0f) {
		dir = 1;
	}
	Bubbles_Draw();
	CP_Settings_NoStroke();
	CP_Settings_Fill(CP_Color_Create(255, 255, 0, 255));
	CP_Graphics_DrawRectAdvanced(1520.0f, 300.0f, 70.0f, 50.0f, sponge_arc, 0.0f);
	
	//go to credits (temporary)
	if (CP_Input_KeyTriggered(KEY_C)) {
		CP_Engine_SetNextGameState(credits_Init, credits_Update, credits_Exit);
	}
}

void Main_Menu_Exit(void)
{
	CP_Font_Free(myFont);
	CP_Font_Free(titleFont);
	CP_Font_Free(subFont);
}
