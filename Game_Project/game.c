#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "mainmenu.h"
#include "sponge.h"
#include "timer.h"
#include "dirt.h"

int new_game = 0;
void Game_Init(void)
{
	new_game = 1;
}


void Game_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(250, 250, 250, 255));

	//Draws the plate
	CP_Settings_Fill(CP_Color_Create(230, 230, 230, 255));
	CP_Graphics_DrawCircle(CP_System_GetWindowWidth() / 2, CP_System_GetWindowHeight() / 2, 600);
	CP_Settings_Fill(CP_Color_Create(210, 210, 210, 255));
	CP_Graphics_DrawCircle(CP_System_GetWindowWidth() / 2, CP_System_GetWindowHeight() / 2, 400);

	//Function to spawn dirts on plate, takes 1 or 0 to decide if it should spawn new dirt in random spots
	spawn_dirt(new_game);

	//Check if N key is clicked, dirt_removed function to check if all dirt is removed. If any true, start new game
	if (CP_Input_KeyTriggered(KEY_N) || dirt_removed()) {
		new_game = 1;
	}
	else {
		new_game = 0;
	}

	//Takes a variable as to if the sponge is equipped or not + sponge power and reduces opacity of dirt if conditions are met.
	//@dan i think we need to figure out a way to get the info from your funtion.
	dirt_scrubbed(1,3);

	sponge_init();
	timer_init();
}

void Game_Exit(void)
{

}

