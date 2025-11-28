/*************************************************************************
@file		gameover.c
@Author		Lim Liang Zhou (l.liangzhou@digipen.edu)
@Co-authors Chiu Jun Wen (j.chiu@digipen.edu)
@brief		This file contains the function definitions for the game over
			screen, including leaderboard management and player name input.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "mainmenu.h"
#include "money.h"
#include "img_font_init.h"
#include <stdio.h>
#include "leaderboard.h"
#include "credits.h"
#include "confetti.h"
#include "bubbles.h"
#include "roomba.h"
#include "faucet.h"

#define OFFSET		150                 // general spacing offset (not used in this file)
#define MAX_NAME_LENGTH 8             // max characters allowed for player name
#define MAX_LEADERBOARD_ENTRIES 5     // number of scores stored

typedef struct {
	char name[MAX_NAME_LENGTH + 1];   // player name string (with room for '\0')
	int score;                        // score value
} Entry;

static Entry leaderboard[MAX_LEADERBOARD_ENTRIES]; // leaderboard array (top scores)
static int entry_count = 0;                        // number of loaded entries
static int lowest_score;                           // lowest leaderboard score (threshold)
static int high_score;                             // highest leaderboard score (for display)
static float overlay_y = 0;                        // y-position of the highscore overlay panel
static int overlay_down = 1;                       // 1 if overlay is hidden/down, 0 if showing
int try_pop = 0;                                   // hover pop effect amount for Try Again
int menu_pop = 0;                                  // hover pop effect amount for Main Menu
int exit_pop = 0;                                  // hover pop effect amount for Exit
int leaderboard_pop = 0;                           // hover pop effect amount for leaderboard icon
int credits_pop = 0;                               // hover pop effect amount for credits icon

FILE* leaderboard_file;                            // file handle for loading/saving leaderboard

static float mx, my;                               // mouse position cached per frame
static char score_text[50];                        // buffer for score text
static char lowest_text[10];                       // buffer (currently not used)
static char highest_text[64];                      // buffer for highest score display

// variables for keyboard input
static char player_name[MAX_NAME_LENGTH + 1];      // typed name for new highscore
static int name_entered;                           // 1 if player confirmed name

void Game_Over_Init(void)
{
	// load images/fonts/colors used on this screen
	InitImagesFontsColors();

	// fetch leaderboard thresholds (from leaderboard module)
	lowest_score = GetLowestScore();
	high_score = GetHighestScore();

	// reset input state
	name_entered = 0;
	player_name[0] = '\0';

	// open leaderboard file for reading
	errno_t err = fopen_s(&leaderboard_file, "Assets/leaderboard.txt", "r");
	if (err != 0 || leaderboard_file == NULL) {
		return; // fail silently if file missing/unreadable
	}

	// read up to MAX_LEADERBOARD_ENTRIES entries from file
	while (entry_count < MAX_LEADERBOARD_ENTRIES &&
		fscanf_s(leaderboard_file, "%8s %d",
			leaderboard[entry_count].name, MAX_NAME_LENGTH + 1,
			&leaderboard[entry_count].score) == 2) {
		entry_count++;
	}
	fclose(leaderboard_file);

	// refresh lowest score after loading entries
	lowest_score = GetLowestScore();
	printf("Loaded %d entries from file\n", entry_count);

	// initialize visual effects
	InitConfetti();
	BubblesInit();
}

void Game_Over_Update(void)
{
	// clear screen and draw background image
	CP_Graphics_ClearBackground(blue_chalk);
	CP_Image_Draw(background_image,
		(float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2,
		(float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);

	// commonly used layout values
	float center_x = CP_System_GetWindowWidth() * 0.5f;
	float center_y = CP_System_GetWindowHeight() * 0.5f;
	float buttons_y = center_y + 130;

	// cache mouse positions
	mx = CP_Input_GetMouseX();
	my = CP_Input_GetMouseY();

	/* ------------------ Main Buttons (Try Again / Main Menu / Exit) ------------------ */

	// draw button rectangles (with pop hover scaling)
	CP_Settings_Fill(button_blue);
	CP_Settings_NoStroke();
	CP_Graphics_DrawRectAdvanced(center_x + 540, buttons_y, 400.0f + exit_pop, 200.0f + exit_pop, 0, 50);
	CP_Graphics_DrawRectAdvanced(center_x - 540, buttons_y, 400.0f + try_pop, 200.0f + try_pop, 0, 50);
	CP_Graphics_DrawRectAdvanced(center_x, buttons_y, 400.0f + menu_pop, 200.0f + menu_pop, 0, 50);

	// draw button labels
	CP_Font_Set(montserrat_light);
	CP_Settings_Fill(white);
	CP_Settings_TextSize(60.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("Try Again", center_x - 540, buttons_y);
	CP_Font_DrawText("Main Menu", center_x, buttons_y);
	CP_Font_DrawText("Exit", center_x + 540, buttons_y);

	/* ------------------ Bottom-right buttons (Leaderboard / Credits) ------------------ */

	// leaderboard icon
	CP_Image_Draw(leaderboard_icon,
		CP_System_GetWindowWidth() - 240.0f, CP_System_GetWindowHeight() - 120.0f,
		100.0f + leaderboard_pop, 100.0f + leaderboard_pop, 255);

	// credits icon (drawn manually using shapes)
	CP_Settings_Fill(button_blue);
	CP_Graphics_DrawRectAdvanced(CP_System_GetWindowWidth() - 120.0f, CP_System_GetWindowHeight() - 120.0f,
		100.0f + credits_pop, 100.0f + credits_pop, 0.0f, 20.0f);

	CP_Settings_Fill(white);
	CP_Graphics_DrawRectAdvanced(CP_System_GetWindowWidth() - 120.0f, CP_System_GetWindowHeight() - 120.0f,
		45.0f, 60.0f, 0.0f, 5.0f);

	CP_Settings_Stroke(button_blue);
	CP_Graphics_DrawLine(CP_System_GetWindowWidth() - 120.0f - 17.0f, CP_System_GetWindowHeight() - 120.0f,
		CP_System_GetWindowWidth() - 120.0f + 17.0f, CP_System_GetWindowHeight() - 120.0f);
	CP_Graphics_DrawLine(CP_System_GetWindowWidth() - 120.0f - 17.0f, CP_System_GetWindowHeight() - 120.0f + 15.0f,
		CP_System_GetWindowWidth() - 120.0f + 17.0f, CP_System_GetWindowHeight() - 120.0f + 15.0f);
	CP_Graphics_DrawLine(CP_System_GetWindowWidth() - 120.0f - 17.0f, CP_System_GetWindowHeight() - 120.0f - 15.0f,
		CP_System_GetWindowWidth() - 120.0f + 17.0f, CP_System_GetWindowHeight() - 120.0f - 15.0f);

	/* ------------------ Highest score display ------------------ */
	CP_Font_Set(sub_font);
	CP_Settings_Fill(button_blue);
	CP_Settings_TextSize(50.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);

	// show higher of saved high_score vs current run score
	if (high_score > GetTotalEarned()) {
		snprintf(highest_text, sizeof(highest_text), "Highest Score: %d", high_score);
	}
	else {
		snprintf(highest_text, sizeof(highest_text), "Highest Score: %d", GetTotalEarned());
	}
	CP_Font_DrawText(highest_text, 60.0f, CP_System_GetWindowHeight() - 120.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

	/* ------------------ Current score display ------------------ */
	CP_Settings_TextSize(75.0f);
	CP_Font_Set(sub_font);
	CP_Settings_Fill(button_blue);
	CP_Font_DrawText("Your Score:", center_x, center_y - 250);

	sprintf_s(score_text, sizeof(score_text), "%d", GetTotalEarned());
	CP_Settings_Fill(white);
	CP_Graphics_DrawRect(center_x, center_y - 100, 400, 150);     // score background box
	CP_Settings_Fill(button_blue);
	CP_Settings_TextSize(150.0f);
	CP_Font_DrawText(score_text, center_x, center_y - 100);
	CP_Settings_NoStroke();

	/* ------------------ New highscore overlay + name input ------------------ */

	// only show name entry overlay if player beat the lowest leaderboard score
	if (GetTotalEarned() > lowest_score) {
		// confetti effect while highscore
		UpdateConfetti(CP_System_GetDt());

		// overlay animation state
		if (!name_entered) {
			overlay_y = CP_System_GetWindowHeight() / 2.0f; // center overlay while typing
			overlay_down = 0;
		}
		else {
			overlay_down = 1; // start sliding away after name confirmed
			overlay_y += 1500 * (int)CP_System_GetDt(); // moves overlay down (dt cast to int here)
			overlay_y = (float)CP_Math_ClampInt((int)overlay_y,
				CP_System_GetWindowHeight() / 2,
				(int)(CP_System_GetWindowHeight() * 1.5));
			CP_Settings_Stroke(white);
			CP_Settings_NoStroke();
		}

		// dim background with a full-screen overlay rect
		CP_Settings_Stroke(button_blue);
		CP_Settings_Fill(blue_chalk);
		CP_Graphics_DrawRect(center_x, (float)overlay_y,
			(float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight());

		// read A-Z key input and append to player_name (up to MAX_NAME_LENGTH)
		for (int c = 65; c <= 90; c++) {
			if (CP_Input_KeyTriggered(c) && (strlen(player_name) < MAX_NAME_LENGTH)) {
				size_t len = strlen(player_name);
				player_name[len] = c;         // store the typed character
				player_name[len + 1] = '\0';  // keep it null-terminated
			}
		}

		// backspace deletes last character
		if (CP_Input_KeyTriggered(KEY_BACKSPACE) && strlen(player_name) > 0) {
			player_name[strlen(player_name) - 1] = '\0';
		}

		// enter confirms name (only if name not empty)
		if (CP_Input_KeyTriggered(KEY_ENTER) && strlen(player_name) > 0) {
			name_entered = 1;
		}

		// draw overlay score + labels + input box
		CP_Font_Set(sub_font);
		CP_Settings_TextSize(150.0f);
		CP_Settings_Fill(button_blue);
		sprintf_s(score_text, sizeof(score_text), "%d", GetTotalEarned());
		CP_Font_DrawText(score_text, center_x, overlay_y - 200);

		CP_Settings_TextSize(75.0f);
		CP_Settings_Fill(red);
		CP_Font_DrawText("New Highscore!", center_x, overlay_y - 100);

		CP_Font_Set(montserrat_light);
		CP_Settings_Fill(button_blue);
		CP_Settings_TextSize(50.0f);
		CP_Font_DrawText("Enter your name:", center_x, overlay_y);

		// name input box
		CP_Settings_Fill(white);
		CP_Graphics_DrawRect(center_x, overlay_y + 100, 300, 70);
		CP_Settings_Fill(button_blue);
		CP_Font_DrawText(player_name, center_x, overlay_y + 100);
		CP_Settings_NoStroke();

		// confetti again (extra update call)
		UpdateConfetti(CP_System_GetDt());
	}

	/* ------------------ Hover + click handling for buttons ------------------ */

	// reset hover pop amounts every frame before checking hover
	try_pop = menu_pop = exit_pop = leaderboard_pop = credits_pop = 0;

	// only allow button interaction when overlay is not blocking input
	if (overlay_down) {
		// credits hover/click
		if (IsAreaClicked(CP_System_GetWindowWidth() - 120.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f, 100.0f, mx, my)) {
			credits_pop = 10;
			BubblesManual(mx, my);

			// tooltip box
			CP_Settings_Fill(button_blue);
			CP_Graphics_DrawRectAdvanced(CP_System_GetWindowWidth() - 120.0f,
				CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f,
				120.0f + credits_pop, 75.0f + credits_pop, 0.0f, 20.0f);

			CP_Font_Set(montserrat_light);
			CP_Settings_Fill(white);
			CP_Settings_TextSize(30.0f);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			CP_Font_DrawText("Credits", CP_System_GetWindowWidth() - 120.0f,
				CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f);

			if (CP_Input_MouseClicked()) {
				CP_Engine_SetNextGameState(credits_Init, credits_Update, credits_Exit);
			}
		}
		// leaderboard hover/click
		else if (IsAreaClicked(CP_System_GetWindowWidth() - 240.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f, 100.0f, mx, my)) {
			leaderboard_pop = 10;
			BubblesManual(mx, my);

			// tooltip box
			CP_Settings_Fill(button_blue);
			CP_Graphics_DrawRectAdvanced(CP_System_GetWindowWidth() - 240.0f,
				CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f,
				170.0f + leaderboard_pop, 75.0f + leaderboard_pop, 0.0f, 20.0f);

			CP_Font_Set(montserrat_light);
			CP_Settings_Fill(white);
			CP_Settings_TextSize(30.0f);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			CP_Font_DrawText("Leaderboard", CP_System_GetWindowWidth() - 240.0f,
				CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f);

			if (CP_Input_MouseClicked()) {
				CP_Engine_SetNextGameState(Leaderboard_Init, Leaderboard_Update, Leaderboard_Exit);
			}
		}
		// try again hover/click
		else if (IsAreaClicked(center_x - 540, buttons_y, 400.0f + menu_pop, 200.0f + menu_pop, mx, my)) {
			try_pop = 10;
			BubblesManual(mx, my);
			if (CP_Input_MouseClicked()) {
				CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
			}
		}
		// main menu hover/click
		else if (IsAreaClicked(center_x, buttons_y, 400.0f + menu_pop, 200.0f + menu_pop, mx, my)) {
			menu_pop = 10;
			BubblesManual(mx, my);
			if (CP_Input_MouseClicked()) {
				CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
			}
		}
		// exit hover/click
		else if (IsAreaClicked(center_x + 540, buttons_y, 400.0f + menu_pop, 200.0f + menu_pop, mx, my)) {
			exit_pop = 10;
			BubblesManual(mx, my);
			if (CP_Input_MouseClicked()) {
				CP_Engine_Terminate();
			}
		}
	}

	// draw bubbles particle effect
	Bubbles_Draw();
}

void Game_Over_Exit(void)
{
	// free images/fonts used by this screen
	FreeImagesFonts();

	/* ------------------ Insert new leaderboard score (if qualifies) ------------------ */

	// if leaderboard not full OR score beats lowest, prepare to insert
	if (entry_count < MAX_LEADERBOARD_ENTRIES || GetTotalEarned() > lowest_score) {

		// if there is room, increase entry_count to include new entry
		if (entry_count < MAX_LEADERBOARD_ENTRIES) {
			entry_count++;
		}

		// place new score at the end (will be sorted next)
		leaderboard[entry_count - 1].score = GetTotalEarned();

		// copy player_name into entry name safely
		strncpy_s(leaderboard[entry_count - 1].name,
			sizeof(leaderboard[entry_count - 1].name),
			player_name, _TRUNCATE);

		// ensure name is null-terminated
		leaderboard[entry_count - 1].name[MAX_NAME_LENGTH] = '\0';

		// sort entries by DESCENDING score (simple bubble/selection style swap)
		for (int i = 0; i < entry_count - 1; i++) {
			for (int j = i + 1; j < entry_count; j++) {
				if (leaderboard[j].score > leaderboard[i].score) {
					Entry temp = leaderboard[i];
					leaderboard[i] = leaderboard[j];
					leaderboard[j] = temp;
				}
			}
		}

		// clamp stored entries to the max
		if (entry_count > MAX_LEADERBOARD_ENTRIES)
			entry_count = MAX_LEADERBOARD_ENTRIES;
	}

	/* ------------------ Save leaderboard back to file ------------------ */

	// open leaderboard file for writing (overwrite)
	errno_t err = fopen_s(&leaderboard_file, "Assets/leaderboard.txt", "w");
	if (err != 0 || leaderboard_file == NULL) {
		return; // silently fail if cannot write
	}

	// write each entry: "NAME SCORE"
	for (int i = 0; i < entry_count; i++) {
		fprintf_s(leaderboard_file, "%s %d\n", leaderboard[i].name, leaderboard[i].score);
	}
	fclose(leaderboard_file);

	/* ------------------ Reset game systems for next run ------------------ */
	ResetRoomba();
	Faucet_ResetCooldown();
	ResetMoney();
}
