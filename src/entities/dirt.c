/*************************************************************************
@file		dirt.c
@Author		Lim Liang Zhou l.liangzhou@digipen.edu
@Co-authors	nil
@brief		This file contains the function definitions for managing dirt
			entities, including their initialization, rendering, opacity
			reduction when scrubbed, and checking for removal conditions.

Copyright © 2025 DigiPen, All rights reserved.
*************************************************************************/
#include "cprocessing.h"
#include "utils.h"
#include "bubbles.h"
#include "dirt.h"
#include "money.h"
#include "plate.h"
#include "timer.h"
#include "day.h"

#define MAX_DIRT			50   // max dirt allowed in the list
#define DIRT_RADIUS			50   // base dirt collision/draw radius
#define SPONGE_RADIUS		50   // sponge collision radius

dirt dirt_list[MAX_DIRT];       // global list of dirt objects

int num_of_dirt = 5;            // how many dirt spots are currently used
int total_opacity = 0;          // total opacity sum (used to detect “all cleared”)
int currently_scrubbbing = 0;   // flag (currently unused here)
float time_after_generation = 0.0f; // time since dirt was generated (for hint pointer)

int GetSpongeRadius(void) {
	return SPONGE_RADIUS;        // expose sponge radius to other systems
}

int GetTotalOpacity(void) {
	return total_opacity;        // expose total opacity (updated in DirtRemoved)
}

void SetNumberOfDirt(int num) {
	int current_day = DayGetDay(); // get current day number

	// Increase dirt count by 1% each day to progressively raise difficulty.
	float difficulty_multiplier = 1.0f + 0.01f * current_day;
	int adjusted_amount = (int)(num * difficulty_multiplier);

	// clamp so we never exceed bounds and always have at least 1 dirt
	adjusted_amount = CP_Math_ClampInt(adjusted_amount, 1, MAX_DIRT);

	// store final amount used by all dirt loops
	num_of_dirt = adjusted_amount;
}

int GetNumberOfDirt(void) {
	return num_of_dirt;          // returns current number of dirt spots
}

static int plate_awarded = 0;   // latch: 0 = money not awarded yet, 1 = already awarded

void InitDirt(void) {
	// initialize each dirt spot's position, opacity, and size
	for (int i = 0; i < MAX_DIRT; i++) {
		// random position around center-ish area (500x500 region)
		dirt_list[i].position_x = (float)CP_System_GetWindowWidth() / 2 - 250 + CP_Random_RangeInt(0, 500);
		dirt_list[i].position_y = (float)CP_System_GetWindowHeight() / 2 - 250 + CP_Random_RangeInt(0, 500);
		dirt_list[i].opacity = 220;   // start fully visible
		dirt_list[i].size = DIRT_RADIUS;
	}

	// reset hint timer + payout latch for the next dirt/plate cycle
	time_after_generation = 0.0f;
	plate_awarded = 0;  // <-- reset award latch when new dirt/plate is created
}

void ReduceAllDirtOpacity(int amount) {
	// reduce opacity for all active dirt spots
	for (int i = 0; i < num_of_dirt; i++) {
		dirt_list[i].opacity -= amount;
		dirt_list[i].opacity = CP_Math_ClampInt(dirt_list[i].opacity, 0, 220); // keep in valid range
	}
}

void DrawDirt(void)
{
	// draw each dirt spot as a colored circle with alpha = opacity
	for (int i = 0; i < num_of_dirt; i++) {
		CP_Settings_NoStroke();
		CP_Settings_Fill(CP_Color_Create(157, 92, 61, dirt_list[i].opacity));
		CP_Graphics_DrawCircle(dirt_list[i].position_x, dirt_list[i].position_y, dirt_list[i].size * 2);

		// restore stroke/fill defaults after drawing each circle
		CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	}

	// optionally draws a pointer highlight after some time passes
	DirtPointer();
}

// function to check if required conditions are met to start reducing opacity of dirt


void DirtScrubbed(int equipped, int sponge_power)
{
	// reduce opacity when sponge is equipped and the player is scrubbing over dirt
	for (int i = 0; i < num_of_dirt; i++) {
		// checks:
		// - sponge equipped
		// - sponge circle intersects dirt circle
		// - mouse moved this frame (scrubbing motion)
		if (equipped &&
			AreCirclesIntersecting(dirt_list[i].position_x, dirt_list[i].position_y, DIRT_RADIUS,
				CP_Input_GetMouseX(), CP_Input_GetMouseY(), SPONGE_RADIUS) &&
			CP_Input_MouseMoved()) {

			// reduce opacity by sponge power and clamp
			dirt_list[i].opacity -= sponge_power;
			dirt_list[i].opacity = CP_Math_ClampInt(dirt_list[i].opacity, 0, 220);
		}
	}
}

//function to check if total opacity of all dirt is less than  1 (all dirt cleared)
int DirtRemoved(void) {
	// recompute total opacity each call
	total_opacity = 0;
	for (int i = 0; i < num_of_dirt; i++) {
		total_opacity += dirt_list[i].opacity;
	}

	// if all dirt has reached opacity 0, trigger clear condition
	if (total_opacity < 1) {
		// only award money/time once per cleared plate
		if (!plate_awarded) {                  // <-- only pay once
			IncrementMoney(GetPlateValue());   // give player reward
			plate_awarded = 1;                 // latch so it doesn’t repeat
			time_after_generation = 0.0f;      // reset hint timer for next cycle
			TimeAdd(GetPlateValue());          // add extra time based on plate value
			return 1;                          // signal cleared this frame
		}
		return 0;                              // already paid; don't trigger again
	}
	else {
		return 0;                              // not cleared yet
	}
}

void DirtPointer(void) {
	// count time since generation
	time_after_generation += CP_System_GetDt();

	// after 10 seconds, highlight low-opacity dirt spots to help player find them
	if (time_after_generation > 10.0f) {
		for (int i = 0; i < num_of_dirt; i++) {
			// show pointer if dirt is almost gone but not fully removed
			if (dirt_list[i].opacity < 50 && dirt_list[i].opacity != 0) {
				CP_Settings_NoFill();
				CP_Settings_Stroke(CP_Color_Create(255, 0, 0, 255));
				CP_Graphics_DrawCircle(dirt_list[i].position_x, dirt_list[i].position_y, DIRT_RADIUS * 2 + 10);
			}
		}
	}
}
