#include "cprocessing.h"
#include "utils.h"

// initialise array containing all the x coords for dirt
float dirt_x[] = { 0,0,0,0,0 };
// initialise array containing all the y coords for dirt
float dirt_y[] = { 0,0,0,0,0 };
// initialise array contianing all the opacity values for dirt
int opacities[] = { 220,220,220,220,220 };
// initialise array that contains the dirt RGB values (done this way so that opacity values can be
// edited separately.)
int dirt_rgb[] = { 157,92,61 };

// function to spawn new dirt
void spawn_dirt(new_game) {
		// Checks if new game variable is true, if so, reset all opacity values to 200 and generate
		// new x y values for all dirt 
		if(new_game) {
			for (int i = 0; i < 5; i++) {
				opacities[i] = 220;
				dirt_x[i] = (float)CP_System_GetWindowWidth() / 2 - 250 + CP_Random_RangeInt(0, 500);
				dirt_y[i] = (float)CP_System_GetWindowHeight() / 2 - 250 + CP_Random_RangeInt(0, 500);
			}
		};
	// prints all the dirt with their corresponding opacity values
	for (int i = 0; i < 5; i++) {
		CP_Settings_NoStroke();
		CP_Settings_Fill(CP_Color_Create(dirt_rgb[0], dirt_rgb[1], dirt_rgb[2], opacities[i]));
		CP_Graphics_DrawCircle(dirt_x[i], dirt_y[i], 100);
		CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	}

}

// function to check if required conditions are met to start reducing opacity of dirt
void dirt_scrubbed(equipped,sponge_power) {
	for (int i = 0; i < 5; i++) {
		// currently checks for if sponge is equipped, sponge intersects with dirt and if mouse is being moved(scrubbing)
		if (equipped && AreCirclesIntersecting(dirt_x[i], dirt_y[i], 50, CP_Input_GetMouseX(), CP_Input_GetMouseY(), 50) && CP_Input_MouseMoved()) {
			opacities[i] -= sponge_power;
			opacities[i] = CP_Math_ClampInt(opacities[i], 0, 200);
		}
	}
}

//function to check if total opacity of all dirt is less than  1 (all dirt cleared)
int dirt_removed(void) {
	int total_opacity = 0;
	for (int i = 0; i < 5; i++) {
		total_opacity += opacities[i];
	}
	if (total_opacity < 1) {
		return 1;
	}
	else {
		return 0;
	}
}