/*************************************************************************
@file
@Author
@Co-authors
@brief

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include "gameover.h"
#include "money.h"
#include "leaderboard.h"
#include "img_font_init.h"
#include "settings.h"
#include "mainmenu.h"

// Timer variable as 100%
static float timer = 100.0f;
static float max_timer = 100.0f;

// Bool to check if the game is paused
int is_game_running = 1;
int blink_toggle = 0;
struct blinking_timer {
	int speed;
};
// NEW: freezes countdown when set (e.g., end-of-day)
static int is_timer_stopped = 0;

int current_score;
int lowest_leaderboard_score;

int timer_opacity = 0;

// Function to start the timer countdown, when started timer will decrease
static void TimeStart(void) {
	if (timer > 0.0f && !is_timer_stopped) {
		timer -= (2 * (CP_System_GetDt()));
	}
}


// Function to stop the timer countdown, when stopped timer will not decrease
void TimeStop(void) {
	is_timer_stopped = 1;
}

// Function to reset the timer back to 100%
void TimeReset(void) {
	timer = max_timer;
	is_timer_stopped = 0;
}

// Function to add time to the timer, can be used after clearing plates
void TimeAdd(float add_time) {
	timer = (timer > max_timer) ? timer : timer + add_time;
}
// Function to check if the game is paused, can be used in other files so that
// players cannot move while the game is paused
int CheckGameRunning(void) {
	return is_game_running;
}

// Handles the timer update and pause functionality
static void TimeUpdate(void) {
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
		is_game_running = (is_game_running == 1) ? 0 : 1;
	}

	if (!CheckGameRunning()) {
		UnequipSponge();
		float mx = CP_Input_GetMouseX();
		float my = CP_Input_GetMouseY();
		float center_X = CP_System_GetWindowWidth() / 2.0f;
		float center_Y = CP_System_GetWindowHeight() / 2.0f;

		float btn_width = 300.0f;
		float btn_height = 150.0f;
		float gap = btn_height + 50.0f;

		// Do not decrement; do not touch isTimerStopped here
		CP_Graphics_ClearBackground(blue_chalk);
		CP_Font_Set(sub_font);
		CP_Settings_Fill(button_blue);
		CP_Settings_TextSize(100.0f);
		CP_Font_DrawText("GAME PAUSED", center_X, center_Y - gap);

		float buttons_Y[2] = { center_Y, center_Y + gap };
		const char* btnText[2] = { "Resume Game", "Save & Exit" };
		CP_Color btnColor[2] = {
			CP_Color_Create(136, 172, 219,255),
			CP_Color_Create(81, 103, 131, 255),
		};

		for (int i = 0; i < 2; ++i) {
			CP_Settings_NoStroke();
			CP_Settings_Fill(button_blue);
			CP_Graphics_DrawRectAdvanced(center_X, buttons_Y[i], btn_width, btn_height, 0, 25.0f);
			CP_Settings_Fill(white);
			CP_Font_Set(montserrat_light);
			CP_Settings_TextSize(45.0f);
			CP_Font_DrawText(btnText[i], center_X, buttons_Y[i]);

			if (CP_Input_MouseClicked()) {
				int on_button = (mx >= center_X - btn_width * 0.5f && mx <= center_X + btn_width * 0.5f 
					&& my >= buttons_Y[i] - btn_height * 0.5f && my <= buttons_Y[i] + btn_height * 0.5f);
				if (on_button) {
					switch (i) {
					case 0: is_game_running = 1; break;
					case 1: CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit); break;
					}
				}
			}
		}
	}
	else {
		TimeStart(); // will only tick if !isTimerStopped
	}
			

	if (timer <= 0.0f) {
		// Timer has reached zero, TODO: Handle end-of-timer event here
		//CP_Engine_SetNextGameState(Game_Over_Init, Game_Over_Update, Game_Over_Exit);
		current_score = GetTotalEarned();
		CP_Engine_SetNextGameState(Game_Over_Init, Game_Over_Update, Game_Over_Exit);
		TimeReset();
	}
}

void TimerBlink(struct blinking_timer *blink) {
	if (timer <= 30.0f) {
		if (timer_opacity <= 0)
			blink_toggle = 1;
		else if (timer_opacity >= 200)
			blink_toggle = 0;
		timer_opacity = (blink_toggle) ? timer_opacity + blink -> speed * CP_System_GetDt() : timer_opacity + blink -> speed * -CP_System_GetDt();
	}
	else {
		timer_opacity = 200;
	}
}

void TimerInit() {
	struct blinking_timer blink;
	blink.speed = 200;
	
	TimeUpdate();
	TimerBlink(&blink);

	CP_Settings_Stroke(black);
	CP_Settings_Fill(CP_Color_Create((int)timer, (int)(timer * 2), (int)(255 - timer), timer_opacity));
	// Draw the timer bar 
	CP_Graphics_DrawQuadAdvanced(
		50, 820,								50 + (1500 * (timer/100.0f)), 820, 
		50 + (1500 * (timer/100.0f)), 880,		50, 880,
		0);

	//get lowest score from the text file
	lowest_leaderboard_score = GetLowestScore();
}

int IsTimerStopped(void) {
	return is_timer_stopped;
}
