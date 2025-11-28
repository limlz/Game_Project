/*************************************************************************
@file		mainmenu.c
@Author		Nur Nadia Natasya Binte Mohd Taha (nurnadianatasya.b@digipen.edu)
@Co-authors	Lim Liang Zhou (l.liangzhou@digipen.edu)
			Chiu Jun Wen (j.chiu@digipen.edu)
@brief		This file contains the function definitions for the main menu,
			including button interactions and navigation to other game states.
			This file also includes bubble effects when buttons are clicked, and
			bubblegun functionality in the main menu.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "settings.h"
#include "sounds.h"
#include "plate.h"
#include "bubbles.h"
#include "bubblegun.h"
#include "gameover.h"
#include "leaderboard.h"	
#include "img_font_init.h"
#include "mainmenu.h"
#include "tutorial.h"
#include "credits.h"

#define OFFSET		275   // horizontal spacing from center for Play/Exit buttons
#define MOVE_DOWN   125   // vertical offset for button placement

float mx, my;                 // current mouse x/y each frame
float sponge_arc = 20.0f;     // unused right now (was for sponge wobble animation)
int dir = 1;                  // unused right now (wobble direction)
int rand_sub_text = 0;        // index for random subtext line
int show_controls = 0;        // 1 if controls overlay is visible

void Main_Menu_Init(void)
{
	// pick a random subtext line to show at start
	rand_sub_text = CP_Random_RangeInt(0, 9);

	// init bubblegun bullet pool
	BulletsInit();

	// load menu visuals + fonts + colors
	InitImagesFontsColors();

	// start background music + apply saved volume
	InitBackgrounMusic();
	UpdateVolume();

	// init bubble particle system
	BubblesInit();
}

/*------------------------------------------
Main Menu Update Function
Handles menu interactions, UI rendering,
button hover effects, animations, and transitions
------------------------------------------*/
void Main_Menu_Update(void)
{
	// pop values used to “grow” buttons/icons when hovered
	int play_pop = 0, exit_pop = 0, settings_pop = 0, controls_pop = 0, leaderboard_pop = 0, credits_pop = 0;

	// cache mouse coordinates for hover checks + effects
	mx = CP_Input_GetMouseX();
	my = CP_Input_GetMouseY();

	// clear screen and draw menu background image
	CP_Graphics_ClearBackground(CP_Color_Create(233, 239, 255, 255));
	CP_Image_Draw(background_image,
		(float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2,
		(float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);

	float center_x = CP_System_GetWindowWidth() * 0.5f;   // screen center x
	float button_y = CP_System_GetWindowHeight() * 0.5f;  // base y for main buttons

	/* ------------------ Hover + click logic for buttons/icons ------------------ */

	// Play button hover/click
	if (IsAreaClicked(center_x - OFFSET, button_y + MOVE_DOWN, 300, 150, mx, my)) {
		play_pop = 10;                 // hover pop effect
		BubblesManual(mx, my);         // spawn bubbles at cursor
		if (CP_Input_MouseClicked()) { // start game
			CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
		}
	}
	// Exit button hover/click
	else if (IsAreaClicked(center_x + OFFSET, button_y + MOVE_DOWN, 300, 150, mx, my)) {
		exit_pop = 10;
		BubblesManual(mx, my);
		if (CP_Input_MouseClicked()) {
			CP_Engine_Terminate();     // quit application
		}
	}
	// Settings icon hover/click (bottom-left)
	else if (IsAreaClicked(120.0, CP_System_GetWindowHeight() - 120.0f, 100.0f, 100.0f, mx, my)) {
		settings_pop = 10;
		BubblesManual(mx, my);

		// tooltip
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
	// Controls icon hover/click (bottom-right)
	else if (IsAreaClicked(CP_System_GetWindowWidth() - 120.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f, 100.0f, mx, my)) {
		controls_pop = 10;
		BubblesManual(mx, my);

		// tooltip
		CP_Settings_Fill(button_blue);
		CP_Graphics_DrawRectAdvanced(CP_System_GetWindowWidth() - 120.0f, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f, 120.0f, 75.0f, 0.0f, 20.0f);

		CP_Font_Set(montserrat_light);
		CP_Settings_Fill(white);
		CP_Settings_TextSize(30.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("Controls", CP_System_GetWindowWidth() - 120.0f, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f);

		// click shows controls overlay (if not already showing)
		if (CP_Input_MouseClicked() && !show_controls) {
			show_controls = 1;
		}
	}
	// Leaderboard icon hover/click (bottom-right, left of controls)
	else if (IsAreaClicked(CP_System_GetWindowWidth() - 240.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f, 100.0f, mx, my)) {
		leaderboard_pop = 10;
		BubblesManual(mx, my);

		// tooltip
		CP_Settings_Fill(button_blue);
		CP_Graphics_DrawRectAdvanced(CP_System_GetWindowWidth() - 240.0f, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f, 170.0f, 75.0f, 0.0f, 20.0f);

		CP_Font_Set(montserrat_light);
		CP_Settings_Fill(white);
		CP_Settings_TextSize(30.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("Leaderboard", CP_System_GetWindowWidth() - 240.0f, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f);

		if (CP_Input_MouseClicked()) {
			CP_Engine_SetNextGameState(Leaderboard_Init, Leaderboard_Update, Leaderboard_Exit);
		}
	}
	// Credits icon hover/click (bottom-left, right of settings)
	else if (IsAreaClicked(240.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f, 100.0f, mx, my)) {
		credits_pop = 10;
		BubblesManual(mx, my);

		// tooltip
		CP_Settings_Fill(button_blue);
		CP_Graphics_DrawRectAdvanced(240, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f, 120.0f, 75.0f, 0.0f, 20.0f);

		CP_Font_Set(montserrat_light);
		CP_Settings_Fill(white);
		CP_Settings_TextSize(30.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("Credits", 240.0f, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f);

		if (CP_Input_MouseClicked()) {
			CP_Engine_SetNextGameState(credits_Init, credits_Update, credits_Exit);
		}
	}

	/* ------------------ Background UI decoration (plates) ------------------ */

	// draw big plate circles behind UI
	CP_Settings_Fill(plate_outer);
	CP_Graphics_DrawCircle((float)CP_System_GetWindowWidth() / 2, 1050.0f, 1200.0f);
	CP_Settings_Fill(plate_inner);
	CP_Graphics_DrawCircle((float)CP_System_GetWindowWidth() / 2, 1050.f, 900.0f);

	/* ------------------ Draw main buttons and icons ------------------ */

	// main buttons
	CP_Settings_Fill(button_blue);
	CP_Settings_NoStroke();
	CP_Graphics_DrawRectAdvanced(center_x - OFFSET, button_y + MOVE_DOWN, 300.0f + play_pop, 150.0f + play_pop, 0.0f, 50.0f);
	CP_Graphics_DrawRectAdvanced(center_x + OFFSET, button_y + MOVE_DOWN, 300.0f + exit_pop, 150.0f + exit_pop, 0.0f, 50.0f);

	// settings icon
	CP_Image_Draw(settings_icon, 120.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f + settings_pop, 100.0f + settings_pop, 255);

	// controls icon
	CP_Image_Draw(controls_icon, CP_System_GetWindowWidth() - 120.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f + controls_pop, 100.0f + controls_pop, 255);

	// leaderboard icon
	CP_Image_Draw(leaderboard_icon, CP_System_GetWindowWidth() - 240.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f + leaderboard_pop, 100.0f + leaderboard_pop, 255);

	// credits icon (drawn manually)
	CP_Settings_Fill(button_blue);
	CP_Graphics_DrawRectAdvanced(240.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f + credits_pop, 100.0f + credits_pop, 0.0f, 20.0f);
	CP_Settings_Fill(white);
	CP_Graphics_DrawRectAdvanced(240.0f, CP_System_GetWindowHeight() - 120.0f, 45.0f, 60.0f, 0.0f, 5.0f);
	CP_Settings_Stroke(button_blue);
	CP_Graphics_DrawLine(240.0f - 17.0f, CP_System_GetWindowHeight() - 120.0f, 240.0f + 17.0f, CP_System_GetWindowHeight() - 120.0f);
	CP_Graphics_DrawLine(240.0f - 17.0f, CP_System_GetWindowHeight() - 120.0f + 15.0f, 240.0f + 17.0f, CP_System_GetWindowHeight() - 120.0f + 15.0f);
	CP_Graphics_DrawLine(240.0f - 17.0f, CP_System_GetWindowHeight() - 120.0f - 15.0f, 240.0f + 17.0f, CP_System_GetWindowHeight() - 120.0f - 15.0f);

	// button text
	CP_Font_Set(montserrat_light);
	CP_Settings_Fill(white);
	CP_Settings_TextSize(70.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("Play", center_x - OFFSET, button_y + MOVE_DOWN);
	CP_Font_DrawText("Exit", center_x + OFFSET, button_y + MOVE_DOWN);

	/* ------------------ Menu art (hamster, title, soap bottle) ------------------ */

	CP_Settings_NoStroke();
	CP_Settings_Fill(CP_Color_Create(141, 144, 147, 200));
	CP_Image_Draw(hamsta, 400.0f, 240.0f, 220.0f, 220.0f, 255);

	CP_Image_Draw(title_image, center_x - 15.0f, 275.0f, 1315.0f, 740.0f, 255);
	CP_Image_DrawAdvanced(soap_bottle, 1500.0f, 120.0f, 200.0f, 200.0f, 255, 340.0f);

	/* ------------------ Random subtext under title ------------------ */

	char subtext[10][64] = { "The Ultimate Dish-Washing Simulator",
							"Become a Pro at Cleaning Plates!",
							"Scrub Away Your Worries!",
							"Where Every Plate Tells a Story",
							"Get Ready to Rinse and Repeat!",
							"Making Dishes Sparkle, One Plate at a Time",
							"Join the Clean Plate Revolution!",
							"From Grimy to Gleaming!",
							"Wash, Rinse, Repeat - The Fun Never Ends!",
							"Turn Dirty Dishes into Shiny Smiles!" };

	// clicking anywhere changes the subtext line
	if (CP_Input_MouseClicked()) {
		rand_sub_text = CP_Random_RangeInt(0, 9);
	}

	// draw subtext shadow + main text
	CP_Font_Set(title_font);
	CP_Settings_Fill(grey);
	CP_Settings_TextSize(40.0f);
	CP_Font_DrawText(subtext[rand_sub_text], center_x, CP_System_GetWindowHeight() - 120.0f);

	CP_Settings_Fill(white);
	CP_Settings_TextSize(40.0f);
	CP_Font_DrawText(subtext[rand_sub_text], center_x + 5, CP_System_GetWindowHeight() - 120.0f - 5);

	/* ------------------ Bubblegun effects (bullets + bubbles) ------------------ */

	// update/draw bullets and bubble particles
	BulletsUpdateAndDraw();
	Bubbles_Draw();

	/* ------------------ Hamster arm aiming + shooting ------------------ */

	// compute angle from hamster hand origin to mouse position
	CP_Vector hand_origin = CP_Vector_Set(470.0f, 240.0f);
	CP_Vector hand_vector = CP_Vector_Subtract(CP_Vector_Set(mx, my), hand_origin);
	CP_Vector up = CP_Vector_Set(0, 1);
	float hand_angle = CP_Vector_AngleCW(up, CP_Vector_Negate(hand_vector));

	// fire bullets while mouse button is held down
	if (CP_Input_MouseDown(MOUSE_BUTTON_1)) {
		BulletsSpawn(hand_origin, CP_Vector_Set(mx, my), 400.0f, 15.0f);
	}

	// draw correct arm sprite depending on angle (flipped when aiming to the left)
	if (hand_angle > 0 && hand_angle < 180) {
		CP_Image_DrawAdvanced(arm, 470.0f, 240.0f, 120.0f, 120.0f, 255, hand_angle);
	}
	else {
		CP_Image_DrawAdvanced(arm_flipped, 470.0f, 240.0f, 120.0f, 120.0f, 255, hand_angle);
	}

	CP_Settings_NoStroke();
	CP_Settings_Fill(CP_Color_Create(255, 255, 0, 255));

	/* ------------------ Controls overlay toggle ------------------ */

	// clicking near center bottom hides controls overlay
	if (IsAreaClicked((float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2 + 240,
		250.0f, 100.0f, mx, my)) {
		if (CP_Input_MouseClicked() && show_controls) {
			show_controls = 0;
		}
	}

	// draw overlay if enabled
	if (show_controls) {
		ControlsOverlay();
	}
}

void Main_Menu_Exit(void)
{
	// free menu image/font resources
	FreeImagesFonts();
}
