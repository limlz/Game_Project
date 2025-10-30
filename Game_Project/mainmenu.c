#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "settings.h"
#include "sounds.h"
#include "plate.h"
#include "bubbles.h"
#include "credits.h"
#include "bubblegun.h"

#define OFFSET		275
#define MOVE_DOWN   125

CP_Font montserrat_light;
CP_Font title_font;
CP_Font sub_font;
CP_Color button_blue;
CP_Color white;
CP_Color plate_outer;
CP_Color plate_inner;
CP_Image arm;
CP_Image arm_flipped;
CP_Image hamsta;
float mx, my;
float sponge_arc = 20.0f;
int dir = 1;


void Main_Menu_Init(void)
{
	BulletsInit();
	hamsta = CP_Image_Load("Assets/hambod.gif");
	arm = CP_Image_Load("Assets/hamgun.gif");
	arm_flipped = CP_Image_Load("Assets/hamgunflip.gif");
	montserrat_light = CP_Font_Load("Assets/MontserratLight.ttf");
	title_font = CP_Font_Load("Assets/SuperWater.ttf");
	sub_font = CP_Font_Load("Assets/MontserratBlackItalic.otf");
	white = CP_Color_Create(255, 255, 255, 255);
	button_blue = CP_Color_Create(123, 183, 220 , 255);
	plate_outer = CP_Color_Create(230, 230, 230, 255);
	plate_inner = CP_Color_Create(210, 210, 210, 255);
	InitBackgrounMusic();
	UpdateVolume();
	BubblesInit();
	ChangePlate();
}

void Main_Menu_Update(void)
{
	int play_pop = 0, exit_pop = 0, settings_pop = 0, credits_pop = 0, leaderboard_pop = 0;
	// Get mouse x y
	mx = CP_Input_GetMouseX();
	my = CP_Input_GetMouseY();

	CP_Graphics_ClearBackground(CP_Color_Create(233, 239, 255, 255));

	float center_x = CP_System_GetWindowWidth() * 0.5f;
	float button_y = CP_System_GetWindowHeight() * 0.5f;

	// Interactive aspect of ButtonBlue
	if (IsAreaClicked(center_x - OFFSET, button_y + MOVE_DOWN, 300, 150, mx, my)) {
		play_pop = 10;
		BubblesManual(mx, my);
		if (CP_Input_MouseClicked()) {
			CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
		}
	}
	else if (IsAreaClicked(center_x + OFFSET, button_y + MOVE_DOWN, 300, 150, mx, my)) {
		exit_pop = 10;
		BubblesManual(mx, my);
		if (CP_Input_MouseClicked()) {
			CP_Engine_Terminate();
		}
	}
	else if (IsAreaClicked(120.0, CP_System_GetWindowHeight() - 120.0f, 100.0f, 100.0f, mx, my)) {
		settings_pop = 10;
		BubblesManual(mx, my);

		CP_Settings_Fill(button_blue);
		CP_Graphics_DrawRectAdvanced(120, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f, 120.0f, 75.0f, 0.0f, 20.0f);

		CP_Font_Set(montserrat_light);
		CP_Settings_Fill(white);
		CP_Settings_TextSize(30.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("Settings", 120, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f);

		if (CP_Input_MouseClicked()) {
			CP_Engine_SetNextGameState(Settings_Init, Settings_Update, Settings_Exit);
		}
	}
	else if (IsAreaClicked(CP_System_GetWindowWidth() - 120.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f, 100.0f, mx, my)) {
		credits_pop = 10;
		BubblesManual(mx, my);

		CP_Settings_Fill(button_blue);
		CP_Graphics_DrawRectAdvanced(CP_System_GetWindowWidth() - 120.0f, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f, 120.0f, 75.0f, 0.0f, 20.0f);

		CP_Font_Set(montserrat_light);
		CP_Settings_Fill(white);
		CP_Settings_TextSize(30.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("Credits", CP_System_GetWindowWidth() - 120.0f, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f);

		if (CP_Input_MouseClicked()) {
			CP_Engine_SetNextGameState(credits_Init, credits_Update, credits_Exit);
		}
	}
	else if (IsAreaClicked(CP_System_GetWindowWidth() - 240.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f, 100.0f, mx, my)) {
		leaderboard_pop = 10;
		BubblesManual(mx, my);

		CP_Settings_Fill(button_blue);
		CP_Graphics_DrawRectAdvanced(CP_System_GetWindowWidth() - 240.0f, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f, 170.0f, 75.0f, 0.0f, 20.0f);

		CP_Font_Set(montserrat_light);
		CP_Settings_Fill(white);
		CP_Settings_TextSize(30.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("Leaderboard", CP_System_GetWindowWidth() - 240.0f, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f);

		if (CP_Input_MouseClicked()) {
			CP_Engine_SetNextGameState(credits_Init, credits_Update, credits_Exit);
		}
	}

	BubblesManual(1520.0f, 300.0f);
	
	// UI decor - randomised plates, utilising ChangePlate() function in plate.c
	CP_Settings_Fill(plate_outer);
	CP_Graphics_DrawCircle((float)CP_System_GetWindowWidth() / 2, 1050.0f, 1200.0f);
	CP_Settings_Fill(plate_inner);
	CP_Graphics_DrawCircle((float)CP_System_GetWindowWidth() / 2, 1050.f, 900.0f);

	// Draw rectangles for ButtonBlue
	CP_Settings_Fill(button_blue);
	CP_Settings_NoStroke();
	CP_Graphics_DrawRectAdvanced(center_x - OFFSET, button_y + MOVE_DOWN, 300.0f + play_pop, 150.0f + play_pop, 0.0f, 50.0f);
	CP_Graphics_DrawRectAdvanced(center_x + OFFSET, button_y + MOVE_DOWN, 300.0f + exit_pop, 150.0f + exit_pop, 0.0f, 50.0f);

	// Draw settings button
	CP_Settings_Fill(button_blue);
	CP_Graphics_DrawRectAdvanced(120, CP_System_GetWindowHeight() - 120.0f, 100.0f + settings_pop, 100.0f + settings_pop, 0.0f, 20.0f);
	CP_Settings_Fill(white);
	CP_Graphics_DrawCircle(120, CP_System_GetWindowHeight() - 120.0f, 50.0f);

	// Draw Credits button
	CP_Settings_Fill(button_blue);
	CP_Graphics_DrawRectAdvanced(CP_System_GetWindowWidth() - 120.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f + credits_pop, 100.0f + credits_pop, 0.0f, 20.0f);
	CP_Settings_Fill(white);
	CP_Graphics_DrawRectAdvanced(CP_System_GetWindowWidth() - 120.0f, CP_System_GetWindowHeight() - 120.0f, 45.0f , 60.0f , 0.0f, 5.0f);
	CP_Settings_Stroke(button_blue);
	CP_Graphics_DrawLine(CP_System_GetWindowWidth() - 120.0f - 17.0f, CP_System_GetWindowHeight() - 120.0f, CP_System_GetWindowWidth() - 120.0f + 17.0f, CP_System_GetWindowHeight() - 120.0f);
	CP_Graphics_DrawLine(CP_System_GetWindowWidth() - 120.0f - 17.0f, CP_System_GetWindowHeight() - 120.0f + 15.0f, CP_System_GetWindowWidth() - 120.0f + 17.0f, CP_System_GetWindowHeight() - 120.0f + 15.0f);
	CP_Graphics_DrawLine(CP_System_GetWindowWidth() - 120.0f - 17.0f, CP_System_GetWindowHeight() - 120.0f - 15.0f, CP_System_GetWindowWidth() - 120.0f + 17.0f, CP_System_GetWindowHeight() - 120.0f - 15.0f);
	// Draw leaderboard button
	CP_Settings_Fill(button_blue);
	CP_Graphics_DrawRectAdvanced(CP_System_GetWindowWidth() - 240.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f + leaderboard_pop, 100.0f + leaderboard_pop, 0.0f, 20.0f);
	CP_Settings_Fill(white);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() - 240.0f, CP_System_GetWindowHeight() - 120.0f, 20.0f, 60.0f);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() - 240.0f + 20.0f, CP_System_GetWindowHeight() - 120.0f + 15.0f, 20.0f, 30.0f);
	CP_Graphics_DrawRect(CP_System_GetWindowWidth() - 240.0f - 20.0f, CP_System_GetWindowHeight() - 120.0f + 10.0f, 20.0f, 40.0f);
	
	// Draw text for button_blue
	CP_Font_Set(montserrat_light);
	CP_Settings_Fill(white);
	CP_Settings_TextSize(70.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("Play", center_x - OFFSET, button_y + MOVE_DOWN);
	CP_Font_DrawText("Exit", center_x + OFFSET, button_y + MOVE_DOWN);

	// UI decor - text shadow
	CP_Font_Set(title_font);
	CP_Settings_Fill(white);
	CP_Settings_TextSize(250.0f);
	CP_Font_DrawText("WEWASHPL8", center_x + 15, button_y - 215);

	// Draw game logo
	CP_Font_Set(title_font);
	CP_Settings_Fill(button_blue);
	CP_Settings_TextSize(250.0f);
	CP_Font_DrawText("WEWASHPL8", center_x, button_y - 200);

	// Draw Subtext below Game Title
	CP_Font_Set(sub_font);
	CP_Settings_Fill(button_blue);
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

	// Hamster
	CP_Settings_NoStroke();
	CP_Settings_Fill(CP_Color_Create(141, 144, 147, 200));
	CP_Graphics_DrawEllipse(CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - 80.0f, 200.0f, 30.0f);
	CP_Image_Draw(hamsta, CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - 130.0f, 170.0f, 170.0f, 255);
	BulletsUpdateAndDraw();


	// Hamster Pointer
	CP_Vector hand_origin = CP_Vector_Set(CP_System_GetWindowWidth() / 2.0f + 60.0f, CP_System_GetWindowHeight() - 130.0f);
	CP_Vector hand_vector = CP_Vector_Subtract(CP_Vector_Set(mx, my), hand_origin);
	CP_Vector up = CP_Vector_Set(0, 1);
	float hand_angle = CP_Vector_AngleCW(up, CP_Vector_Negate(hand_vector));

	// Fire once per click
	if (CP_Input_MouseDown(MOUSE_BUTTON_1)) {
		BulletsSpawn(hand_origin, CP_Vector_Set(mx, my), 400.0f, 10.0f);
	}
	if (hand_angle > 0 && hand_angle < 180) {
		CP_Image_DrawAdvanced(arm, (float)(CP_System_GetWindowWidth() / 2 + 55.0f), (float)(CP_System_GetWindowHeight() - 130.0f), 80.0f, 80.0f, 255, hand_angle);
	}
	else {
		CP_Image_DrawAdvanced(arm_flipped, (float)(CP_System_GetWindowWidth() / 2 + 55.0f), (float)(CP_System_GetWindowHeight() - 130.0f), 60.0f, 80.0f, 255, hand_angle);
	}


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
	CP_Font_Free(montserrat_light);
	CP_Font_Free(title_font);
	CP_Font_Free(sub_font);
}

