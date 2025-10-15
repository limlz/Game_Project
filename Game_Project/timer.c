#include "cprocessing.h"

// Timer variable as 100%
static float timer = 100.0f;

// Bool to check if the game is paused
int isGamePaused = 0;

// Function to start the timer countdown, when started timer will decrease
static void timeStart(void) {
	if (timer > 0.0f) {
		timer -= (CP_System_GetDt());
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
int isGamePausedFunc(void) {
	return isGamePaused;
}

// Handles the timer update and pause functionality
static void time_update(void) {
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
		isGamePaused = (isGamePaused == 0) ? 1 : 0;
	}


	if (isGamePausedFunc()) {
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
		//CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		//timeReset();
	}
}

void timer_init() {
	time_update();

	// Draw the timer bar 
	CP_Graphics_DrawQuadAdvanced(
		50, 800,								50 + (1500 * (timer/100.0f)), 800, 
		50 + (1500 * (timer/100.0f)), 880,		50, 880,
		0);

}