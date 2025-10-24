#include "cprocessing.h"
#include "utils.h"
#include "bubbles.h"
#include "dirt.h"
#include "money.h"
#include "plate.h"

#define MAX_DIRT			50
#define DIRT_RADIUS			50
#define SPONGE_RADIUS		50		

dirt dirtList[MAX_DIRT];

int num_of_dirt = 5;

void set_number_of_dirt(int num) {
	num_of_dirt = num;
}

int get_number_of_dirt(void) {
	return num_of_dirt;
}

static int plate_awarded = 0;   // 0 = not yet awarded for this plate

void init_dirt(void) {
	for (int i = 0; i < MAX_DIRT; i++) {
		dirtList[i].positionX = (float)CP_System_GetWindowWidth() / 2 - 250 + CP_Random_RangeInt(0, 500);
		dirtList[i].positionY = (float)CP_System_GetWindowHeight() / 2 - 250 + CP_Random_RangeInt(0, 500);
		dirtList[i].opacity = 220;
	}
	plate_awarded = 0;  // <-- reset award latch when new dirt/plate is created
}

void draw_dirt(void)
{
	for (int i = 0; i < num_of_dirt; i++) {
		CP_Settings_NoStroke();
		CP_Settings_Fill(CP_Color_Create(157, 92, 61, dirtList[i].opacity));
		CP_Graphics_DrawCircle(dirtList[i].positionX, dirtList[i].positionY, DIRT_RADIUS * 2);
		CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	}
}

// function to check if required conditions are met to start reducing opacity of dirt


void dirt_scrubbed(int equipped, int sponge_power)
{
	for (int i = 0; i < num_of_dirt; i++) {
		// currently checks for if sponge is equipped, sponge intersects with dirt and if mouse is being moved(scrubbing)
		if (equipped && AreCirclesIntersecting(dirtList[i].positionX, dirtList[i].positionY, DIRT_RADIUS, CP_Input_GetMouseX(), CP_Input_GetMouseY(), SPONGE_RADIUS) && CP_Input_MouseMoved()) {
			dirtList[i].opacity -= sponge_power;
			dirtList[i].opacity = CP_Math_ClampInt(dirtList[i].opacity, 0, 220);
		}
	}
}

//function to check if total opacity of all dirt is less than  1 (all dirt cleared)
int dirt_removed(void) {
	int total_opacity = 0;
	for (int i = 0; i < num_of_dirt; i++) {
		total_opacity += dirtList[i].opacity;
	}
	if (total_opacity < 1) {
		if (!plate_awarded) {                  // <-- only pay once
			increment_money(get_plate_value());
			plate_awarded = 1;
			return 1;
		}
		return 0;                              // already paid; don't trigger again
	}
	else {
		return 0;
	}
}
