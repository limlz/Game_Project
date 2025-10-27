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
#include "soap.h"


int debugging = 0;
CP_Font gameFont;
CP_Font subFont;

void Game_Init(void)
{	
	gameFont = CP_Font_Load("Assets/MontserratLight.ttf");
	subFont = CP_Font_Load("Assets/MontserratBlackItalic.otf");
	// Initialise random variable required for bubble production
	Bubbles_Init();
	init_scrubbing_sounds();
	update_volumes();
	init_roomba();
	init_dirt();
	Day_Init();
	stream_init();
	Day_StartCurrentDay();   // begin Day 0 (goal = 5 plates)
	Soap_Init();

}


void Game_Update(void)
{
	CP_Font_Set(gameFont);

	//General UI - scene setting (sink base) pt 1
	CP_Graphics_ClearBackground(CP_Color_Create(233, 239, 255, 255));
	CP_Settings_Fill(CP_Color_Create(186, 191, 197, 255));
	CP_Settings_NoStroke();
	CP_Graphics_DrawRect((float)CP_System_GetWindowWidth() * 0.5f, 30.0f, (float)CP_System_GetWindowWidth(), 60.0f);

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
		if (Soap_CanScrub()) {
			dirt_scrubbed(is_SpongeEquipped(), get_SpongePower());
			Soap_ConsumeOnScrub();
			start_scrubbing_sound();
		}
		else {
			stop_scrubbing_sound();
		}
	}
	else {
		stop_scrubbing_sound();
	}

	// Bubble drawing function
	Bubbles_Draw();
	purchase_roomba();
	if (roomba_purchase()) {
		roomba();
	}

	if (CP_Input_KeyDown(KEY_6) && CP_Input_KeyTriggered(KEY_7) && is_currently_debugging() == 0) {
		start_debugging();
	}
	else if (CP_Input_KeyTriggered(KEY_6)) {
		stop_debugging();
	}

	if (is_currently_debugging()) {
		debug_print();
	}

	Soap_Update();
	shop_init();
	

	int attack_opacity = 1; // set as variable to be upgraded

	AOE_stream(attack_opacity);
	sponge_init();

	//General UI - scene setting (sink base) pt 2
	CP_Settings_Fill(CP_Color_Create(186, 191, 197, 255));
	CP_Settings_NoStroke();
	CP_Graphics_DrawRect((float)CP_System_GetWindowWidth() * 0.5f, 850.0f, (float)CP_System_GetWindowWidth(), 100.0f);
	
	// cooldown_timer_stream();
	timer_init();
	Day_DrawPopup();


}

void Game_Exit(void)
{
	CP_Font_Free(gameFont);
	CP_Font_Free(subFont);
	clear_sounds();
	reset_money();
	sell_roomba();
}


