/*************************************************************************
@file tuturial.c
@Author Nur Nadia Natasya Binte Mohd Taha (nurnadianatasya.b@digipen.edu)
@Co-authors	Lim Liang Zhou (l.liangzhou@digipen.edu)
@brief	This file contains the function definitions for the tutorial system, 
		including starting the tutorial, displaying tutorial steps, and 
		managing user interactions. The tutorial guides players through the game's
		mechanics, such as equipping the sponge, using abilities, and navigating 
		the shop. The tutorial is designed around Day 0 of the game.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "game.h"
#include "img_font_init.h"
#include "day.h"
#include "cprocessing.h"
#include "timer.h"
#include "utils.h"
#include "sponge.h"
#include "dirt.h"
#include "faucet.h"
#include "shop.h"
#include "roomba.h"

#define TIME 3.0f

int tutorial_need_answer	= 1; // Flag: should the game ask player whether they want a tutorial?
int start_tutorial			= 0; // Flag: tutorial intro screen currently active
int is_tutorial_active		= 0; // Main tutorial sequence active
int tutorial_step			= 0; // Tracks which step of the tutorial the player is currently on
int sponge_can_scrub		= 1; // Controls whether the sponge is allowed to scrub
int text_placement			= 1; // Controls textbox orientation and position
int standard_textbox_values = 1; // If 1, use default textbox position based on hamsta
int back_pop				= 0; // Used for button animation when clicking "Back"

// Hamsta & textbox layout
float hamsta_x       = 180.0f;
float hamsta_y       = 570.0f;
float ham_angle      = 0.0f;
float width          = 350.0f;

float draw_text_x,	draw_text_y1,	draw_text_y2;
float textbox_x,	textbox_y,		textbox_x_below,	textbox_y_below;

CP_Image pointer_arm;

void TutorialStart(void) {
	// Compute standard textbox positions unless custom ones are used
	if (standard_textbox_values) {
        textbox_x       = hamsta_x + (width * 0.5f - 70.0f);
        textbox_y       = hamsta_y - 145.0f;
        textbox_x_below = hamsta_x - (width * 0.5f - 70.0f);
        textbox_y_below = hamsta_y + 110.0f;
	}
	else {
		// Manual positioning for later tutorial stages
		textbox_x = 1450.0f + (width * 0.5f - 70.0f);
		textbox_y = 710.0f - 145.0f;
	}
	
	// Adjust text draw position depending on placement style
	if (text_placement == 1) //text above hamsta, left-aligned
	{ 
		draw_text_x		= (textbox_x - (width * 0.5f)) + 15.0f;
		draw_text_y1	= textbox_y - 15;
		draw_text_y2	= textbox_y + 15;
	}
	else if (text_placement == 2) //text below hamsta, right-aligned
	{ 
		draw_text_x		= (textbox_x_below - (width * 0.5f)) + 15.0f;
		draw_text_y1	= textbox_y_below - 15;
		draw_text_y2	= textbox_y_below + 15;
	}
	else if (text_placement == 3) //text below hamsta, left-aligned
	{ 
		draw_text_x		= (textbox_x - (width * 0.5f)) + 15.0f;
		draw_text_y1	= textbox_y_below - 15;
		draw_text_y2	= textbox_y_below + 15;
	}
	else //text above hamsta, right-aligned
	{ 
		draw_text_x		= (textbox_x_below - (width * 0.5f)) + 15.0f;
		draw_text_y1	= textbox_y - 15;
		draw_text_y2	= textbox_y + 15;

	}
	
	// Draw tutorial visuals only if active
	if (is_tutorial_active) 
	{	
		// Draw hamsta + pointer arm
		CP_Image_Draw(hamsta, hamsta_x, hamsta_y, 150.0f, 150.0f, 255);
		CP_Image_DrawAdvanced(pointer_arm, hamsta_x + 55.0f, hamsta_y, 180.0f, 150.0f, 255, ham_angle);
		CP_Settings_Fill(white);

		if (text_placement == 1) 
		{
			CP_Graphics_DrawRectAdvanced(textbox_x, textbox_y, width, 100.0f, 0.0f, 20.0f);
		}
		else if (text_placement == 2) 
		{
			CP_Graphics_DrawRectAdvanced(textbox_x_below, textbox_y_below, width, 100.0f, 0.0f, 20.0f);
		}
		else if (text_placement == 3) 
		{
			CP_Graphics_DrawRectAdvanced(textbox_x, textbox_y_below, width, 100.0f, 0.0f, 20.0f);
		}
		else 
		{
			CP_Graphics_DrawRectAdvanced(textbox_x_below, textbox_y, width, 100.0f, 0.0f, 20.0f);
		}

		CP_Font_Set(montserrat_light);
		CP_Settings_Fill(black);
		CP_Settings_TextSize(30.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);

		// --- Tutorial step-by-step logic ---
		if (tutorial_step == 0) 
		{
			pointer_arm = arm_arrow_right;
			ham_angle	= 45.0f;

			CP_Font_DrawText("First, click and drag the", draw_text_x, draw_text_y1);
			CP_Font_DrawText("sponge to clean the plates!", draw_text_x, draw_text_y2);

			if (IsSpongeEquipped() == 1) 
			{
				tutorial_step = 1;
			}
		}
		else if (tutorial_step == 1) 
		{
			ham_angle = 0.0f;

			CP_Font_DrawText("Great! Now, let's get to", draw_text_x, draw_text_y1);
			CP_Font_DrawText("scrubbing those plates!", draw_text_x, draw_text_y2);

			if (GetTotalOpacity() < 1) 
			{
				tutorial_step = 2;
			}
		}
		else if (tutorial_step == 2) 
		{
			sponge_can_scrub	= 0;
			pointer_arm			= arm_arrow_left;
			ham_angle			= 315.0f;

			CP_Font_DrawText("Now, witness some magic", draw_text_x, draw_text_y1);
			CP_Font_DrawText("by pressing W!", draw_text_x, draw_text_y2);

			if (Faucet_ReturnStreamOn()) 
			{
				tutorial_step = 3;
			}
		}
		else if (tutorial_step == 3) 
		{
			pointer_arm	= arm_arrow_right;
			ham_angle	= 0.0f;
			width		= 320.0f;

			CP_Font_DrawText("Wow, look at this water!", draw_text_x, draw_text_y1);
			CP_Font_DrawText("So fast!", draw_text_x, draw_text_y2);
			
			if (!Faucet_ReturnStreamOn()) 
			{
				tutorial_step = 4;
			}
		}
		else if (tutorial_step == 4) 
		{
			pointer_arm = arm_arrow_left;
			ham_angle	= 315.0f;
			width		= 270.0f;

			CP_Font_DrawText("Ability on cooldown!", draw_text_x, draw_text_y1);
			CP_Font_DrawText("Hang tight!", draw_text_x, draw_text_y2);

			static float timer_for_next_step = TIME;
			timer_for_next_step -= CP_System_GetDt();

			if (timer_for_next_step <= 0.0f) 
			{
				tutorial_step = 5;
			}
		}
		else if (tutorial_step == 5) 
		{
			text_placement	= 2;
			pointer_arm		= arm_arrow_right;
			hamsta_x		= 1440.0f;
			hamsta_y		= 260.0f;
			ham_angle		= 315.0f;
			width			= 350.0f;

			CP_Font_DrawText("Time to polish those skils!", draw_text_x, draw_text_y1);
			CP_Font_DrawText("Press F to visit the shop!", draw_text_x, draw_text_y2);

			if (IsShopOpen() == 1) 
			{
				tutorial_step = 6;
			}
		}
		else if (tutorial_step == 6) 
		{
			pointer_arm = arm_arrow_left;
			hamsta_x	= 1520.0f;
			hamsta_y	= 350.0f;
			ham_angle	= 345.0f;
			width		= 270.0f;

			CP_Font_DrawText("Upgrage your skills!", draw_text_x, draw_text_y1);
			CP_Font_DrawText("Be the very best!", draw_text_x, draw_text_y2);

			static float timer_for_next_step = TIME;
			timer_for_next_step -= CP_System_GetDt();

			if (timer_for_next_step <= 0.0f) 
			{
				ForceCloseShop();
				tutorial_step = 7;
			}
		}
		else if (tutorial_step == 7) 
		{
			text_placement	= 3;
			pointer_arm		= arm_arrow_left;
			hamsta_x		= 120.0f;
			hamsta_y		= 240.0f;
			ham_angle		= 45.0f;
			width			= 300.0f;

			CP_Font_DrawText("Here's your target for", draw_text_x, draw_text_y1);
			CP_Font_DrawText("the day! Be quick!", draw_text_x, draw_text_y2);

			static float timer_for_next_step = TIME;
			timer_for_next_step -= CP_System_GetDt();

			if (timer_for_next_step <= 0.0f) 
			{
				ForceCloseShop();
				tutorial_step = 8;
			}
		}
		else if (tutorial_step == 8) 
		{
			text_placement	= 4;
			hamsta_x		= 1490.0f;
			hamsta_y		= 710.0f;
			pointer_arm		= arm_arrow_left;
			ham_angle		= 315.0f;
			width			= 320.0f;

			CP_Font_DrawText("Here's the timer!", draw_text_x, draw_text_y1);
			CP_Font_DrawText("Finish before time's up!", draw_text_x, draw_text_y2);

			static float timer_for_next_step = TIME;
			timer_for_next_step -= CP_System_GetDt();

			if (timer_for_next_step <= 0.0f) 
			{
				tutorial_step = 9;
				TimeReset();
			}
		}
		else if (tutorial_step == 9) 
		{
			sponge_can_scrub			= 1;
			hamsta_x					= 2000.0f;
			hamsta_y					= 2000.0f;
			width						= 355.0f;
			standard_textbox_values		= 0;
			text_placement				= 4;

			PurchaseRoomba();
			CP_Settings_TextSize(30.0f);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);

			static float timer_for_next_step = TIME*2;
			timer_for_next_step -= CP_System_GetDt();

			if (timer_for_next_step <= 0.0f) 
			{
				CP_Font_DrawText("Now, get to work!", draw_text_x, draw_text_y1);
				CP_Font_DrawText("These plates should SHINE!", draw_text_x, draw_text_y2);
			}
			else 
			{
				CP_Font_DrawText("You can purchase me too!", draw_text_x, draw_text_y1);
				CP_Font_DrawText("Shiny plates await!", draw_text_x, draw_text_y2);
				CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			}	
		}
	}	
}


	void TutorialDayZero(void) 
	{
		TutorialStart();
		if (start_tutorial) 
			// Freeze gameplay while presenting intro
		{
			/* Tutorial Day Zero – initial intro screen with character dialogue */
			TimeStop(); // Freeze gameplay behind the tutorial overlay

			float mid_x		= CP_System_GetWindowWidth() * 0.5f;
			float mid_y		= CP_System_GetWindowHeight() * 0.5f;
			float width		= mid_x + 95.0f;
			float height	= mid_y / 3.0f;

			CP_Settings_Fill(CP_Color_Create(0, 0, 0, 175));
			CP_Graphics_DrawRect(mid_x, mid_y, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight());

			/* --- Background Overlay --- */
			CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 155));
			CP_Settings_Fill(CP_Color_Create(230, 230, 230, 255));
			CP_Graphics_DrawCircle((float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, 600);
			CP_Settings_Fill(CP_Color_Create(210, 210, 210, 255));
			CP_Graphics_DrawCircle((float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, 400);

			CP_Image_Draw(hamstamugshot, mid_x, mid_y - 120.0f, 300.0f, 245.0f, 255);

			CP_Settings_NoStroke();
			CP_Settings_Fill(button_blue);
			CP_Graphics_DrawRectAdvanced(mid_x, mid_y, width, height, 0, 20.0f);
			CP_Font_Set(montserrat_light);
			CP_Settings_Fill(white);
			CP_Settings_TextSize(40.0f);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			CP_Font_DrawText("So, you want to be the best dish-washer in town, huh?", mid_x, mid_y - 30.0f);
			CP_Font_DrawText("Come on over and let me show you around...", mid_x, mid_y + 30.0f);

			//button for user
			CP_Settings_Fill(white);
			CP_Settings_Stroke(button_blue);
			CP_Font_Set(sub_font);
			CP_Graphics_DrawRectAdvanced(mid_x, mid_y + 150.0f, 300.0f, 100.0f, 0, 20.0f);
			CP_Settings_Fill(button_blue);
			CP_Font_DrawText("YES, LET'S GO!", mid_x, mid_y + 150.0f);

			if (CP_Input_MouseClicked() && IsAreaClicked(mid_x, mid_y + 150.0f, 300.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
				start_tutorial		= 0;
				is_tutorial_active	= 1;
			}
		}
}

void TutorialYesorNo(void) 
{

	float mid_x = CP_System_GetWindowWidth() * 0.5f;
	float mid_y = CP_System_GetWindowHeight() * 0.5f;

	// Only show Yes/No screen if the tutorial needs player confirmation
	if (tutorial_need_answer) 
	{
		TimeStop();
		CP_Graphics_ClearBackground(blue_chalk);
		CP_Image_Draw(background_image, mid_x, mid_y, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight(), 255);
		CP_Settings_NoStroke();
		CP_Settings_Fill(white);
		CP_Graphics_DrawRectAdvanced(mid_x, mid_y - 85.0f, 700.0f, 75.0f, 0.0f, 20.0f);
		CP_Font_Set(montserrat_light);
		CP_Settings_Fill(black);
		CP_Settings_TextSize(40.0f);
		CP_Image_Draw(tutorial_towel, mid_x, mid_y - 30.0f, 900.0f, 600.0f, 255);
		CP_Font_DrawText("Hey there! Would you like a tutorial?", mid_x, mid_y - 85.0f);

		// two buttons yes and n0
		CP_Settings_Fill(button_blue);
		CP_Graphics_DrawRectAdvanced(mid_x - 120.0f, mid_y, 150.0f, 70.0f, 0, 15.0f);
		CP_Graphics_DrawRectAdvanced(mid_x + 120.0f, mid_y, 150.0f, 70.0f, 0, 15.0f);
		CP_Settings_Fill(white);
		CP_Settings_TextSize(40.0f);
		CP_Font_DrawText("Yes", mid_x - 120.0f, mid_y);
		CP_Font_DrawText("No", mid_x + 120.0f, mid_y);
	}

	if (CP_Input_MouseClicked() && IsAreaClicked(mid_x - 120.0f, mid_y, 150.0f, 70.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY())) 
	{
		tutorial_need_answer	= 0;
		start_tutorial			= 1;
	}
	else if (!start_tutorial && !is_tutorial_active && CP_Input_MouseClicked() && IsAreaClicked(mid_x + 120.0f, mid_y, 150.0f, 70.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY())) 
	{
		tutorial_need_answer = 0;
		DayForceEndDay();
	}
}

void ControlsOverlay(void) 
{
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 175));
	CP_Graphics_DrawRect((float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight());
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 155));
	CP_Image_Draw(hamstamugshot, (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2 - 300.0f, 300.0f, 245.0f, 255);
	CP_Settings_NoStroke();
	CP_Settings_Fill(button_blue);
	CP_Graphics_DrawRectAdvanced((float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, 1200.0f, 500.0f, 0, 20.0f);
	CP_Settings_Fill(blue_chalk);
	CP_Graphics_DrawRectAdvanced((float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2, 1150.0f, 450.0f, 0, 20.0f);

	if (IsAreaClicked((float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2 + 240, 250.0f, 100.0f, CP_Input_GetMouseX(), CP_Input_GetMouseY())) 
	{
		back_pop = 10;
	}

	CP_Settings_Fill(button_blue);
	CP_Graphics_DrawRectAdvanced((float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2 + 240, 250.0f + back_pop, 100.0f + back_pop, 0, 20.0f);
	CP_Settings_Fill(blue_chalk);
	CP_Graphics_DrawRectAdvanced((float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2 + 240, 230.0f + back_pop, 80.0f + back_pop, 0, 20.0f);

	back_pop = 0;
	CP_Font_Set(sub_font);
	CP_Settings_Fill(white);
	CP_Settings_TextSize(40.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_Fill(button_blue);
	CP_Settings_TextSize(90.0f);
	CP_Font_DrawText("Controls", (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2 - 180.0f);
	CP_Settings_TextSize(60.0f);
	CP_Font_DrawText("Back", (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2 + 240);

	CP_Font_Set(montserrat_light);
	CP_Settings_TextSize(40.0f);
	CP_Font_DrawText("Hover over sponge and click E to equip!", (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2 - 90.0f);
	CP_Font_DrawText("Click W to activate faucet/stream function!", (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2 - 30.0f);
	CP_Font_DrawText("Click F to open shop!", (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2 + 30.0f);
	CP_Font_DrawText("Ensure soap level does not reach 0 to continue cleaning!", (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2 + 90.0f);
	CP_Font_DrawText("Good Luck Scrubbing!", (float)CP_System_GetWindowWidth() / 2, (float)CP_System_GetWindowHeight() / 2 + 150.0f);
}

int TutorialIsActive(void)	{	return is_tutorial_active;	}

void ForceTutorialClose(void)	{	is_tutorial_active = 0;	}

void ResetTutorial(void) 
{
	tutorial_need_answer	= 1;
	start_tutorial			= 0;
	is_tutorial_active		= 0;
	tutorial_step			= 0;
}

int SpongeCanScrub(void)	{	return sponge_can_scrub;	}

int TutorialYesNo(void) { return tutorial_need_answer;  }

