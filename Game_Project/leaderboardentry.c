#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "mainmenu.h"
#include "money.h"
#include <stdio.h>
#include "leaderboard.h"

#define OFFSET		150
#define MAX_NAME_LENGTH 3
#define MAX_LEADERBOARD_ENTRIES 5


typedef struct {
	char name[MAX_NAME_LENGTH + 1];
	int score;
} Entry;
Entry leaderboard[MAX_LEADERBOARD_ENTRIES];
int entry_count;
int lowest_score;

FILE* leaderboard_file;

CP_Color white;
CP_Color black;
CP_Color background_colour;
CP_Color button_blue;
CP_Font title_font;
CP_Font montserrat_light;
float mx, my;
char score_text[50];

//variables for keyboard input
char player_name[MAX_NAME_LENGTH + 1];
int name_entered;

void Leaderboard_Entry_Init(void) {
	white = CP_Color_Create(255, 255, 255, 255);
	black = CP_Color_Create(0, 0, 0, 255);
	background_colour = CP_Color_Create(233, 239, 255, 255);
	button_blue = CP_Color_Create(123, 183, 220, 255);
	title_font = CP_Font_Load("Assets/SuperWater.ttf");
	montserrat_light = CP_Font_Load("Assets/MontserratLight.ttf");
	name_entered = 0;
	entry_count = 0;

	// Initialize globals (do NOT shadow)
	player_name[0] = '\0';
	entry_count = 0;

	errno_t err = fopen_s(&leaderboard_file, "Assets/leaderboard.txt", "r");
	if (err != 0 || leaderboard_file == NULL) {
		// file not found or cannot be opened; leave entry_count == 0
		return;
	}

	// Read into the global leaderboard array and update the global entry_count
	while (entry_count < MAX_LEADERBOARD_ENTRIES &&
	       fscanf_s(leaderboard_file, "%3s %d",
	                leaderboard[entry_count].name, MAX_NAME_LENGTH + 1,
	                &leaderboard[entry_count].score) == 2) {
		entry_count++;
	}
	fclose(leaderboard_file);

	lowest_score = GetLowestScore();
}

void Leaderboard_Entry_Update(void) {
	CP_Graphics_ClearBackground(background_colour);

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
	CP_Font_DrawText("ahhhhhh?", center_x, button_y - OFFSET);
	CP_Font_DrawText("Exit to Main Menu", center_x, button_y + OFFSET);

	// Draw score
	CP_Font_Set(title_font);
	CP_Settings_TextSize(80.0f);
	CP_Settings_Fill(button_blue);
	sprintf_s(score_text, sizeof(score_text), "NEW HIGHSCORE : %d", GetTotalEarned());
	CP_Font_DrawText(score_text, 800, 100);

	//get keyboard input for player name
	for (int c = 65; c <= 90; c++) {
		if (CP_Input_KeyTriggered(c) && strlen(player_name) < MAX_NAME_LENGTH) {
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

	CP_Font_DrawText(player_name, 450, 450);
}

void Leaderboard_Entry_Exit(void) {
	CP_Font_Free(title_font);
	CP_Font_Free(montserrat_light);
	

	//inserting of new score //if leaderboard not full or if score is higher than lowest
	if (entry_count < MAX_LEADERBOARD_ENTRIES || GetTotalEarned() > lowest_score) {

		//add new entry
		if (entry_count < MAX_LEADERBOARD_ENTRIES) {
			entry_count++;
		}
		leaderboard[entry_count - 1].score = GetTotalEarned();
		strncpy_s(leaderboard[entry_count - 1].name, sizeof(leaderboard[entry_count - 1].name),player_name,_TRUNCATE);
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
	
	ResetMoney();
}