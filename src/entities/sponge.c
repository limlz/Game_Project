/*************************************************************************
@file
@Author
@Co-authors
@brief

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
/* This file contains the definition for the Sponge Mechanics
To create the sponge, #include "sponge.h" and use sponge_init() to spawn the sponge in

Controls: 
To equip the sponge, hover over the sponge and press "E", the sponge will now follow the players mouse.
Press "E" again to unequip the sponge

While holding the sponge, hold down left mouse button and drag across the screen to "Scrub"
*/

// Requires CProcessing library
#include "CProcessing.h"
#include "utils.h"
#include "timer.h"
#include "debug.h"
#include "dirt.h"
#include "img_font_init.h"

// Bool to check if sponge is equipped by player
static int sponge_equipped;

// Required parameters to draw the sponge

static float sponge_x;
static float sponge_y;
static float sponge_height = 100.0f;
static float sponge_width = 100.0f;
static float sponge_vx = 0.0f;
static float sponge_vy = 0.0f;
static float prev_mouse_x = 0.0f;
static float prev_mouse_y = 0.0f;
static float sponge_bounce = 0.8f;


// How much damage the sponge does
static int sponge_power = 5;

static const int init_sponge_base_power = 5;
static const int init_sponge_max_power = 100;

// Bool to check if the player is scrubbing while using the sponge
int IsScrubbing(void) {
	return (sponge_equipped == 1);
}

int IsSpongeEquipped(void) {
	return sponge_equipped;
}

int GetSpongePower(void) {
	return sponge_power;
}
int SpongeUpgradeable(void) {
	return sponge_power < init_sponge_max_power ? 1 : 0;
}

void UpgradeSponge(void) {
	if (SpongeUpgradeable()) {
		sponge_power += 2;
		if (sponge_power > init_sponge_max_power) {
			sponge_power = init_sponge_max_power;
		}
	}
}

void SpongeReset(void) {
	sponge_equipped = 0;
	sponge_power = init_sponge_base_power;
	sponge_x = 335.0f;
	sponge_y = 630.0f;

	sponge_x = 100.0f;
	sponge_y = 220.0f;

	Reset_SpongeUpgradeCost();
	sponge_x = 335.0f;
	sponge_y = 630.0f;
}

static void SpongeInput(void) {

	// Track mouse velocity while equipped
	if (sponge_equipped == 1) {
		float mx = CP_Input_GetMouseX();
		float my = CP_Input_GetMouseY();

		sponge_vx = mx - prev_mouse_x;
		sponge_vy = my - prev_mouse_y;

		prev_mouse_x = mx;
		prev_mouse_y = my;
	}

	// Equip when clicked
	if (CP_Input_MouseDown(MOUSE_BUTTON_1) &&
		IsAreaClicked(sponge_x, sponge_y, sponge_height, sponge_width,
		CP_Input_GetMouseX(), CP_Input_GetMouseY())) {

		sponge_equipped = 1;
		prev_mouse_x = CP_Input_GetMouseX();
		prev_mouse_y = CP_Input_GetMouseY();
	}

	else if (CP_Input_MouseReleased(MOUSE_BUTTON_1)) {
		sponge_equipped = 0;
	}
}


void SpongeInit(void) {
	if (CheckGameRunning()) {
		// Sponge not equipped by player, sets its location back to the original position
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
		if (sponge_equipped != 1) {
			sponge_x += sponge_vx;
			sponge_y += sponge_vy;
			// Optional damping so it slows down naturally
			sponge_vx *= 0.90f;
			sponge_vy *= 0.90f;
			// Left wall
			if (sponge_x - sponge_width / 2 < 0) {
				sponge_x = sponge_width / 2;
				sponge_vx *= -sponge_bounce;
			}

			// Right wall
			if (sponge_x + sponge_width / 2 > CP_System_GetWindowWidth()) {
				sponge_x = CP_System_GetWindowWidth() - sponge_width / 2;
				sponge_vx *= -sponge_bounce;
			}

			// Top wall
			if (sponge_y - sponge_height / 2 < 0) {
				sponge_y = sponge_height / 2;
				sponge_vy *= -sponge_bounce;
			}

			// Bottom wall
			if (sponge_y + sponge_height / 2 > CP_System_GetWindowHeight()) {
				sponge_y = CP_System_GetWindowHeight() - sponge_height / 2;
				sponge_vy *= -sponge_bounce;
			}
			CP_System_ShowCursor(1); // Hide cursor when sponge is equipped
			CP_Settings_TextSize(20.0f);
			CP_Font_DrawText("Equip [E]", 335.0f, 700.0f);
		} 
		// player is now equipped with the sponge, sponge will follow the mouse
		else {
			// Equipped mouse
			sponge_x = CP_Input_GetMouseX();
			sponge_y = CP_Input_GetMouseY();
			CP_System_ShowCursor(0); // Hide cursor when sponge is equipped
			CP_Settings_TextSize(20.0f);
			CP_Font_DrawText("Unequip [E]", 335.0f, 700.0f);
		}
		// Handle input for the sponge
		SpongeInput();
	}
	
	// Draw the sponge
	if (!IsCurrentlyDebugging()) {
		CP_Image_Draw(sponge_cat, sponge_x, sponge_y, sponge_height, sponge_width, 255);
	}
	else {
		CP_Settings_Fill(CP_Color_Create(255, 0, 0, 150));
		CP_Graphics_DrawCircle(sponge_x, sponge_y, GetSpongeRadius() * 2);
	}


}

void UnequipSponge(void) {
	sponge_equipped = 0;
	CP_System_ShowCursor(1);
}
