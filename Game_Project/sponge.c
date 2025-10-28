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

// Bool to check if sponge is equipped by player
static int sponge_equipped;

// Required parameters to draw the sponge
static float sponge_x;
static float sponge_y;
static float sponge_height = 100.0f;
static float sponge_width = 100.0f;

// How much damage the sponge does
static int sponge_power = 5;

static const int kSpongeBasePower = 5;
static const int kSpongeMaxPower = 100;

// Bool to check if the player is scrubbing while using the sponge
int is_Scrubbing(void) {
	return (sponge_equipped == 1 && CP_Input_MouseDragged(MOUSE_BUTTON_LEFT));
}

int is_SpongeEquipped(void) {
	return sponge_equipped;
}

int get_SpongePower(void) {
	return sponge_power;
}
int sponge_upgradeable(void) {
	return sponge_power < kSpongeMaxPower ? 1 : 0;
}

void upgrade_Sponge(void) {
	if (sponge_upgradeable()) {
		sponge_power += 5;
		if (sponge_power > kSpongeMaxPower) {
			sponge_power = kSpongeMaxPower;
		}
	}
}

void sponge_reset(void) {
	sponge_equipped = 0;
	sponge_power = kSpongeBasePower;
	sponge_x = 100.0f;
	sponge_y = 220.0f;
}

static void sponge_input(void) {
	// Check if 'E' key is pressed and mouse is over the sponge
	if (CP_Input_KeyTriggered(KEY_E) && IsAreaClicked(sponge_x, sponge_y, sponge_height, sponge_width, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
		// Toggle sponge equipped state
		sponge_equipped = sponge_equipped == 0 ? 1 : 0;
	}
}

void sponge_init(void) {
	if (checkGameRunning()) {
		// Sponge not equipped by player, sets its location back to the original position
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 100));
		if (sponge_equipped != 1) {
			sponge_x = 320.0f;
			sponge_y = 670.0f;

			CP_Settings_TextSize(20.0f);
			CP_Font_DrawText("Equip [E]", sponge_x, sponge_y + 110);
		} 
		// player is now equipped with the sponge, sponge will follow the mouse
		else {
			sponge_x = CP_Input_GetMouseX();
			sponge_y = CP_Input_GetMouseY();

			CP_Settings_TextSize(20.0f);
			CP_Font_DrawText("Unequip [E]", sponge_x, sponge_y + 110);
		}
		// Handle input for the sponge
		sponge_input();
	}
	
	// Draw the sponge
	if (!IsCurrentlyDebugging()) {
		CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
		CP_Settings_Fill(CP_Color_Create(255, 255, 0, 255));
		CP_Graphics_DrawRect(sponge_x, sponge_y, sponge_height, sponge_width);
	}
	else {
		CP_Settings_Fill(CP_Color_Create(255, 0, 0, 150));
		CP_Graphics_DrawCircle(sponge_x, sponge_y, GetSpongeRadius() * 2);
	}


}
