#include "cprocessing.h"
#include "utils.h"
#include "game.h"
#include "mainmenu.h"
#include "sponge.h"
#include "timer.h"
#include "dirt.h"
#include "shop.h"
#include "money.h"
#include "sounds.h"
#include "bubbles.h"
#include "plate.h"
#include "roomba.h"
#include "day.h"
#include "faucet.h"


int roomba_activated = 0;
CP_Font gameFont;

void Game_Init(void)
{	
	gameFont = CP_Font_Load("Assets/MontserratLight.ttf");
	// Initialise random variable required for bubble production
	Bubbles_Init();
	init_scrubbing_sounds();
	update_volumes();
	init_roomba();
	init_dirt();
	Day_Init();
	stream_init();
	Day_StartCurrentDay();   // begin Day 0 (goal = 5 plates)

}


void Game_Update(void)
{
	CP_Font_Set(gameFont);

	//General UI - scene setting (sink base)
	CP_Graphics_ClearBackground(CP_Color_Create(233, 239, 255, 255));
	CP_Settings_Fill(CP_Color_Create(186, 191, 197, 255));
	CP_Settings_NoStroke();
	CP_Graphics_DrawRect((float)CP_System_GetWindowWidth() * 0.5f, 30.0f, (float)CP_System_GetWindowWidth(), 60.0f);
	CP_Graphics_DrawRect((float)CP_System_GetWindowWidth() * 0.5f, 850.0f, (float)CP_System_GetWindowWidth(), 100.0f);

	draw_plate();
	draw_faucet();

	//Function to spawn dirts on plate, takes 1 or 0 to decide if it should spawn new dirt in random spots
	draw_dirt();

	//Check if N key is clicked, dirt_removed function to check if all dirt is removed. If any true, start new game
	if (Day_IsInGameplay() && (CP_Input_KeyTriggered(KEY_N) || dirt_removed())) {
		Day_OnPlateCleaned();

		// Only spawn a new plate if still in gameplay (not in shop phase)
		if (Day_IsInGameplay()) {
			init_dirt();
			change_plate();
		}
	}



	// displays current money
	money_display();
	Day_DrawHUD(80.0f, 80.0f);

	
	/*
	If the player is scrubbing, call the dirt_scrubbed function to reduce opacity of dirt
	Also checks to see if the game is running to prevent scrubbing while paused
	
	Call dirt scrubbed function with parameters 
		- Check if sponge is equipped
		- Check sponge power level
	*/ 
	if (is_Scrubbing() && checkGameRunning()) {
		dirt_scrubbed(is_SpongeEquipped(), get_SpongePower());
		start_scrubbing_sound();
	}
	else {
		stop_scrubbing_sound();
	}

	shop_init();
	
	
	// Bubble drawing function
	Bubbles_Draw();


	//temporary roomba functions
	if (CP_Input_KeyTriggered(KEY_R)) {
		roomba_activated = 1;
	}
	else if (CP_Input_KeyTriggered(KEY_T)) {
		roomba_activated = 0;
	}
	if (roomba_activated) {
		roomba();
	}

	//update_stream();
	sponge_init();
	timer_init();
	Day_DrawPopup();
}

void Game_Exit(void)
{
	CP_Font_Free(gameFont);
	clear_sounds();
}

