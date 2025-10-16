#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "mainmenu.h"
#include "sponge.h"
#include "timer.h"
#include "dirt.h"
#include "shop.h"
#include "money.h"

int new_game = 0;
void Game_Init(void)
{
	new_game = 1;
	// Initialise random variable required for bubble production
	Bubbles_Init();
}


void Game_Update(void)
{
	CP_Graphics_ClearBackground(CP_Color_Create(250, 250, 250, 255));

	//Draws the plate 
	// P.S. Added floats to prevent implicit conversion warnings of int to float from compiler
	CP_Settings_Fill(CP_Color_Create(230, 230, 230, 255));
	CP_Graphics_DrawCircle((float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, 600);
	CP_Settings_Fill(CP_Color_Create(210, 210, 210, 255));
	CP_Graphics_DrawCircle((float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, 400);

	//Function to spawn dirts on plate, takes 1 or 0 to decide if it should spawn new dirt in random spots
	spawn_dirt(new_game);

	//Check if N key is clicked, dirt_removed function to check if all dirt is removed. If any true, start new game
	if (CP_Input_KeyTriggered(KEY_N) || dirt_removed()) {
		new_game = 1;
	}
	else {
		new_game = 0;
	}

	money_display();
	
	/*
	If the player is scrubbing, call the dirt_scrubbed function to reduce opacity of dirt
	Also checks to see if the game is running to prevent scrubbing while paused
	
	Call dirt scrubbed function with parameters 
		- Check if sponge is equipped
		- Check sponge power level
	*/ 
	if (is_Scrubbing() && checkGameRunning()) {
		dirt_scrubbed(is_SpongeEquipped(), get_SpongePower());
	}

	shop_init();

	
	// Bubble drawing function
	Bubbles_UpdateAndDraw();
	sponge_init();
	timer_init();
}

void Game_Exit(void)
{

}

