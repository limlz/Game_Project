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

#define OFFSET		150
#define MAX_NAME_LENGTH 8
#define MAX_LEADERBOARD_ENTRIES 5



typedef struct {
	char name[MAX_NAME_LENGTH + 1];
	int score;
} Entry;
static Entry leaderboard[MAX_LEADERBOARD_ENTRIES];
static int entry_count = 0;
static int lowest_score;
static int high_score;
static float overlay_y = 0;
static int overlay_down = 1;
int try_pop = 0;
int menu_pop = 0;
int exit_pop = 0;
int leaderboard_pop = 0;
int credits_pop = 0;

FILE* leaderboard_file;

static float mx, my;
static char score_text[50];
static char lowest_text[10];
static char highest_text[64];

//variables for keyboard input
static char player_name[MAX_NAME_LENGTH + 1];
static int name_entered;

void Game_Over_Init(void)
{
	InitImagesFontsColors();
	lowest_score = GetLowestScore();
	high_score = GetHighestScore();
	name_entered = 0;

	player_name[0] = '\0';

	errno_t err = fopen_s(&leaderboard_file, "Assets/leaderboard.txt", "r");
	if (err != 0 || leaderboard_file == NULL) {
		return;
	}

	while (entry_count < MAX_LEADERBOARD_ENTRIES &&
		fscanf_s(leaderboard_file, "%8s %d",
			leaderboard[entry_count].name, MAX_NAME_LENGTH + 1,
			&leaderboard[entry_count].score) == 2) {
		entry_count++;
	}
	fclose(leaderboard_file);

	lowest_score = GetLowestScore();
	printf("Loaded %d entries from file\n", entry_count);

	InitConfetti();
	BubblesInit();

}

void Game_Over_Update(void)
{
	CP_Graphics_ClearBackground(blue_chalk);
	CP_Image_Draw(background_image, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);

	float center_x = CP_System_GetWindowWidth() * 0.5f;
	float center_y = CP_System_GetWindowHeight() * 0.5f;
	float buttons_y = center_y + 130;
	mx = CP_Input_GetMouseX();
	my = CP_Input_GetMouseY();

	// Draw rectangles for button_blue
	CP_Settings_Fill(button_blue);
	CP_Settings_NoStroke();
	CP_Graphics_DrawRectAdvanced(center_x+540, buttons_y, 400.0f + exit_pop, 200.0f + exit_pop, 0, 50);
	CP_Graphics_DrawRectAdvanced(center_x-540, buttons_y, 400.0f + try_pop, 200.0f + try_pop, 0, 50);
	CP_Graphics_DrawRectAdvanced(center_x, buttons_y, 400.0f + menu_pop, 200.0f + menu_pop, 0, 50);


	// Draw text for button_blue
	CP_Font_Set(montserrat_light);
	CP_Settings_Fill(white);
	CP_Settings_TextSize(60.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("Try Again", center_x-540, buttons_y);
	CP_Font_DrawText("Main Menu", center_x, buttons_y);
	CP_Font_DrawText("Exit", center_x+540, buttons_y);



	// Draw leaderboard button
	CP_Image_Draw(leaderboard_icon, CP_System_GetWindowWidth() - 240.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f + leaderboard_pop, 100.0f + leaderboard_pop, 255);

	// Draw Credits button
	CP_Settings_Fill(button_blue);
	CP_Graphics_DrawRectAdvanced(CP_System_GetWindowWidth() - 120.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f + credits_pop, 100.0f + credits_pop, 0.0f, 20.0f);
	CP_Settings_Fill(white);
	CP_Graphics_DrawRectAdvanced(CP_System_GetWindowWidth() - 120.0f, CP_System_GetWindowHeight() - 120.0f, 45.0f, 60.0f, 0.0f, 5.0f);
	CP_Settings_Stroke(button_blue);
	CP_Graphics_DrawLine(CP_System_GetWindowWidth() - 120.0f - 17.0f, CP_System_GetWindowHeight() - 120.0f, CP_System_GetWindowWidth() - 120.0f + 17.0f, CP_System_GetWindowHeight() - 120.0f);
	CP_Graphics_DrawLine(CP_System_GetWindowWidth() - 120.0f - 17.0f, CP_System_GetWindowHeight() - 120.0f + 15.0f, CP_System_GetWindowWidth() - 120.0f + 17.0f, CP_System_GetWindowHeight() - 120.0f + 15.0f);
	CP_Graphics_DrawLine(CP_System_GetWindowWidth() - 120.0f - 17.0f, CP_System_GetWindowHeight() - 120.0f - 15.0f, CP_System_GetWindowWidth() - 120.0f + 17.0f, CP_System_GetWindowHeight() - 120.0f - 15.0f);

	// Draw Highest Score
	CP_Font_Set(sub_font);
	CP_Settings_Fill(button_blue);
	CP_Settings_TextSize(50.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
	if (high_score > GetTotalEarned()) {
		snprintf(highest_text, sizeof(highest_text), "Highest Score: %d", high_score);
	}
	else {
		snprintf(highest_text, sizeof(highest_text), "Highest Score: %d", GetTotalEarned());
	}
	CP_Font_DrawText(highest_text, 60.0f, CP_System_GetWindowHeight() - 120.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

	CP_Settings_TextSize(75.0f);
	CP_Font_Set(sub_font);
	CP_Settings_Fill(button_blue);
	CP_Font_DrawText("Your Score:", center_x, center_y - 250);
	sprintf_s(score_text, sizeof(score_text), "%d", GetTotalEarned());
	CP_Settings_Fill(white);
	CP_Graphics_DrawRect(center_x, center_y - 100, 400, 150);
	CP_Settings_Fill(button_blue);
	CP_Settings_TextSize(150.0f);
	CP_Font_DrawText(score_text, center_x, center_y - 100);
	CP_Settings_NoStroke();

	// Save score if name entered and is a highscore
	if (GetTotalEarned() > lowest_score) {
		//confetti
		UpdateConfetti(CP_System_GetDt()); //confetti hehe

		if (!name_entered) {
			overlay_y = CP_System_GetWindowHeight()/2.0f;
			overlay_down = 0;
		}
		else { 
			overlay_down = 1;
			overlay_y += 1500 * (int)CP_System_GetDt();
			overlay_y = (float)CP_Math_ClampInt((int)overlay_y, CP_System_GetWindowHeight() / 2, (int)(CP_System_GetWindowHeight()*1.5));
			CP_Settings_Stroke(white);
			CP_Settings_NoStroke();
		}

		//overlay

		CP_Settings_Stroke(button_blue);
		CP_Settings_Fill(blue_chalk);
		CP_Graphics_DrawRect(center_x, (float)overlay_y, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight()); //dim background
		//User Input for Name
		for (int c = 65; c <= 90; c++) {
			if (CP_Input_KeyTriggered(c) && (strlen(player_name) < MAX_NAME_LENGTH)) {
				size_t len = strlen(player_name);
				player_name[len] = c;
				player_name[len + 1] = '\0';
			}
		}
		if (CP_Input_KeyTriggered(KEY_BACKSPACE) && strlen(player_name) > 0) {
			player_name[strlen(player_name) - 1] = '\0';
		}
		if (CP_Input_KeyTriggered(KEY_ENTER) && strlen(player_name) > 0) {
			name_entered = 1;
		}


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

		CP_Settings_Fill(white);
		CP_Graphics_DrawRect(center_x, overlay_y + 100, 300, 70);
		CP_Settings_Fill(button_blue);
		CP_Font_DrawText(player_name, center_x, overlay_y + 100); //temporary
		CP_Settings_NoStroke();

		//confetti
		UpdateConfetti(CP_System_GetDt()); //confetti hehe
	}
	try_pop = menu_pop = exit_pop = leaderboard_pop = credits_pop = 0;
	if (overlay_down) {
		if (IsAreaClicked(CP_System_GetWindowWidth() - 120.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f, 100.0f, mx, my)) {
			credits_pop = 10;
			BubblesManual(mx, my);

			CP_Settings_Fill(button_blue);
			CP_Graphics_DrawRectAdvanced(CP_System_GetWindowWidth() - 120.0f, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f, 120.0f + credits_pop, 75.0f + credits_pop, 0.0f, 20.0f);

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
			CP_Graphics_DrawRectAdvanced(CP_System_GetWindowWidth() - 240.0f, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f, 170.0f + leaderboard_pop, 75.0f + leaderboard_pop, 0.0f, 20.0f);

			CP_Font_Set(montserrat_light);
			CP_Settings_Fill(white);
			CP_Settings_TextSize(30.0f);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			CP_Font_DrawText("Leaderboard", CP_System_GetWindowWidth() - 240.0f, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f);

			if (CP_Input_MouseClicked()) {
				CP_Engine_SetNextGameState(Leaderboard_Init, Leaderboard_Update, Leaderboard_Exit);
			}
		}
		else if (IsAreaClicked(center_x - 540, buttons_y, 400.0f + menu_pop, 200.0f + menu_pop, mx, my)) {
			try_pop = 10;
			BubblesManual(mx, my);
			if (CP_Input_MouseClicked()) {
				CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
			}
		}
		else if (IsAreaClicked(center_x, buttons_y, 400.0f + menu_pop, 200.0f + menu_pop, mx, my)) {
			menu_pop = 10;
			BubblesManual(mx, my);
			if (CP_Input_MouseClicked()) {
				CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
			}
		}
		else if (IsAreaClicked(center_x + 540, buttons_y, 400.0f + menu_pop, 200.0f + menu_pop,mx, my)) {
			exit_pop = 10;
			BubblesManual(mx, my);
			if (CP_Input_MouseClicked()) {
				CP_Engine_Terminate();
			}
		}
	}

	Bubbles_Draw();

}

void Game_Over_Exit(void)
{
	FreeImagesFonts();

	//inserting of new score //if leaderboard not full or if score is higher than lowest
	if (entry_count < MAX_LEADERBOARD_ENTRIES || GetTotalEarned() > lowest_score) {

		//add new entry
		if (entry_count < MAX_LEADERBOARD_ENTRIES) {
			entry_count++;
		}
		leaderboard[entry_count - 1].score = GetTotalEarned();
		strncpy_s(leaderboard[entry_count - 1].name, sizeof(leaderboard[entry_count - 1].name), player_name, _TRUNCATE);
		leaderboard[entry_count - 1].name[MAX_NAME_LENGTH] = '\0';

		//sort by descending order
		for (int i = 0; i < entry_count - 1; i++) {
			for (int j = i + 1; j < entry_count; j++) {
				if (leaderboard[j].score > leaderboard[i].score) {
					Entry temp = leaderboard[i];
					leaderboard[i] = leaderboard[j];
					leaderboard[j] = temp;
				}
			}
		}

		//keep only max number of entries
		if (entry_count > MAX_LEADERBOARD_ENTRIES)
			entry_count = MAX_LEADERBOARD_ENTRIES;
	}

	//open leaderboard.txt to write
	errno_t err = fopen_s(&leaderboard_file, "Assets/leaderboard.txt", "w");
	if (err != 0 || leaderboard_file == NULL) {
		return;
	}
	for (int i = 0; i < entry_count; i++) {
		fprintf_s(leaderboard_file, "%s %d\n", leaderboard[i].name, leaderboard[i].score);
	}
	fclose(leaderboard_file);

	ResetRoomba();
	Faucet_ResetCooldown();
	ResetMoney();
}