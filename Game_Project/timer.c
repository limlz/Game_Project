#include "cprocessing.h"
#include "gameover.h"

// Timer variable as 100%
static float timer = 100.0f;

// Bool to check if the game is paused
int isGameRunning = 1;

// Function to start the timer countdown, when started timer will decrease
static void timeStart(void) {
	if (timer > 0.0f) {
		timer -= (3 * (CP_System_GetDt()));
	}
}

// Function to stop the timer countdown, when stopped timer will not decrease
static void timeStop(void) {
	timer += 0;
}

// Function to reset the timer back to 100%
static void timeReset(void) {
	timer = 100.0f;
}

// Function to check if the game is paused, can be used in other files so that
// players cannot move while the game is paused
int checkGameRunning(void) {
	return isGameRunning;
}

// Handles the timer update and pause functionality
static void time_update(void) {
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
		isGameRunning = (isGameRunning == 1) ? 0 : 1;
	}


	if (!checkGameRunning()) {
		timeStop();

		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 200));
		CP_Settings_TextSize(100.0f);
		CP_Font_DrawText("GAME PAUSED", 800, 200);
	}
	else {
		timeStart();
	}


	if (timer <= 0.0f) {
		// Timer has reached zero, TODO: Handle end-of-timer event here
		// 
		// 
		CP_Engine_SetNextGameState(Game_Over_Init, Game_Over_Update, Game_Over_Exit);
		timeReset();
	}
}

void timer_init() {
	time_update();


	CP_Settings_Fill(CP_Color_Create(timer, timer * 2, 255 - timer, 200));
	// Draw the timer bar 
	CP_Graphics_DrawQuadAdvanced(
		50, 800,								50 + (1500 * (timer/100.0f)), 800, 
		50 + (1500 * (timer/100.0f)), 880,		50, 880,
		0);

}