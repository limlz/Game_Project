#include "cprocessing.h"
#include "gameover.h"
#include "money.h"
#include "leaderboardentry.h"
#include "leaderboard.h"

// Timer variable as 100%
static float timer = 100.0f;

// Bool to check if the game is paused
int is_game_running = 1;

// NEW: freezes countdown when set (e.g., end-of-day)
static int is_timer_stopped = 0;

int current_score;
int lowest_leaderboard_score;

// Function to start the timer countdown, when started timer will decrease
static void TimeStart(void) {
	if (timer > 0.0f && !is_timer_stopped) {
		timer -= (3 * (CP_System_GetDt()));
	}
}

// Function to stop the timer countdown, when stopped timer will not decrease
void TimeStop(void) {
	is_timer_stopped = 1;
}

// Function to reset the timer back to 100%
void TimeReset(void) {
	timer = 100.0f;
	is_timer_stopped = 0;
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
		// Do not decrement; do not touch isTimerStopped here
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 200));
		CP_Settings_TextSize(100.0f);
		CP_Font_DrawText("GAME PAUSED", 800, 200);
	}
	else {
		TimeStart(); // will only tick if !isTimerStopped
	}



	if (timer <= 0.0f) {
		// Timer has reached zero, TODO: Handle end-of-timer event here
		// 
		// 
		//CP_Engine_SetNextGameState(Game_Over_Init, Game_Over_Update, Game_Over_Exit);
		current_score = GetTotalEarned();
		if (current_score > lowest_leaderboard_score) {
			CP_Engine_SetNextGameState(Leaderboard_Entry_Init, Leaderboard_Entry_Update, Leaderboard_Entry_Exit);
			TimeReset();
		}
		else
		{
			CP_Engine_SetNextGameState(Game_Over_Init, Game_Over_Update, Game_Over_Exit);
			TimeReset();
		}
	}
}

void TimerInit() {
	TimeUpdate();

	CP_Settings_Fill(CP_Color_Create((int)timer, (int)(timer * 2), (int)(255 - timer), 200));
	// Draw the timer bar 
	CP_Graphics_DrawQuadAdvanced(
		50, 820,								50 + (1500 * (timer/100.0f)), 820, 
		50 + (1500 * (timer/100.0f)), 880,		50, 880,
		0);

	//get lowest score from the text file
	lowest_leaderboard_score = GetLowestScore();
}