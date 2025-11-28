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

#define MAX_DIRT			50
#define DIRT_RADIUS			50
#define SPONGE_RADIUS		50		

dirt dirt_list[MAX_DIRT];

int num_of_dirt = 5;
int total_opacity = 0;
int currently_scrubbbing = 0;
float time_after_generation = 0.0f;

int GetSpongeRadius(void) {
	return SPONGE_RADIUS;
}

int GetTotalOpacity(void) {
	return total_opacity;
}

void SetNumberOfDirt(int num) {
	int current_day = DayGetDay();

	// Increase dirt count by 1% each day to progressively raise difficulty.
	float difficulty_multiplier = 1.0f + 0.01f * current_day;
	int adjusted_amount = (int)(num * difficulty_multiplier);
	adjusted_amount = CP_Math_ClampInt(adjusted_amount, 1, MAX_DIRT);

	num_of_dirt = adjusted_amount;
}

int GetNumberOfDirt(void) {
	return num_of_dirt;
}

static int plate_awarded = 0;   // 0 = not yet awarded for this plate

void InitDirt(void) {
	for (int i = 0; i < MAX_DIRT; i++) {
		dirt_list[i].position_x = (float)CP_System_GetWindowWidth() / 2 - 250 + CP_Random_RangeInt(0, 500);
		dirt_list[i].position_y = (float)CP_System_GetWindowHeight() / 2 - 250 + CP_Random_RangeInt(0, 500);
		dirt_list[i].opacity = 220;
		dirt_list[i].size = DIRT_RADIUS;
	}
	time_after_generation = 0.0f;
	plate_awarded = 0;  // <-- reset award latch when new dirt/plate is created
}

void ReduceAllDirtOpacity(int amount) {
	for (int i = 0; i < num_of_dirt; i++) {
		dirt_list[i].opacity -= amount;
		dirt_list[i].opacity = CP_Math_ClampInt(dirt_list[i].opacity, 0, 220);
	}
}

void DrawDirt(void)
{
	for (int i = 0; i < num_of_dirt; i++) {
		CP_Settings_NoStroke();
		CP_Settings_Fill(CP_Color_Create(157, 92, 61, dirt_list[i].opacity));
		CP_Graphics_DrawCircle(dirt_list[i].position_x, dirt_list[i].position_y, dirt_list[i].size * 2);
		CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	}
	DirtPointer();
}

// function to check if required conditions are met to start reducing opacity of dirt


void DirtScrubbed(int equipped, int sponge_power)
{
	for (int i = 0; i < num_of_dirt; i++) {
		// currently checks for if sponge is equipped, sponge intersects with dirt and if mouse is being moved(scrubbing)
		if (equipped && AreCirclesIntersecting(dirt_list[i].position_x, dirt_list[i].position_y, DIRT_RADIUS, CP_Input_GetMouseX(), CP_Input_GetMouseY(), SPONGE_RADIUS) && CP_Input_MouseMoved()) {
			dirt_list[i].opacity -= sponge_power;
			dirt_list[i].opacity = CP_Math_ClampInt(dirt_list[i].opacity, 0, 220);
		}
	}
}

//function to check if total opacity of all dirt is less than  1 (all dirt cleared)
int DirtRemoved(void) {
	total_opacity = 0;
	for (int i = 0; i < num_of_dirt; i++) {
		total_opacity += dirt_list[i].opacity;
	}
	if (total_opacity < 1) {
		if (!plate_awarded) {                  // <-- only pay once
			IncrementMoney(GetPlateValue());
			plate_awarded = 1;
			time_after_generation = 0.0f;
			TimeAdd(GetPlateValue());
			return 1;
		}
		return 0;                              // already paid; don't trigger again
	}
	else {
		return 0;
	}
}

void DirtPointer(void) {
	time_after_generation += CP_System_GetDt();
	if (time_after_generation > 10.0f) {
		for (int i = 0; i < num_of_dirt; i++) {
			if (dirt_list[i].opacity < 50 && dirt_list[i].opacity != 0) {
				CP_Settings_NoFill();
				CP_Settings_Stroke(CP_Color_Create(255, 0, 0, 255));
				CP_Graphics_DrawCircle(dirt_list[i].position_x, dirt_list[i].position_y, DIRT_RADIUS * 2 + 10);
			}
		}
	}
}
