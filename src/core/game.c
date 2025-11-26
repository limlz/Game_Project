/*************************************************************************
@file		
@Author		
@Co-authors
@brief

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
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
#include "debug.h"
#include "img_font_init.h"
#include "tutorial.h"


int debugging = 0;
CP_Font gameFont;
CP_Font sub_font;

void Game_Init(void)
{
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	InitImagesFontsColors();
	// Initialise random variable required for bubble production
	BubblesInit();
	InitScrubbingSounds();
	UpdateVolume();
	InitRoomba();
	InitDirt();
	DayInit();
	InitStream();
	DayStartCurrentDay();   // begin Day 0 (goal = 5 plates)
	SoapInit();
	SpongeReset();
	EasyModeChangePlate();

}

void Game_Update(void)
{
	CP_Font_Set(gameFont);

	//General UI - scene setting (sink base) pt 1
	CP_Graphics_ClearBackground(blue_chalk);
	CP_Settings_Fill(blue_grey);
	CP_Settings_NoStroke();
	CP_Graphics_DrawRect((float)CP_System_GetWindowWidth() * 0.5f, 30.0f, (float)CP_System_GetWindowWidth(), 60.0f);

	DrawPlate();
	DrawFaucet();

	//Function to spawn dirts on plate, takes 1 or 0 to decide if it should spawn new dirt in random spots
	DrawDirt();

	//Check if N key is clicked, DirtRemoved function to check if all dirt is removed. If any true, start new game
	if (DayIsInGameplay() && (CP_Input_KeyTriggered(KEY_N) || DirtRemoved())) {
		DayOnPlateCleaned();

		// Only spawn a new plate if still in gameplay (not in shop phase)
		if (DayIsInGameplay()) {
			InitDirt();
			if (DayGetDay() < 3) {
				EasyModeChangePlate();
			}
			else {
				ChangePlate();
			}
		}
	}

	// displays current money
	MoneyDisplay();
	DayDrawHUD(80.0f, 80.0f);

	/*
	If the player is scrubbing, call the DirtScrubbed function to reduce opacity of dirt
	Also checks to see if the game is running to prevent scrubbing while paused

	Call dirt scrubbed function with parameters
		- Check if sponge is equipped
		- Check sponge power level
	*/
	if (IsScrubbing() && CheckGameRunning()) {
		if (SoapCanScrub() && SpongeCanScrub()) {
			DirtScrubbed(IsSpongeEquipped(), GetSpongePower());
			SoapConsumeOnScrub();
			StartScrubbingSounds();
		}
		else {
			StopScrubbingSounds();
		}	
	}
	else {
		StopScrubbingSounds();
	}

	// Bubble drawing function
	Bubbles_Draw();
	if (RoombaPurchase()) {
		RoombaFunction();
	}

	AOEStream();

	//General UI - scene setting (sink base) pt 2
	CP_Settings_Fill(CP_Color_Create(186, 191, 197, 255));
	CP_Settings_NoStroke();
	CP_Graphics_DrawRect((float)CP_System_GetWindowWidth() * 0.5f, 850.0f, (float)CP_System_GetWindowWidth(), 100.0f);

	SpongeInit();
	SoapUpdate();

	if (DayIsInGameplay() && (DayGetDay() == 0)) {
		TimerInit();
		TutorialYesorNo();
		TutorialDayZero();
	} else {
		PurchaseRoomba();
		TimerInit();
	}
	shop_init();

	DayDrawPopup();

	if (CP_Input_KeyDown(KEY_6) && CP_Input_KeyTriggered(KEY_7) && IsCurrentlyDebugging() == 0) {
		StartDebugging();
	} else if (CP_Input_KeyTriggered(KEY_6)) {
		StopDebugging();
	}

	if (IsCurrentlyDebugging()) {
		DebugPrint();
	}
}

void Game_Exit(void) {
	UnequipSponge();
	FreeImagesFonts();
	ClearSounds();
}
