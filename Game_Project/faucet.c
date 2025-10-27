#include "cprocessing.h"
#include <stdio.h>
#include "dirt.h"

#define MAX_DROPLETS		220
#define AOE_duration		3.0f

typedef struct {
	CP_Vector	position;
	CP_Vector	velocity;
	float		radius;
} Stream;

Stream streamlist[MAX_DROPLETS];

//*----------------------   DRAWING FUNCTIONS : FOR STREAM AND FAUCET   ------------------------*//

static int		stream_on = 0;
static int		faucet_on = 0;
static float	rotation1 = 45.0f;
static float	rotation2 = 90.0f;
int				movement = 1;

void draw_stream(void) {
	for (int i = 0; i < MAX_DROPLETS; i++) {
		CP_Settings_Stroke(CP_Color_Create(123, 183, 220, 100));
		CP_Settings_Fill(CP_Color_Create(135, 206, 235, 120));
		CP_Graphics_DrawCircle(streamlist[i].position.x, streamlist[i].position.y, 40.0f);
		CP_Settings_NoStroke();
	}
}

void draw_faucet(void) {
	float faucet_x = CP_System_GetWindowWidth() * 0.5f;
	float faucet_y = 35.0f;
	float radius = 140.0f;
	float rotation1_max = 135.0f;
	float rotation2_max = 180.0f;

	if (CP_Input_KeyTriggered(KEY_W))
		faucet_on = 1;

	if (faucet_on == 1) {
		rotation1 += 60.0f * CP_System_GetDt() * movement;
		rotation2 += 60.0f * CP_System_GetDt() * movement;
		if (rotation1 >= rotation1_max && rotation2 >= rotation2_max) {
			movement = -1;
		}
		if (movement == -1 && rotation1 <= 45.0f && rotation2 <= 90.0f) {
			movement = 1;
			faucet_on = 0;
		}
	}

	CP_Settings_Stroke(CP_Color_Create(206, 216, 220, 250));
	CP_Settings_Fill(CP_Color_Create(164, 164, 164, 255));
	CP_Graphics_DrawRectAdvanced(faucet_x, faucet_y, radius, radius, rotation1, 5.0f);
	CP_Graphics_DrawRectAdvanced(faucet_x, faucet_y, radius, radius, rotation2, 5.0f);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 225));
	CP_Graphics_DrawCircle(faucet_x, faucet_y, radius - 20.0f);
}

//*-----------------------------------   STREAM CONTROLS   -------------------------------------*//

void stream_init(void) {
	float center_x = CP_System_GetWindowWidth() * 0.5f;
	float offset = 300.0f; //diameter of plate = 600.0f (plate.c)

	for (int i = 0; i < MAX_DROPLETS; i++) {
		streamlist[i].position = CP_Vector_Set(CP_Random_RangeFloat(center_x - offset, center_x + offset), 
								 CP_Random_RangeFloat((float)-CP_System_GetWindowHeight(), -20.0f));
		streamlist[i].velocity = CP_Vector_Set(0, 0);
	}
}

void update_stream(void) {
	for (int i = 0; i < MAX_DROPLETS; i++) {
		streamlist[i].velocity = CP_Vector_Set(0, CP_Random_RangeFloat(4, 8));
		streamlist[i].position = CP_Vector_Add(streamlist[i].position, streamlist[i].velocity);

		if (streamlist[i].position.y > CP_System_GetWindowHeight())
			streamlist[i].position.y = CP_Random_RangeFloat(-100.0f, 0.0f);
	}
}

// function to stop the stream but ensure the droplets keep falling; to reset their position, 
// with no velocity when they go off screen

void stop_stream(void) {
	float center_x = CP_System_GetWindowWidth() * 0.5f;
	float offset = 300.0f; 

	for (int i = 0; i < MAX_DROPLETS; i++) {
		streamlist[i].position = CP_Vector_Add(streamlist[i].position, streamlist[i].velocity);

		if (streamlist[i].position.y > CP_System_GetWindowHeight()) {
			streamlist[i].position.y = CP_Random_RangeFloat(-100.0f, 0.0f);
			streamlist[i].position = CP_Vector_Set(CP_Random_RangeFloat(center_x - offset, center_x + offset),
									 CP_Random_RangeFloat((float)-CP_System_GetWindowHeight(), -20.0f));
			streamlist[i].velocity = CP_Vector_Set(0, 0);
		}

	}
}

//*---------------------   STREAM IMPLEMENTATION : AOE CLEANING EFFECT   -----------------------*//

static float	aoe_time_left = 0.0f;

void clean_dirt_with_stream(int attack_opacity) {
	for (int i = 0; i < MAX_DROPLETS; i++) {
		for (int j = 0; j < get_number_of_dirt(); j++) {
			float distance = CP_Math_Distance(streamlist[i].position.x, streamlist[i].position.y, dirtList[j].positionX, dirtList[j].positionY);
			float cleaning_radius = 50.0f;

			if (distance < cleaning_radius) {
				dirtList[j].opacity -= attack_opacity;
				if (dirtList[j].opacity < 0)
					dirtList[j].opacity = 0;
			}
		}
	}
}

void AOE_stream(int attack_opacity) {

	draw_stream();
	clean_dirt_with_stream(attack_opacity);

	if (CP_Input_KeyTriggered(KEY_W)) {
		stream_on = 1;
		aoe_time_left = AOE_duration;
	}
	//set to 3 seconds for AOE attack
	if (stream_on == 1) {
		update_stream();
		aoe_time_left -= CP_System_GetDt();
		if (aoe_time_left <= 0.0f) 
			stream_on = 0;
	} else {	
		stop_stream();	
	}
}

static float time_on = AOE_duration;
static float time_left = 0.0f;

static float cooldown = 20.0f;
static float cooldown_left = 0.0f;

int opacity = 0;

/*
void cooldown_timer_stream(void) {
	float radius = 50.0f;
	float faucet_x = 105.0f;
	float faucet_y = 730.0f;
	int max_opacity = 150;

	// white circular background for logo
	CP_Settings_NoStroke();
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Graphics_DrawCircle(faucet_x, faucet_y, 2 * radius + 0.5f);
	
	CP_Settings_Stroke(CP_Color_Create(206, 216, 220, 250));
	CP_Settings_Fill(CP_Color_Create(164, 164, 164, 255));
	CP_Graphics_DrawRectAdvanced(faucet_x, faucet_y, radius, radius, rotation1, 5.0f);
	CP_Graphics_DrawRectAdvanced(faucet_x, faucet_y, radius, radius, rotation2, 5.0f);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 225));
	CP_Graphics_DrawCircle(faucet_x, faucet_y, radius - 20.0f);

	//fading background, controlling alpha value based on cooldown

	CP_Settings_NoStroke();
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, opacity));
	CP_Graphics_DrawCircle(faucet_x, faucet_y, 2 * radius + 0.5f);
}
*/


