#include "cprocessing.h"
#include <stdio.h>

#define MAX_DROPLETS		250
#define AOE_duration		5.0f

typedef struct {
	CP_Vector position;
	CP_Vector velocity;
	float radius;
	int   isRunning;
} Stream;

Stream streamlist[MAX_DROPLETS];

static float rotation1 = 45.0f;
static float rotation2 = 90.0f;
static int stream_on = 0;
static int faucet_on = 0;
static float aoe_time_left = 0.0f;

void stream_init(void) {
	for (int i = 0; i < MAX_DROPLETS; i++) {
		float center_x = CP_System_GetWindowWidth() * 0.5f;
		float offset = 300.0f; //diameter of plate = 600.0f (plate.c)

		streamlist[i].position = CP_Vector_Set(CP_Random_RangeFloat(center_x - offset, center_x + offset), CP_Random_RangeFloat((float)-CP_System_GetWindowHeight(), -5.0f));
		streamlist[i].velocity = CP_Vector_Set(0, CP_Random_RangeFloat(4, 8));
	}
}

void update_stream(void) {
	for (int i = 0; i < MAX_DROPLETS; i++) {
		streamlist[i].position = CP_Vector_Add(streamlist[i].position, streamlist[i].velocity);
		if (streamlist[i].position.y > CP_System_GetWindowHeight()) {
			streamlist[i].position.y = CP_Random_RangeFloat(-100.0f, 0.0f);
		}
		CP_Settings_Stroke(CP_Color_Create(123, 183, 220, 100));
		CP_Settings_Fill(CP_Color_Create(135, 206, 235, 150));
		CP_Graphics_DrawCircle(streamlist[i].position.x, streamlist[i].position.y, 40.0f);
		CP_Settings_NoStroke();
	}
}


void AOE_stream(void) {
	if (CP_Input_KeyTriggered(KEY_W)) {
		stream_on = 1;
		aoe_time_left = AOE_duration;
	}

	//timer set to 5 seconds for AOE attack
	if (stream_on == 1) {
		update_stream();
		aoe_time_left -= CP_System_GetDt();
		if (aoe_time_left <= 0.0f) { stream_on = 0; }
	}
}

int movement = 1;

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


