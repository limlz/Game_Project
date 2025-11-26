/*************************************************************************
@file       leaderboard.c
@Author     Chiu Jun Wen (j.chiu@digipen.edu)
@Co-authors NIL
@brief      This file contains the function definitions for the 
			leaderboard screen	

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/

#include <stdio.h>
#include "leaderboard.h"
#include <limits.h>
#include <cprocessing.h>
#include "money.h"
#include "mainmenu.h"
#include "bubbles.h"	
#include "utils.h"	
#include "img_font_init.h"

#define MAX_NAME_LENGTH 8
#define MAX_LEADERBOARD_ENTRIES 5
CP_TEXT_ALIGN_HORIZONTAL horizontal = CP_TEXT_ALIGN_H_CENTER;
CP_TEXT_ALIGN_VERTICAL vertical = CP_TEXT_ALIGN_V_MIDDLE;

typedef struct {
	char name[MAX_NAME_LENGTH + 1];
	int score;
} Entry;

static Entry leaderboard[MAX_LEADERBOARD_ENTRIES];

typedef struct {
	float x;
	float y;
	float width;
	float height;
	float degrees;
	float corner_radius;
} RectAdvanced;

static RectAdvanced draw_leaderboard[MAX_LEADERBOARD_ENTRIES];

typedef struct {
	float x;
	float y;
} Text;

static Text title;

CP_Font title_font, montserrat_light;
CP_Color black, button_blue, background_colour, white;
CP_Image back_arrow;
static char score_string[9];
static float center_x, center_y;
static int lowest_score;
static char rank[8];
static float mx, my;

FILE* leaderboard_file;

void Leaderboard_Init(void) {

	//load assets
	title_font = CP_Font_Load("Assets/SuperWater.ttf");
	back_arrow = CP_Image_Load("Assets/back_arrow.png");
	montserrat_light = CP_Font_Load("Assets/MontserratLight.ttf");

	//initialise colors
	black = CP_Color_Create(0, 0, 0, 255);
	button_blue = CP_Color_Create(123, 183, 220, 255);
	background_colour = CP_Color_Create(233, 239, 255, 255);
	white = CP_Color_Create(255, 255, 255, 255);

	//calculate center of window
	center_x = CP_System_GetWindowWidth() * 0.5f;
	center_y = CP_System_GetWindowHeight() * 0.5f;

	//get lowest score from leaderboard
	lowest_score = GetLowestScore();

	//set title position
	title.x = center_x;
	title.y = 100.0f;

	//load leaderboard from file
	errno_t err = fopen_s(&leaderboard_file, "Assets/leaderboard.txt", "r");
	if (err != 0 || leaderboard_file == NULL) {
		return;
	}
	for (int j = 0; j < MAX_LEADERBOARD_ENTRIES; j++) {
		fscanf_s(leaderboard_file, "%8s %d",
			leaderboard[j].name, MAX_NAME_LENGTH + 1,
			&leaderboard[j].score);
	}
	fclose(leaderboard_file);

	//initialise bubbles
	BubblesInit();
}


void Leaderboard_Update(void) {

	//clear background
	CP_Graphics_ClearBackground(background_colour);
	CP_Image_Draw(background_image, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);

	//get mouse position
	mx = CP_Input_GetMouseX();
	my = CP_Input_GetMouseY();

	//draw back button
	int leaderboard_pop = 0;
	CP_Settings_NoStroke();
	if (IsAreaClicked(120, (float)CP_System_GetWindowHeight() - 120, 100, 100, mx, my)) {
		leaderboard_pop = 10;
		BubblesManual(mx, my);

		CP_Settings_Fill(button_blue);
		CP_Graphics_DrawRectAdvanced(120, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f, 120.0f, 75.0f, 0.0f, 20.0f);
		CP_Font_Set(montserrat_light);
		CP_Settings_Fill(white);
		CP_Settings_TextSize(30.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("Back", 120, CP_System_GetWindowHeight() - 120.0f - 100.0f - 10.0f);

		if (CP_Input_MouseClicked()) {
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		}
	}

	//title shadow
	CP_Font_Set(title_font);
	CP_Settings_Fill(white);
	CP_Settings_TextSize(80.0f);
	CP_Settings_TextAlignment(horizontal, vertical);
	CP_Font_DrawText("LEADERBOARD", title.x + 10, title.y - 10);

	//draw title
	CP_Font_Set(title_font);
	CP_Settings_TextSize(80.0f);
	CP_Settings_Fill(button_blue);
	CP_Settings_TextAlignment(horizontal, vertical);
	CP_Font_DrawText("LEADERBOARD", title.x, title.y);

	//draw first leaderboard rectangle
	CP_Settings_NoStroke();
	CP_Settings_Fill(button_blue);
	draw_leaderboard[0].width = 900.0f;
	draw_leaderboard[0].height = 100.0f;
	draw_leaderboard[0].x = center_x;
	draw_leaderboard[0].y = draw_leaderboard[0].height / 2.0f + title.y + 50.0f;
	draw_leaderboard[0].degrees = 0.0f;
	draw_leaderboard[0].corner_radius = 20.0f;
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Graphics_DrawRectAdvanced(draw_leaderboard[0].x,
								draw_leaderboard[0].y,
								draw_leaderboard[0].width,
								draw_leaderboard[0].height,
								draw_leaderboard[0].degrees,
								draw_leaderboard[0].corner_radius);
	//draw first leaderboard rank number
	CP_Font_Set(title_font);
	CP_Settings_TextSize(80.0f);
	CP_Settings_TextAlignment(horizontal, vertical);
	CP_Font_DrawText("1", 300, draw_leaderboard[0].y);

	//draw first leaderboard name
	CP_Settings_Fill(background_colour);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, vertical);
	CP_Font_DrawText(leaderboard[0].name, 375, draw_leaderboard[0].y);

	//draw first leaderboard score
	sprintf_s(score_string, sizeof(score_string), "%d", leaderboard[0].score);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, vertical);
	CP_Font_DrawText(score_string, 1225, draw_leaderboard[0].y);

	//draw rest of leaderboard entries
	for (int i = 1; i < MAX_LEADERBOARD_ENTRIES; i++) {
		CP_Settings_NoStroke();
		CP_Settings_Fill(button_blue);
		draw_leaderboard[i].width = 900.0f;
		draw_leaderboard[i].height = 100.0f;
		draw_leaderboard[i].x = center_x;
		draw_leaderboard[i].y = draw_leaderboard[i].height / 2.0f + draw_leaderboard[i - 1].y + 70.0f;
		draw_leaderboard[i].degrees = 0.0f;
		draw_leaderboard[i].corner_radius = 20.0f;
		CP_Graphics_DrawRectAdvanced(draw_leaderboard[i].x,
									 draw_leaderboard[i].y,
									 draw_leaderboard[i].width,
									 draw_leaderboard[i].height,
									 draw_leaderboard[i].degrees,
									 draw_leaderboard[i].corner_radius);
		CP_Settings_TextAlignment(horizontal, vertical);
		sprintf_s(rank, sizeof(rank), "%d", i + 1);
		CP_Font_DrawText(rank, 300, draw_leaderboard[i].y);
		CP_Settings_Fill(background_colour);

		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, vertical);
		CP_Font_DrawText(leaderboard[i].name, 375, draw_leaderboard[i].y);

		sprintf_s(score_string, sizeof(score_string), "%d", leaderboard[i].score);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, vertical);
		CP_Font_DrawText(score_string, 1225, draw_leaderboard[i].y);
	}

	// Draw back button
	CP_Image_Draw(back_arrow, 120.0f, CP_System_GetWindowHeight() - 120.0f, 100.0f + leaderboard_pop, 100.0f + leaderboard_pop, 255);

	//draw bubbles
	Bubbles_Draw();
}

void Leaderboard_Exit(void) {
	//free assets
	CP_Font_Free(montserrat_light);
	CP_Font_Free(title_font);
	CP_Image_Free(back_arrow);
}

int GetLowestScore(void) {
	FILE* leaderboard_file;
	char name[MAX_NAME_LENGTH + 1];
	int score = 0;
	int lowest = INT_MAX;

	//open leaderboard file
	errno_t err = fopen_s(&leaderboard_file, "Assets/leaderboard.txt", "r");
	if (err != 0 || leaderboard_file == NULL) {
		return -1;
	}
	//read through file to find lowest score
	while (fscanf_s(leaderboard_file, "%8s %d", name, (unsigned)(sizeof(name) / sizeof(name[0])), &score) == 2) {
		if (score < lowest) {
			lowest = score;
		}
	}
	//close file and return lowest score
	fclose(leaderboard_file);
	return lowest;
}

int GetHighestScore(void) {
	FILE* leaderboard_file;
	char name[MAX_NAME_LENGTH + 1];
	int score = 0;
	int highest = 0;

	//open leaderboard file
	errno_t err = fopen_s(&leaderboard_file, "Assets/leaderboard.txt", "r");
	if (err != 0 || leaderboard_file == NULL) {
		return -1;
	}

	//read through file to find highest score
	while (fscanf_s(leaderboard_file, "%8s %d", name, (unsigned)(sizeof(name) / sizeof(name[0])), &score) == 2) {
		if (score > highest) {
			highest = score;
		}
	}

	//close file and return highest score
	fclose(leaderboard_file);
	return highest;
}
