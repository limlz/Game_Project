#include <stdio.h>
#include "leaderboard.h"
#include <limits.h>
#include <cprocessing.h>
#include "money.h"
#include "mainmenu.h"

#define MAX_ENTRIES 5

CP_Font title_font, montserrat_light;
CP_Color black;
char score_string[9];
int lowest_score;

int GetLowestScore(void) {
	FILE* leaderboard_file;
	char name[4];
	int score = 0;
	int lowest = INT_MAX;

	errno_t err = fopen_s(&leaderboard_file, "Assets/leaderboard.txt", "r");
	if (err != 0 || leaderboard_file == NULL) {
		return -1;
	}
	while (fscanf_s(leaderboard_file, "%3s %d", name, (unsigned)(sizeof(name)/sizeof(name[0])), &score) == 2) {
		if (score < lowest) {
			lowest = score;
		}
	}
	fclose(leaderboard_file);
	return lowest;
}



void Leaderboard_Init(void) {
	title_font = CP_Font_Load("Assets/SuperWater.ttf");
	montserrat_light = CP_Font_Load("Assets/MontserratLight.ttf");
	black = CP_Color_Create(0, 0, 0, 255);
	lowest_score = GetLowestScore();
}
void Leaderboard_Update(void) {
	CP_Graphics_ClearBackground(CP_Color_Create(233, 239, 255, 255));
	CP_Font_Set(montserrat_light);
	CP_Settings_Fill(black);
	CP_Settings_TextSize(60.0f);
	sprintf_s(score_string, sizeof(score_string),"%d", lowest_score);
	CP_Font_DrawText(score_string, 450.0f, 450.0f);

	if (CP_Input_KeyTriggered(KEY_C)) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
}
void Leaderboard_Exit(void) {
	CP_Font_Free(montserrat_light);
	CP_Font_Free(title_font);
}