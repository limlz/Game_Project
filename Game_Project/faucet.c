#include "cprocessing.h"
#include <stdio.h>

#define MAX_DROPLETS		200

typedef struct {
	CP_Vector position;
	CP_Vector positionY;
	CP_Vector velocity;
	float radius;
	int   isRunning;
} Stream;

Stream streamlist[MAX_DROPLETS];

static float rotation1 = 45.0f;
static float rotation2 = 90.0f;
static int faucet_on = 0;


void stream_init(void) {
	for (int i = 0; i < MAX_DROPLETS; i++) {
		float center_x = CP_System_GetWindowWidth() * 0.5f;
		float offset = 300.0f; //diameter of plate = 600.0f (plate.c)

		streamlist[i].position = CP_Vector_Set(CP_Random_RangeFloat(center_x - offset, center_x + offset), 0.0f);
		streamlist[i].velocity = CP_Vector_Set(0, CP_Random_RangeFloat(4, 8));
	}
}

/*
void update_stream(void) {
	for (int i = 0; i < MAX_DROPLETS; i++) {
		streamlist[i].position = CP_Vector_Add(streamlist[i].position, streamlist[i].velocity);
		if (streamlist[i].position.y > CP_System_GetWindowHeight()) {
			streamlist[i].position.y = CP_Random_RangeFloat(-100, 0);
		}
		CP_Settings_NoStroke();
		CP_Settings_Fill(CP_Color_Create(135, 206, 235, 150));
		CP_Graphics_DrawCircle(streamlist[i].position.x, streamlist[i].position.y, 30.0f);
	}
}
*/

void draw_sink (void) {
	float center_x = CP_System_GetWindowWidth() * 0.5f;
	float center_y = CP_System_GetWindowHeight() * 0.5f;

	CP_Settings_Stroke(CP_Color_Create(161, 161, 161, 10));
	CP_Settings_Fill(CP_Color_Create(205, 205, 205, 100));
	CP_Graphics_DrawRectAdvanced(center_x, center_y, 1150.0f, 650.0f, 0.0f, 0.0f);
	CP_Settings_Fill(CP_Color_Create(245, 245, 245, 100));
	CP_Graphics_DrawRectAdvanced(center_x, center_y, 980.0f, 520.0f, 0.0f, 0.0f);
}

void draw_faucet(void) {
	float faucet_x = CP_System_GetWindowWidth() * 0.5f;
	float faucet_y = 35.0f;
	float radius = 140.0f;


	if (CP_Input_KeyTriggered(KEY_W)) {
		faucet_on = !faucet_on;
		if (faucet_on == 1) {
			rotation1 = 125.0f;
			rotation2 = 170.0f;

		} else {
			rotation1 = 45.0f;
			rotation2 = 90.0f;
		}
	} 

	CP_Settings_Stroke(CP_Color_Create(206, 216, 220, 250));
	CP_Settings_Fill(CP_Color_Create(164, 164, 164, 255));
	CP_Graphics_DrawRectAdvanced(faucet_x, faucet_y, radius, radius, rotation1, 5.0f);
	CP_Graphics_DrawRectAdvanced(faucet_x, faucet_y, radius, radius, rotation2, 5.0f);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 225));
	CP_Graphics_DrawCircle(faucet_x, faucet_y, radius - 20.0f);
}


